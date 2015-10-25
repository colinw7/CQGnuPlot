#include <CGnuPlotContour.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlot.h>
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

static std::vector<CRGBA> contour_colors;

void initContourColors() {
  if (contour_colors.empty()) {
    contour_colors.push_back(CRGBName::toRGBA("#3182BD"));
    contour_colors.push_back(CRGBName::toRGBA("#6BAED6"));
    contour_colors.push_back(CRGBName::toRGBA("#9ECAE1"));
    contour_colors.push_back(CRGBName::toRGBA("#C6DBEF"));
    contour_colors.push_back(CRGBName::toRGBA("#E6550D"));
    contour_colors.push_back(CRGBName::toRGBA("#FD8D3C"));
    contour_colors.push_back(CRGBName::toRGBA("#FDAE6B"));
    contour_colors.push_back(CRGBName::toRGBA("#FDD0A2"));
    contour_colors.push_back(CRGBName::toRGBA("#31A354"));
    contour_colors.push_back(CRGBName::toRGBA("#74C476"));
    contour_colors.push_back(CRGBName::toRGBA("#A1D99B"));
    contour_colors.push_back(CRGBName::toRGBA("#C7E9C0"));
    contour_colors.push_back(CRGBName::toRGBA("#756BB1"));
    contour_colors.push_back(CRGBName::toRGBA("#9E9AC8"));
    contour_colors.push_back(CRGBName::toRGBA("#BCBDDC"));
    contour_colors.push_back(CRGBName::toRGBA("#DADAEB"));
    contour_colors.push_back(CRGBName::toRGBA("#636363"));
    contour_colors.push_back(CRGBName::toRGBA("#969696"));
    contour_colors.push_back(CRGBName::toRGBA("#BDBDBD"));
    contour_colors.push_back(CRGBName::toRGBA("#D9D9D9"));
  }
}

//---

