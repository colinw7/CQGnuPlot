#ifndef CGnuPlotContour_H
#define CGnuPlotContour_H

#include <CRGBA.h>
#include <COptVal.h>
#include <CLine3D.h>
#include <vector>
#include <map>

class CGnuPlot;
class CGnuPlotPlot;
class CGnuPlotRenderer;

class CGnuPlotContour {
 public:
  typedef std::vector<CLine3D>  Lines;
  typedef std::map<int,Lines>   LevelLines;
  typedef std::vector<CPoint3D> Points;
  typedef std::vector<Points>   Paths;

  struct LevelData {
    double level;
    CRGBA  c;

    LevelData(double level1, const CRGBA &c1) :
     level(level1), c(c1) {
    }
  };

 public:
  CGnuPlotContour(CGnuPlotPlot *plot);

  CGnuPlot *app() const;

  CGnuPlotPlot *plot() const { return plot_; }

  bool solid() const { return solid_; }
  void setSolid(bool b) { solid_ = b; }

  void reset();

  void setData(double *x, double *y, double *z, double *c, int no_x, int no_y, bool has_c);

  int nx() const { return x_.size(); }
  int ny() const { return y_.size(); }

  double x(int i) const { return x_[i]; }
  double y(int j) const { return y_[j]; }

  double z(int i, int j) const { return z_[j*x_.size() + i]; }
  double c(int i, int j) const { return c_[j*x_.size() + i]; }

  void setContourLevels(const std::vector<double> &levels);
  void setContourColors(const std::vector<CRGBA> &colors);

  void drawContour(CGnuPlotRenderer *renderer);

  void getLevelData(std::vector<LevelData> &levelDatas) const;

  const LevelLines &levelLines() const { return llines_; }

  CRGBA levelColor(int level) const;

  double levelValue(int l) const;

 private:
  void drawContourLines(CGnuPlotRenderer *renderer);
  void drawContourSolid(CGnuPlotRenderer *renderer);

  void calcContourLines();

  void getLevels(std::vector<double> &levels) const;

  void initLevels() const;

  void fillContourBox(CGnuPlotRenderer *renderer, double, double, double, double,
                      double, double, double, double, const std::vector<double> &);

  static void getPaths(const Lines &lines, Paths &path);

 private:
  CGnuPlotPlot*       plot_;
  bool                solid_;
  std::vector<double> x_, y_, z_, c_;
  std::vector<double> levels_;
  std::vector<CRGBA>  colors_;
  COptReal            xmin_, ymin_, zmin_, cmin_;
  COptReal            xmax_, ymax_, zmax_, cmax_;
  double              min_x_, min_y_, min_z_;
  LevelLines          llines_;
};

#endif
