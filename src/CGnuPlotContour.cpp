#include <CGnuPlotContour.h>
#include <CGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPathObject.h>
#include <CRGBName.h>

static int contour_flags[] = {
  0, 4, // bottom edge to diagonals
  0, 5,
  6, 1, // top edge to diagonals
  7, 1,

  2, 4, // left edge to diagonals
  2, 6,
  5, 3, // right edge to diagonals
  7, 3,

  4, 5, // diagonal to diagonal
  4, 6,
  5, 7,
  6, 7,
};

CGnuPlotContour::
CGnuPlotContour(CGnuPlotPlot *plot) :
 plot_(plot)
{
  min_x_ = 1E-6;
  min_y_ = 1E-6;
  min_z_ = -1.0;
}

CGnuPlot *
CGnuPlotContour::
app() const
{
  return plot_->app();
}

void
CGnuPlotContour::
reset()
{
  x_.clear();
  y_.clear();
  z_.clear();
  c_.clear();

  levels_.clear();
}

void
CGnuPlotContour::
setData(double *x, double *y, double *z, double *c, int no_x, int no_y, bool has_c)
{
  const auto &contourData = plot_->contourData();

  int no_z = no_x*no_y;

  x_ = std::vector<double>(&x[0], &x[no_x]);
  y_ = std::vector<double>(&y[0], &y[no_y]);
  z_ = std::vector<double>(&z[0], &z[no_z]);

  if (has_c)
    c_ = std::vector<double>(&c[0], &c[no_z]);

  xmin_.setInvalid(); ymin_.setInvalid(); zmin_.setInvalid();
  xmax_.setInvalid(); ymax_.setInvalid(); zmax_.setInvalid();

  for (auto x : x_) { xmin_.updateMin(x); xmax_.updateMax(x); }
  for (auto y : y_) { ymin_.updateMin(y); ymax_.updateMax(y); }
  for (auto z : z_) { zmin_.updateMin(z); zmax_.updateMax(z); }
  for (auto c : c_) { cmin_.updateMin(c); cmax_.updateMax(c); }

  auto *group = plot_->group();

  min_z_ = group->cameraPlaneZMin();
  pos_   = contourData.pos();

  calcContourLines();
}

void
CGnuPlotContour::
setContourLevels(const std::vector<double> &levels)
{
  levels_ = levels;
}

void
CGnuPlotContour::
drawContour(CGnuPlotRenderer *renderer)
{
  auto *group = plot_->group();

  const auto &contourData = plot_->contourData();

  //CGnuPlotAxisData &zaxis = group->zaxis(plot_->zind());

  auto min_z = group->cameraPlaneZMin();
  auto pos   = contourData.pos();

  if (min_z != min_z_ || pos != pos_) {
    min_z_ = min_z;
    pos_   = pos;

    calcContourLines();
  }

  //---

  if (solid())
    drawContourSolid(renderer);
  else
    drawContourLines(renderer);
}

