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
  1, 6, // top edge to diagonals
  1, 7,
  2, 4, // left edge to diagonals
  2, 6,
  3, 5, // right edge to diagonals
  3, 7,

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
setData(double *x, double *y, double *z, int no_x, int no_y)
{
  x_ = std::vector<double>(&x[0], &x[no_x]);
  y_ = std::vector<double>(&y[0], &y[no_y]);
  z_ = std::vector<double>(&z[0], &z[no_x*no_y]);

  xmin_.setInvalid(); ymin_.setInvalid(); zmin_.setInvalid();
  xmax_.setInvalid(); ymax_.setInvalid(); zmax_.setInvalid();

  for (auto x : x_) { xmin_.updateMin(x); xmax_.updateMax(x); }
  for (auto y : y_) { ymin_.updateMin(y); ymax_.updateMax(y); }
  for (auto z : z_) { zmin_.updateMin(z); zmax_.updateMax(z); }
}

void
CGnuPlotContour::
setContourLevels(const std::vector<double> &levels)
{
  levels_ = levels;
}

void
CGnuPlotContour::
setContourColours(const std::vector<CRGBA> &colors)
{
  colors_ = colors;
}

void
CGnuPlotContour::
drawContour()
{
  if (solid())
    drawContourSolid();
  else
    drawContourLines();
}

void
CGnuPlotContour::
drawContourLines()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CGnuPlotCamera *camera = plot_->group()->camera();

  // draw contour points
  for (auto y : y_) {
    for (auto x : x_) {
      CPoint3D p = camera->transform(CPoint3D(x, y, min_z_));

      renderer->drawPoint(CPoint2D(p.x, p.y), CRGBA(0,0,0));
    }
  }

  //---

  std::vector<double> levels;

  initLevels(levels);

  //---

  double xc[8];
  double yc[8];
  int    flag[8];

  for (uint l = 0; l < levels.size(); ++l) {
    double level = levels[l];

    CRGBA c(0,0,0);

    if (! colors_.empty())
      c = colors_[l % colors_.size()];

    for (uint i = 0; i < x_.size() - 1; ++i) {
      double xi1 = x_[i + 0], xi2 = x_[i + 1];

      double xm = CGnuPlotUtil::avg({xi1, xi2});

      int i1 =  i     *y_.size();
      int i2 = (i + 1)*y_.size();

      for (uint j = 0; j < y_.size() - 1; ++j) {
        double yj1 = y_[j + 0], yj2 = y_[j + 1];

        double ym = CGnuPlotUtil::avg({yj1, yj2});

        double z1 = z_[i1 + j    ], z2 = z_[i2 + j    ];
        double z3 = z_[i1 + j + 1], z4 = z_[i2 + j + 1];

        double zm = CGnuPlotUtil::avg({z1, z2, z3, z4});

        //---

        // get point on lower side for specified value (if in range)
        if ((flag[0] = ((level >= z1 && level <= z2) || (level <= z1 && level >= z2)))) {
          xc[0] = (z1 != z2 ? CGnuPlotUtil::map(level, z1, z2, xi1, xi2) : xi1);
          yc[0] = yj1;
        }

        // get point on upper side for specified value (if in range)
        if ((flag[1] = ((level >= z3 && level <= z4) || (level <= z3 && level >= z4)))) {
          xc[1] = (z3 != z4 ? CGnuPlotUtil::map(level, z3, z4, xi1, xi2) : xi1);
          yc[1] = yj2;
        }

        // get point on left side for specified value (if in range)
        if ((flag[2] = ((level >= z1 && level <= z3) || (level <= z1 && level >= z3)))) {
          xc[2] = xi1;
          yc[2] = (z1 != z3 ? CGnuPlotUtil::map(level, z1, z3, yj1, yj2) : yj1);
        }

        // get point on right side for specified value (if in range)
        if ((flag[3] = ((level >= z2 && level <= z4) || (level <= z2 && level >= z4)))) {
          xc[3] = xi2;
          yc[3] = (z2 != z4 ? CGnuPlotUtil::map(level, z2, z4, yj1, yj2) : yj1);
        }

        //---

        // get point on left half of diagonal up line
        if ((flag[4] = ((level >= z1 && level <= zm) || (level <= z1 && level >= zm)))) {
          xc[4] = (z1 != zm ? CGnuPlotUtil::map(level, z1, zm, xi1, xm) : xi1);
          yc[4] = (z1 != zm ? CGnuPlotUtil::map(level, z1, zm, yj1, ym) : yj1);
        }

        // get point on right half of diagonal down line
        if ((flag[5] = ((level >= z2 && level <= zm) || (level <= z2 && level >= zm)))) {
          xc[5] = (z2 != zm ? CGnuPlotUtil::map(level, z2, zm, xi2, xm) : xi2);
          yc[5] = (z2 != zm ? CGnuPlotUtil::map(level, z2, zm, yj1, ym) : yj1);
        }

        // get point on left half of diagonal down line
        if ((flag[6] = ((level >= z3 && level <= zm) || (level <= z3 && level >= zm)))) {
          xc[6] = (z3 != zm ? CGnuPlotUtil::map(level, z3, zm, xi1, xm) : xi1);
          yc[6] = (z3 != zm ? CGnuPlotUtil::map(level, z3, zm, yj2, ym) : yj2);
        }

        // get point on right half of diagonal up line
        if ((flag[7] = ((level >= z4 && level <= zm) || (level <= z4 && level >= zm)))) {
          xc[7] = (z4 != zm ? CGnuPlotUtil::map(level, z4, zm, xi2, xm) : xi2);
          yc[7] = (z4 != zm ? CGnuPlotUtil::map(level, z4, zm, yj2, ym) : yj2);
        }

        //---

        // connect lines
        for (int k = 0; k < 12; ++k) {
          int f1 = contour_flags[2*k + 0];
          int f2 = contour_flags[2*k + 1];

          if (flag[f1] && flag[f2]) {
            CPoint3D p1 = camera->transform(CPoint3D(xc[f1], yc[f1], min_z_));
            CPoint3D p2 = camera->transform(CPoint3D(xc[f2], yc[f2], min_z_));

            renderer->drawLine(CPoint2D(p1.x, p1.y), CPoint2D(p2.x, p2.y), 0.0, c);
          }
        }
      }
    }
  }
}

