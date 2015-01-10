#ifndef CGnuPlotContour_H
#define CGnuPlotContour_H

#include <vector>
#include <CRGBA.h>
#include <COptVal.h>

class CGnuPlot;
class CGnuPlotPlot;

class CGnuPlotContour {
 public:
  CGnuPlotContour(CGnuPlotPlot *plot);

  CGnuPlot *app() const;

  CGnuPlotPlot *plot() const { return plot_; }

  bool solid() const { return solid_; }
  void setSolid(bool b) { solid_ = b; }

  void setData(double *x, double *y, double *z, int no_x, int no_y);

  void setContourLevels(const std::vector<double> &levels);
  void setContourColours(const std::vector<CRGBA> &colors);

  void drawContour();

 private:
  void drawContourLines();
  void drawContourSolid();

  void initLevels(std::vector<double> &levels) const;

  void fillContourBox(double, double, double, double,
                      double, double, double, double, const std::vector<double> &);

 private:
  CGnuPlotPlot*       plot_;
  bool                solid_;
  std::vector<double> x_, y_, z_;
  std::vector<double> levels_;
  std::vector<CRGBA>  colors_;
  COptReal            xmin_, ymin_, zmin_;
  COptReal            xmax_, ymax_, zmax_;
  double              min_x_, min_y_, min_z_;
};

#endif