void
CGnuPlotContour::
drawContourLines(CGnuPlotRenderer *renderer)
{
  std::vector<double> levels;

  getLevels(levels);

  //---

  const CGnuPlotTextStyle &textStyle = plot_->textStyle();

  const auto &contourData = plot_->contourData();
  const auto &lineStyle   = plot_->lineStyle();

  // draw contour points
  for (uint j = 0; j < y_.size() - 1; ++j) {
    auto y = y_[j];

    auto j1 = j*x_.size();

    for (uint i = 0; i < x_.size() - 1; ++i) {
      auto x = x_[i];
      auto z = z_[j1 + i];

      if (pos_ == CGnuPlotContourData::DrawPos::BASE ||
          pos_ == CGnuPlotContourData::DrawPos::BOTH) {
        renderer->drawPoint(CPoint3D(x, y, min_z_), CRGBA(0,0,0));
      }
      if (pos_ == CGnuPlotContourData::DrawPos::SURFACE ||
          pos_ == CGnuPlotContourData::DrawPos::BOTH) {
        renderer->drawPoint(CPoint3D(x, y, z), CRGBA(0,0,0));
      }
    }
  }

  //---

  if (! renderer->isPseudo()) {
    auto n = 0;

    for (const auto &llines : llines_) {
      const auto &lines = llines.second;

      Paths paths;

      getPaths(lines, paths);

      n += paths.size();
    }

    plot_->updatePathCacheSize(n);
  }

  //---

  auto i = 0;

  auto width = lineStyle.calcWidth();

  for (const auto &llines : llines_) {
    auto        l     = llines.first;
    const auto &lines = llines.second;

    auto c = levelColor(l);

    if (! renderer->isPseudo()) {
      Paths paths;

      getPaths(lines, paths);

      for (const auto &path : paths) {
        auto *pathObj = plot_->pathObjects()[i];

        pathObj->setPoints3D(path);

        if (! pathObj->testAndSetUsed()) {
          CGnuPlotStrokeP stroke1(pathObj->stroke()->dup());

          pathObj->stroke()->setEnabled(true);
          pathObj->stroke()->setColor  (c);
        }

        ++i;

        //---

        if (contourData.labelStart().isValid()) {
          CFontPtr oldFont = renderer->getFont();

          CFontPtr font = contourData.labelFont();

          renderer->setFont(font);

          int li = contourData.labelStart().getValue(-1);

          CPoint3D p1, p2;

          int j = 0;

          for (const auto &p2 : path) {
            if (j > 0 && j == li) {
              auto lp = (p1 + p2)/2;

              auto str = CStrUtil::strprintf("%g", levels[l]);

              CRGBA bg = CRGBA(0,0,0,0);
              CRGBA fg = CRGBA(0,0,0);

              CPoint2D lp1 = renderer->transform2D(lp);

              if (textStyle.isBoxed())
                (void) renderer->drawTextBox(lp1, str, false, CPoint2D(0, 0), bg, fg, 1);

              renderer->drawHAlignedText(lp, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                                         VAlignPos(CVALIGN_TYPE_CENTER, 0), str, fg);

              if (contourData.labelInterval().getValue(-1) > 0)
                li += contourData.labelInterval().getValue();
            }

            ++j;

            p1 = p2;
          }

          renderer->setFont(oldFont);
        }
      }
    }
    else {
      for (const auto &line : lines)
        renderer->drawLine(line.start(), line.end(), c, width);
    }
  }

  if (! renderer->isPseudo()) {
    for (const auto &path : plot_->pathObjects())
      path->draw(renderer);
  }
}