CGnuPlotContour::
CGnuPlotContour(CGnuPlotPlot *plot) :
 plot_(plot), solid_(false), min_z_(-1.0)
{
  initContourColors();

  min_x_ = 1E-6;
  min_y_ = 1E-6;

  colors_ = std::vector<CRGBA>(&contour_colors[0], &contour_colors[20]);
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
setContourColors(const std::vector<CRGBA> &colors)
{
  colors_ = colors;
}

void
CGnuPlotContour::
drawContour(CGnuPlotRenderer *renderer)
{
  CGnuPlotAxisData &zaxis = plot_->group()->zaxis(plot_->zind());

  min_z_ = zaxis.min().getValue();

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
  const CGnuPlotLineStyle &lineStyle = plot_->lineStyle();

  CGnuPlotContourData::DrawPos pos = plot_->contourData().pos();

  // draw contour points
  for (uint j = 0; j < y_.size() - 1; ++j) {
    double y = y_[j];

    int j1 = j*x_.size();

    for (uint i = 0; i < x_.size() - 1; ++i) {
      double x = x_[i];
      double z = z_[j1 + i];

      if (pos == CGnuPlotContourData::DrawPos::BASE ||
          pos == CGnuPlotContourData::DrawPos::BOTH) {
        renderer->drawPoint(CPoint3D(x, y, min_z_), CRGBA(0,0,0));
      }
      if (pos == CGnuPlotContourData::DrawPos::SURFACE ||
          pos == CGnuPlotContourData::DrawPos::BOTH) {
        renderer->drawPoint(CPoint3D(x, y, z), CRGBA(0,0,0));
      }
    }
  }

  //---

  double width = lineStyle.calcWidth();

  for (const auto &llines : llines_) {
    int          l     = llines.first;
    const Lines &lines = llines.second;

    CRGBA c = levelColor(l);

    for (const auto &line : lines) {
      renderer->drawLine(line.start(), line.end(), width, c);
    }
  }
}

void
CGnuPlotContour::
calcContourLines()
{
  llines_.clear();

  CGnuPlotContourData::DrawPos pos = plot_->contourData().pos();

  //---

  std::vector<double> levels;

  getLevels(levels);

  //---

  int    flag[8];
  double xc[8], yc[8], zc[8];

  for (uint l = 0; l < levels.size(); ++l) {
    double level = levels[l];

    if (y_.size() < 1) continue;

    for (uint j = 0; j < y_.size() - 1; ++j) {
      double yj1 = y_[j + 0], yj2 = y_[j + 1];

      double ym = CGnuPlotUtil::avg({yj1, yj2});

      int j1 =  j     *x_.size();
      int j2 = (j + 1)*x_.size();

      if (x_.size() < 1) continue;

      for (uint i = 0; i < x_.size() - 1; ++i) {
        double xi1 = x_[i + 0], xi2 = x_[i + 1];

        double xm = CGnuPlotUtil::avg({xi1, xi2});

        //---

        double z1, z2, z3, z4, zm;

        if (! c_.empty()) {
          z1 = z_[j1 + i], z2 = z_[j1 + i + 1];
          z3 = z_[j2 + i], z4 = z_[j2 + i + 1];

          zm = CGnuPlotUtil::avg({z1, z2, z3, z4});
        }
        else {
          z1 = c_[j1 + i], z2 = c_[j1 + i + 1];
          z3 = c_[j2 + i], z4 = c_[j2 + i + 1];

          zm = CGnuPlotUtil::avg({z1, z2, z3, z4});
        }

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
          int f1 = contour_flags[2*k + 0];
          int f2 = contour_flags[2*k + 1];

          if (flag[f1] && flag[f2]) {
            if (pos == CGnuPlotContourData::DrawPos::BASE ||
                pos == CGnuPlotContourData::DrawPos::BOTH) {
              CPoint3D p1(xc[f1], yc[f1], min_z_);
              CPoint3D p2(xc[f2], yc[f2], min_z_);

              llines_[l].push_back(CLine3D(p1, p2));
            }

            if (pos == CGnuPlotContourData::DrawPos::SURFACE ||
                pos == CGnuPlotContourData::DrawPos::BOTH) {
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
      CLine3D  &line1  = lines[i];
      CPoint3D &point1 = line1.end();

      int    min_j = -1;
      double min_d = 1E50;
      bool   flip  = false;

      for (int j = i + 1; j < nl; ++j) {
        CLine3D  &line2  = lines[j];
        CPoint3D &point2 = line2.start();
        CPoint3D &point3 = line2.end  ();

        double d1 = fabs(point1.x - point2.x) + fabs(point1.y - point2.y);
        double d2 = fabs(point1.x - point3.x) + fabs(point1.y - point3.y);

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
    double x1 = x_[i + 0], x2 = x_[i + 1];

    int i1 =  i     *y_.size();
    int i2 = (i + 1)*y_.size();

    for (uint j = 0; j < y_.size() - 1; ++j) {
      double y1 = y_[j + 0], y2 = y_[j + 1];

      double z1, z2, z3, z4;

      if (! c_.empty()) {
        z1 = z_[i1 + j    ], z2 = z_[i2 + j    ];
        z3 = z_[i1 + j + 1], z4 = z_[i2 + j + 1];
      }
      else {
        z1 = c_[i1 + j    ], z2 = c_[i2 + j    ];
        z3 = c_[i1 + j + 1], z4 = c_[i2 + j + 1];
      }

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
    double level = levels[l];

    CRGBA c = levelColor(l);

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
  if (levels_.empty()) {
    CGnuPlotContour *th = const_cast<CGnuPlotContour *>(this);

    COptReal z1, z2;

    for (uint i = 0; i < x_.size(); ++i) {
      for (uint j = 0; j < y_.size(); ++j) {
        double zm;

        if (! c_.empty())
          zm = z_[i*y_.size() + j];
        else
          zm = c_[i*y_.size() + j];

        z1.updateMin(zm);
        z2.updateMax(zm);
      }
    }

    th->levels_ = plot_->contourData().getLevelValues(z1.getValue(0.0), z2.getValue(0.0));
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
  CGnuPlotContourData::DrawPos pos = plot_->contourData().pos();

  // get box corner values
  if      (fabs(x2 - x1) <= min_x_ && fabs(y2 - y1) <= min_y_) {
    double z1234 = CGnuPlotUtil::avg({z1, z2, z3, z4});

    z1 = z1234; z2 = z1234; z3 = z1234; z4 = z1234;
  }
  else if (fabs(x2 - x1) <= min_x_) {
    double z12 = CGnuPlotUtil::avg({z1, z2}), z34 = CGnuPlotUtil::avg({z3, z4});

    z1 = z12; z2 = z12; z3 = z34; z4 = z34;
  }
  else if (fabs(y2 - y1) <= min_y_) {
    double z13 = CGnuPlotUtil::avg({z1, z3}), z24 = CGnuPlotUtil::avg({z2, z4});

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
    CRGBA c = levelColor(l);

    std::vector<CPoint3D> points;

    if (pos == CGnuPlotContourData::DrawPos::BASE ||
        pos == CGnuPlotContourData::DrawPos::BOTH) {
      points.push_back(CPoint3D(x1, y1, min_z_));
      points.push_back(CPoint3D(x2, y1, min_z_));
      points.push_back(CPoint3D(x2, y2, min_z_));
      points.push_back(CPoint3D(x1, y2, min_z_));

      renderer->fillPolygon(points, c);
    }

    if (pos == CGnuPlotContourData::DrawPos::SURFACE ||
        pos == CGnuPlotContourData::DrawPos::BOTH) {
      points.push_back(CPoint3D(x1, y1, z1));
      points.push_back(CPoint3D(x2, y1, z2));
      points.push_back(CPoint3D(x2, y2, z4));
      points.push_back(CPoint3D(x1, y2, z3));

      renderer->fillPolygon(points, c);
    }

    return;
  }
#else
  double zm = CGnuPlotUtil::avg({z1, z2, z3, z4});

  double l;

  if (! c_.empty())
    l = CGnuPlotUtil::map(zm, zmin_.getValue(zm), zmax_.getValue(zm), 0, 1);
  else
    l = CGnuPlotUtil::map(zm, cmin_.getValue(zm), cmax_.getValue(zm), 0, 1);

  CGnuPlotColorBoxP cb = plot_->group()->colorBox();

  CRGBA c = cb->valueToColor(l).rgba();

  std::vector<CPoint3D> points;

  if (pos == CGnuPlotContourData::DrawPos::BASE ||
      pos == CGnuPlotContourData::DrawPos::BOTH) {
    points.push_back(CPoint3D(x1, y1, min_z_));
    points.push_back(CPoint3D(x2, y1, min_z_));
    points.push_back(CPoint3D(x2, y2, min_z_));
    points.push_back(CPoint3D(x1, y2, min_z_));

    renderer->fillPolygon(points, c);
  }

  if (pos == CGnuPlotContourData::DrawPos::SURFACE ||
      pos == CGnuPlotContourData::DrawPos::BOTH) {
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
  double x12 = CGnuPlotUtil::avg({x1, x2});
  double y12 = CGnuPlotUtil::avg({y1, y2});
  double z12 = CGnuPlotUtil::avg({z1, z2});
  double z24 = CGnuPlotUtil::avg({z2, z4});
  double z13 = CGnuPlotUtil::avg({z1, z3});
  double z34 = CGnuPlotUtil::avg({z3, z4});

  double z1234 = CGnuPlotUtil::avg({z1, z2, z3, z4});

  fillContourBox(renderer, x1 , y1 , x12, y12, z1, z12, z13, z1234, levels);
  fillContourBox(renderer, x12, y1 , x2 , y12, z12, z2, z1234, z24, levels);
  fillContourBox(renderer, x1 , y12, x12, y2 , z13, z1234, z3, z34, levels);
  fillContourBox(renderer, x12, y12, x2 , y2 , z1234, z24, z34, z4, levels);
}

CRGBA
CGnuPlotContour::
levelColor(int level) const
{
  CRGBA c(0,0,0);

  if (! colors_.empty())
    c = colors_[level % colors_.size()];

  return c;
}