void
CGnuPlotContour::
drawContourSolid()
{
  std::vector<double> levels;

  initLevels(levels);

  //---

  min_x_ = 0.1;
  min_y_ = 0.1;

  for (uint i = 0; i < x_.size() - 1; ++i) {
    double x1 = x_[i + 0], x2 = x_[i + 1];

    int i1 =  i     *y_.size();
    int i2 = (i + 1)*y_.size();

    for (uint j = 0; j < y_.size() - 1; ++j) {
      double y1 = y_[j + 0], y2 = y_[j + 1];

      double z1 = z_[i1 + j    ], z2 = z_[i2 + j    ];
      double z3 = z_[i1 + j + 1], z4 = z_[i2 + j + 1];

      fillContourBox(x1, y1, x2, y2, z1, z2, z3, z4, levels);
    }
  }
}

void
CGnuPlotContour::
initLevels(std::vector<double> &levels) const
{
  levels = levels_;

  if (levels.empty()) {
    COptReal z1, z2;

    for (uint i = 0; i < x_.size(); ++i) {
      for (uint j = 0; j < y_.size(); ++j) {
        double zm = z_[i*y_.size() + j];

        z1.updateMin(zm);
        z2.updateMax(zm);
      }
    }

    int no_levels = 10;

    levels.resize(no_levels);

    for (int i = 0; i < no_levels; ++i)
      levels[i] = CGnuPlotUtil::map(i, 0, no_levels - 1, z1.getValue(), z2.getValue());
  }
}

void
CGnuPlotContour::
fillContourBox(double x1, double y1, double x2, double y2,
               double z1, double z2, double z3, double z4, const std::vector<double> &levels)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CGnuPlotCamera *camera = plot_->group()->camera();

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
    CRGBA c = colors_[l % colors_.size()];

    std::vector<CPoint2D> points;

    CPoint3D p1 = camera->transform(CPoint3D(x1, y1, min_z_));
    CPoint3D p2 = camera->transform(CPoint3D(x2, y1, min_z_));
    CPoint3D p3 = camera->transform(CPoint3D(x2, y2, min_z_));
    CPoint3D p4 = camera->transform(CPoint3D(x1, y2, min_z_));

    points.push_back(CPoint2D(p1.x, p1.y));
    points.push_back(CPoint2D(p2.x, p2.y));
    points.push_back(CPoint2D(p3.x, p3.y));
    points.push_back(CPoint2D(p4.x, p4.y));

    renderer->fillPolygon(points, c);

    return;
  }

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

  fillContourBox(x1 , y1 , x12, y12, z1, z12, z13, z1234, levels);
  fillContourBox(x12, y1 , x2 , y12, z12, z2, z1234, z24, levels);
  fillContourBox(x1 , y12, x12, y2 , z13, z1234, z3, z34, levels);
  fillContourBox(x12, y12, x2 , y2 , z1234, z24, z34, z4, levels);
}