void
CGnuPlotContour::
calcContourLines()
{
  llines_.clear();

  //---

  std::vector<double> levels;

  getLevels(levels);

  //---

  int    flag[8];
  double xc[8], yc[8], zc[8];

  for (uint l = 0; l < levels.size(); ++l) {
    auto level = levels[l];

    if (y_.size() < 1) continue;

    for (uint j = 0; j < y_.size() - 1; ++j) {
      auto yj1 = y_[j + 0], yj2 = y_[j + 1];

      auto ym = CGnuPlotUtil::avg({yj1, yj2});

      auto j1 =  j     *x_.size();
      auto j2 = (j + 1)*x_.size();

      if (x_.size() < 1) continue;

      for (uint i = 0; i < x_.size() - 1; ++i) {
        auto xi1 = x_[i + 0], xi2 = x_[i + 1];

        auto xm = CGnuPlotUtil::avg({xi1, xi2});

        //---

        auto z1 = z_[j1 + i], z2 = z_[j1 + i + 1];
        auto z3 = z_[j2 + i], z4 = z_[j2 + i + 1];

        auto zm = CGnuPlotUtil::avg({z1, z2, z3, z4});

#if 0
        if (! c_.empty()) {
          z1 = c_[j1 + i], z2 = c_[j1 + i + 1];
          z3 = c_[j2 + i], z4 = c_[j2 + i + 1];

          zm = CGnuPlotUtil::avg({z1, z2, z3, z4});
        }
#endif

        //---

        // get point on lower side for specified value (if in range)
        if ((flag[0] = ((level >= z1 && level <= z2) || (level <= z1 && level >= z2)))) {
          xc[0] = (z1 != z2 ? CGnuPlotUtil::map(level, z1, z2, xi1, xi2) : xi1);
          yc[0] = yj1;
          zc[0] = level;
        }

        // get point on upper side for specified value (if in range)
        if ((flag[1] = ((level >= z3 && level <= z4) || (level <= z3 && level >= z4)))) {
          xc[1] = (z3 != z4 ? CGnuPlotUtil::map(level, z3, z4, xi1, xi2) : xi1);
          yc[1] = yj2;
          zc[1] = level;
        }

        // get point on left side for specified value (if in range)
        if ((flag[2] = ((level >= z1 && level <= z3) || (level <= z1 && level >= z3)))) {
          xc[2] = xi1;
          yc[2] = (z1 != z3 ? CGnuPlotUtil::map(level, z1, z3, yj1, yj2) : yj1);
          zc[2] = level;
        }

        // get point on right side for specified value (if in range)
        if ((flag[3] = ((level >= z2 && level <= z4) || (level <= z2 && level >= z4)))) {
          xc[3] = xi2;
          yc[3] = (z2 != z4 ? CGnuPlotUtil::map(level, z2, z4, yj1, yj2) : yj1);
          zc[3] = level;
        }

        //---

        // get point on left half of diagonal up line
        if ((flag[4] = ((level >= z1 && level <= zm) || (level <= z1 && level >= zm)))) {
          xc[4] = (z1 != zm ? CGnuPlotUtil::map(level, z1, zm, xi1, xm) : xi1);
          yc[4] = (z1 != zm ? CGnuPlotUtil::map(level, z1, zm, yj1, ym) : yj1);
          zc[4] = level;
        }

        // get point on right half of diagonal down line
        if ((flag[5] = ((level >= z2 && level <= zm) || (level <= z2 && level >= zm)))) {
          xc[5] = (z2 != zm ? CGnuPlotUtil::map(level, z2, zm, xi2, xm) : xi2);
          yc[5] = (z2 != zm ? CGnuPlotUtil::map(level, z2, zm, yj1, ym) : yj1);
          zc[5] = level;
        }

        // get point on left half of diagonal down line
        if ((flag[6] = ((level >= z3 && level <= zm) || (level <= z3 && level >= zm)))) {
          xc[6] = (z3 != zm ? CGnuPlotUtil::map(level, z3, zm, xi1, xm) : xi1);
          yc[6] = (z3 != zm ? CGnuPlotUtil::map(level, z3, zm, yj2, ym) : yj2);
          zc[6] = level;
        }

        // get point on right half of diagonal up line
        if ((flag[7] = ((level >= z4 && level <= zm) || (level <= z4 && level >= zm)))) {
          xc[7] = (z4 != zm ? CGnuPlotUtil::map(level, z4, zm, xi2, xm) : xi2);
          yc[7] = (z4 != zm ? CGnuPlotUtil::map(level, z4, zm, yj2, ym) : yj2);
          zc[7] = level;
        }

        //---

        // connect lines
        for (int k = 0; k < 12; ++k) {
          auto f1 = contour_flags[2*k + 0];
          auto f2 = contour_flags[2*k + 1];

          if (flag[f1] && flag[f2]) {
            if (pos_ == CGnuPlotContourData::DrawPos::BASE ||
                pos_ == CGnuPlotContourData::DrawPos::BOTH) {
              CPoint3D p1(xc[f1], yc[f1], min_z_);
              CPoint3D p2(xc[f2], yc[f2], min_z_);

              llines_[l].push_back(CLine3D(p1, p2));
            }

            if (pos_ == CGnuPlotContourData::DrawPos::SURFACE ||
                pos_ == CGnuPlotContourData::DrawPos::BOTH) {
              CPoint3D p1(xc[f1], yc[f1], zc[f1]);
              CPoint3D p2(xc[f2], yc[f2], zc[f2]);

              llines_[l].push_back(CLine3D(p1, p2));
            }
          }
        }
      }
    }

    // sort lines
    Lines &lines = llines_[l];

    int nl = lines.size();

    for (int i = 0; i < nl - 1; ++i) {
      // find best start matching this line's end
      auto &line1  = lines[i];
      auto &point1 = line1.end();

      int    min_j = -1;
      double min_d = 1E50;
      bool   flip  = false;

      for (int j = i + 1; j < nl; ++j) {
        auto &line2  = lines[j];
        auto &point2 = line2.start();
        auto &point3 = line2.end  ();

        auto d1 = fabs(point1.x - point2.x) + fabs(point1.y - point2.y);
        auto d2 = fabs(point1.x - point3.x) + fabs(point1.y - point3.y);

        if (min_j < 0 || d1 < min_d) {
          min_j = j;
          min_d = d1;
          flip  = false;
        }

        if (min_j < 0 || d2 < min_d) {
          min_j = j;
          min_d = d2;
          flip  = true;
        }
      }

      std::swap(lines[i + 1], lines[min_j]);

      if (flip)
        lines[i + 1].flip();
    }
  }
}

