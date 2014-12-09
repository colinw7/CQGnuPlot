#ifndef CContour_H
#define CContour_H

#include <vector>
#include <CRGBA.h>

class CGnuPlotWindow;

class CGnuPlotContour {
 public:
  CGnuPlotContour(CGnuPlotWindow *window);

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
  CGnuPlotWindow*     window_;
  bool                solid_;
  std::vector<double> x_, y_, z_;
  std::vector<double> levels_;
  std::vector<CRGBA>  colors_;
  double              xmin_, ymin_, zmin_;
  double              xmax_, ymax_, zmax_;
  int                 w_, h_;
  double              min_x_, min_y_;
  double              min_z_;
};

#endif
