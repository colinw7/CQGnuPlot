#ifndef CGnuPlotContour_H
#define CGnuPlotContour_H

#include <CRGBA.h>
#include <COptVal.h>
#include <CLine3D.h>
#include <CAlignType.h>
#include <vector>
#include <map>

#include <CGnuPlotContourData.h>

class CGnuPlot;
class CGnuPlotPlot;
class CGnuPlotRenderer;

class CGnuPlotContour {
 public:
  typedef std::vector<double>           Reals;
  typedef std::vector<CLine3D>          Lines;
  typedef std::map<int,Lines>           LevelLines;
  typedef std::vector<CPoint3D>         Points;
  typedef std::vector<Points>           Paths;
  typedef CGnuPlotContourData::DrawPos  DrawPos;
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

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

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

  void reset();

  void setData(double *x, double *y, double *z, double *c, int no_x, int no_y, bool has_c);

  int nx() const { return x_.size(); }
  int ny() const { return y_.size(); }

  double x(int i) const { return x_[i]; }
  double y(int j) const { return y_[j]; }

  double z(int i, int j) const { return z_[j*x_.size() + i]; }
  double c(int i, int j) const { return c_[j*x_.size() + i]; }

  void setContourLevels(const std::vector<double> &levels);

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
  CGnuPlotPlot* plot_  { 0 };
  bool          solid_ { false };
  Reals         x_, y_, z_, c_;
  Reals         levels_;
  COptReal      xmin_, ymin_, zmin_, cmin_;
  COptReal      xmax_, ymax_, zmax_, cmax_;
  double        min_x_ { 0 }, min_y_ { 0 }, min_z_ { 0 };
  DrawPos       pos_ { DrawPos::BASE };
  LevelLines    llines_;
  std::string   palette_ { "subtle" };
};

#endif