void
CGnuPlotContour::
drawContourSolid(CGnuPlotRenderer *renderer)
{
  std::vector<double> levels;

  getLevels(levels);

  //---

  min_x_ = 0.1;
  min_y_ = 0.1;

  for (uint i = 0; i < x_.size() - 1; ++i) {
    auto x1 = x_[i + 0], x2 = x_[i + 1];

    auto i1 =  i     *y_.size();
    auto i2 = (i + 1)*y_.size();

    for (uint j = 0; j < y_.size() - 1; ++j) {
      auto y1 = y_[j + 0], y2 = y_[j + 1];

      double z1, z2, z3, z4;

      z1 = z_[i1 + j    ], z2 = z_[i2 + j    ];
      z3 = z_[i1 + j + 1], z4 = z_[i2 + j + 1];

#if 0
      if (! c_.empty()) {
        z1 = c_[i1 + j    ], z2 = c_[i2 + j    ];
        z3 = c_[i1 + j + 1], z4 = c_[i2 + j + 1];
      }
#endif

      fillContourBox(renderer, x1, y1, x2, y2, z1, z2, z3, z4, levels);
    }
  }
}

void
CGnuPlotContour::
getLevelData(std::vector<LevelData> &levelDatas) const
{
  std::vector<double> levels;

  getLevels(levels);

  for (uint l = 0; l < levels.size(); ++l) {
    auto level = levels[l];

    auto c = levelColor(l);

    levelDatas.push_back(LevelData(level, c));
  }
}

void
CGnuPlotContour::
getLevels(std::vector<double> &levels) const
{
  initLevels();

  levels = levels_;
}

void
CGnuPlotContour::
initLevels() const
{
  const CGnuPlotContourData &contourData = plot_->contourData();

  if (levels_.empty()) {
    auto *th = const_cast<CGnuPlotContour *>(this);

    COptReal z1, z2;

    for (uint i = 0; i < x_.size(); ++i) {
      for (uint j = 0; j < y_.size(); ++j) {
        auto zm = z_[i*y_.size() + j];

#if 0
        if (! c_.empty())
          zm = c_[i*y_.size() + j];
#endif

        z1.updateMin(zm);
        z2.updateMax(zm);
      }
    }

    th->levels_ = contourData.getLevelValues(z1.getValue(0.0), z2.getValue(0.0));
  }
}

double
CGnuPlotContour::
levelValue(int l) const
{
  initLevels();

  return levels_[l];
}

void
CGnuPlotContour::
fillContourBox(CGnuPlotRenderer *renderer, double x1, double y1, double x2, double y2,
               double z1, double z2, double z3, double z4, const std::vector<double> &levels)
{
  // get box corner values
  if      (fabs(x2 - x1) <= min_x_ && fabs(y2 - y1) <= min_y_) {
    auto z1234 = CGnuPlotUtil::avg({z1, z2, z3, z4});

    z1 = z1234; z2 = z1234; z3 = z1234; z4 = z1234;
  }
  else if (fabs(x2 - x1) <= min_x_) {
    auto z12 = CGnuPlotUtil::avg({z1, z2}), z34 = CGnuPlotUtil::avg({z3, z4});

    z1 = z12; z2 = z12; z3 = z34; z4 = z34;
  }
  else if (fabs(y2 - y1) <= min_y_) {
    auto z13 = CGnuPlotUtil::avg({z1, z3}), z24 = CGnuPlotUtil::avg({z2, z4});

    z1 = z13; z2 = z13; z3 = z24; z4 = z24;
  }

  //---

#if 1
  // get consistent level for four values
  int l = 0;

  if      (z1 < levels[0] && z2 < levels[0] && z3 < levels[0] && z4 < levels[0])
    l = 0;
  else if (z1 > levels[levels.size() - 1] && z2 > levels[levels.size() - 1] &&
           z3 > levels[levels.size() - 1] && z4 > levels[levels.size() - 1])
    l = levels.size();
  else {
    for (l = 1; l < int(levels.size()); ++l) {
      if (z1 >= levels[l - 1] && z2 >= levels[l - 1] &&
          z3 >= levels[l - 1] && z4 >= levels[l - 1] &&
          z1 <= levels[l    ] && z2 <= levels[l    ] &&
          z3 <= levels[l    ] && z4 <= levels[l    ])
        break;

      if (z1 >= levels[l - 1] && z3 >= levels[l - 1] &&
          z2 >= levels[l - 1] && z4 >= levels[l - 1] &&
          z1 <= levels[l    ] && z3 <= levels[l    ] &&
          z2 <= levels[l    ] && z4 <= levels[l    ])
        break;
    }

    if (l >= int(levels.size()))
      l = -1;
  }

  //---

  // if consistent level then fill
  if (l >= 0 && l <= int(levels.size())) {
    auto c = levelColor(l);

    std::vector<CPoint3D> points;

    if (pos_ == CGnuPlotContourData::DrawPos::BASE ||
        pos_ == CGnuPlotContourData::DrawPos::BOTH) {
      points.push_back(CPoint3D(x1, y1, min_z_));
      points.push_back(CPoint3D(x2, y1, min_z_));
      points.push_back(CPoint3D(x2, y2, min_z_));
      points.push_back(CPoint3D(x1, y2, min_z_));

      renderer->fillPolygon(points, c);
    }

    if (pos_ == CGnuPlotContourData::DrawPos::SURFACE ||
        pos_ == CGnuPlotContourData::DrawPos::BOTH) {
      points.push_back(CPoint3D(x1, y1, z1));
      points.push_back(CPoint3D(x2, y1, z2));
      points.push_back(CPoint3D(x2, y2, z4));
      points.push_back(CPoint3D(x1, y2, z3));

      renderer->fillPolygon(points, c);
    }

    return;
  }
#else
  auto zm = CGnuPlotUtil::avg({z1, z2, z3, z4});

  double l;

  if (! c_.empty())
    l = CGnuPlotUtil::map(zm, zmin_.getValue(zm), zmax_.getValue(zm), 0, 1);
  else
    l = CGnuPlotUtil::map(zm, cmin_.getValue(zm), cmax_.getValue(zm), 0, 1);

  CGnuPlotColorBoxP cb = plot_->group()->colorBox();

  auto c = cb->valueToColor(l).rgba();

  std::vector<CPoint3D> points;

  if (pos_ == CGnuPlotContourData::DrawPos::BASE ||
      pos_ == CGnuPlotContourData::DrawPos::BOTH) {
    points.push_back(CPoint3D(x1, y1, min_z_));
    points.push_back(CPoint3D(x2, y1, min_z_));
    points.push_back(CPoint3D(x2, y2, min_z_));
    points.push_back(CPoint3D(x1, y2, min_z_));

    renderer->fillPolygon(points, c);
  }

  if (pos_ == CGnuPlotContourData::DrawPos::SURFACE ||
      pos_ == CGnuPlotContourData::DrawPos::BOTH) {
    points.push_back(CPoint3D(x1, y1, z1));
    points.push_back(CPoint3D(x2, y1, z2));
    points.push_back(CPoint3D(x2, y2, z4));
    points.push_back(CPoint3D(x1, y2, z3));

    renderer->fillPolygon(points, c);
  }
#endif

  //---

  // sub-divide and draw
  // TODO: iterate instead of recurse
  auto x12 = CGnuPlotUtil::avg({x1, x2});
  auto y12 = CGnuPlotUtil::avg({y1, y2});
  auto z12 = CGnuPlotUtil::avg({z1, z2});
  auto z24 = CGnuPlotUtil::avg({z2, z4});
  auto z13 = CGnuPlotUtil::avg({z1, z3});
  auto z34 = CGnuPlotUtil::avg({z3, z4});

  auto z1234 = CGnuPlotUtil::avg({z1, z2, z3, z4});

  fillContourBox(renderer, x1 , y1 , x12, y12, z1, z12, z13, z1234, levels);
  fillContourBox(renderer, x12, y1 , x2 , y12, z12, z2, z1234, z24, levels);
  fillContourBox(renderer, x1 , y12, x12, y2 , z13, z1234, z3, z34, levels);
  fillContourBox(renderer, x12, y12, x2 , y2 , z1234, z24, z34, z4, levels);
}

CRGBA
CGnuPlotContour::
levelColor(int level) const
{
  return CGnuPlotStyleInst->indexColor(palette(), level);
}

void
CGnuPlotContour::
getPaths(const Lines &lines, Paths &paths)
{
  Points points;

  for (const auto &line : lines) {
    if (! points.empty() && ! CPoint3D::isEqual(points.back(), line.start(), 1E-3)) {
      paths.push_back(points);

      points.clear();
    }

    if (points.empty())
      points.push_back(line.start());

    points.push_back(line.end());
  }

  if (! points.empty())
    paths.push_back(points);
}
