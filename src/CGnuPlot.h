#ifndef CGnuPlot_H
#define CGnuPlot_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <climits>
#include <sys/types.h>

#include <CFont.h>
#include <CAutoPtr.h>
#include <CStrUtil.h>

class CAxis2D;
class CGnuPlotRenderer;

class CGnuPlot {
 public:
  enum ChangeState {
    CHANGE_STATE_PLOT_ADDED
  };

  enum Smooth {
    SMOOTH_NONE,
    SMOOTH_UNIQUE,
    SMOOTH_FREQUENCY,
    SMOOTH_BEZIER,
    SMOOTH_SBEZIER,
    SMOOTH_CSPLINES,
    SMOOTH_ACSPLINES
  };

  enum PlotStyle {
    NO_STYLE           = 0,
    LINES_STYLE        = (1<<0),
    POINTS_STYLE       = (1<<1),
    LINES_POINTS_STYLE = (LINES_STYLE | POINTS_STYLE)
  };

  struct Point {
    double x;
    double y;
    bool   discontinuity;

    Point(double x1=0.0, double y1=0.0, bool discontinuity1=false) :
     x(x1), y(y1), discontinuity(discontinuity1) {
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
      os << "(" << p.x << "," << p.y << ")";

      return os;
    }

    bool operator< (const Point &p) const { return (x < p.x || (x == p.x && y < p.y)); }
    bool operator> (const Point &p) const { return (x > p.x || (x == p.x && y > p.y)); }
    bool operator==(const Point &p) const { return (x == p.x && y == p.y); }

    bool operator>=(const Point &p) const { return ! (*this < p); }
    bool operator<=(const Point &p) const { return ! (*this > p); }

    bool operator!=(const Point &p) const { return ! (*this == p); }
  };

  struct UsingCol {
    UsingCol(int i=0) :
     str(""), isInt(true), ival(i) {
    }

    UsingCol(const std::string &str1) :
     str(str1), isInt(false), ival(-1) {
      isInt = CStrUtil::toInteger(str1, &ival);
    }

    std::string str;
    bool        isInt;
    int         ival;
  };

  typedef std::vector<Point> Points;

  class Plot {
   public:
    Plot(CGnuPlot *plot);
   ~Plot();

    uint numPoints() const { return points_.size(); }

    Point getPoint(int i) const { return points_[i]; }

    void clearPoints();

    void addPoint(double x, double y, bool discontinuity=false);

    void setXRange(double xmin, double xmax);
    void getXRange(double &xmin, double &xmax) { xmin = xmin_; xmax = xmax_; }

    void setYRange(double ymin, double ymax);
    void getYRange(double &ymin, double &ymax) { ymin = ymin_; ymax = ymax_; }

    void fit();
    void smooth();

    bool isShowXAxis() const { return xaxis_; }
    void showXAxis(bool show);

    bool isShowYAxis() const { return yaxis_; }
    void showYAxis(bool show);

    CAxis2D *getXAxis();
    CAxis2D *getYAxis();

    PlotStyle getStyle() const { return style_; }
    void setStyle(PlotStyle style) { style_ = style; }

    void setSmooth(Smooth s) { smooth_ = s; }
    Smooth getSmooth() const { return smooth_; }

    void setFit(bool b) { fit_ = b; }
    bool getFit() const { return fit_; }

    void draw(CGnuPlotRenderer *renderer);

   private:
    CGnuPlot          *plot_;
    Points             points_;
    double             xmin_, xmax_;
    double             ymin_, ymax_;
    CAutoPtr<CAxis2D>  xaxis_;
    CAutoPtr<CAxis2D>  yaxis_;
    PlotStyle          style_;
    Smooth             smooth_;
    bool               fit_;
  };

  typedef std::vector<Plot *> Plots;

 public:
  CGnuPlot();

  virtual ~CGnuPlot() { }

  void setDebug(bool b);

  bool isDebug() const { return debug_; }

  PlotStyle getDataStyle() const { return dataStyle_; }
  void setDataStyle(PlotStyle style) { dataStyle_ = style; }

  PlotStyle getFunctionStyle() const { return functionStyle_; }
  void setFunctionStyle(PlotStyle style) { functionStyle_ = style; }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  void setFit(bool b) { fit_ = b; }
  bool getFit() const { return fit_; }

  bool load(const std::string &filename);

  void loop();

  virtual void timeout() { }

  virtual void stateChanged(ChangeState) { }

  uint numPlots() const { return plots_.size(); }

  Plot *getPlot(int i) const { return plots_[i]; }

  void addPlot(Plot *plot);

 private:
  bool parseLine(const std::string &str);

  void showHelp(const std::string &args);
  void showHistory(const std::string &args);
  void printExpression(const std::string &args);
  void changeDir(const std::string &args);
  void printCurrenrDir();

  void callCmd(const std::string &args);
  void loadCmd(const std::string &args);
  void saveCmd(const std::string &args);

  bool plotCmd(const std::string &args);
  void replotCmd();
  void splotCmd(const std::string &args);

  void setCmd(const std::string &args);
  void showCmd(const std::string &args);
  void resetCmd();
  void unsetCmd(const std::string &args);

  void shellCmd(const std::string &args);
  void systemCmd(const std::string &args);

  void clearDevice();
  void lowerWindow(const std::string &args);
  void raiseWindow(const std::string &args);
  void testImage(const std::string &args);

  void fitPoints(const std::string &args);
  void updateFit(const std::string &args);

  void bindKeys(const std::string &args);
  void ifCmd(const std::string &args);
  void pauseMsg(const std::string &args);
  void rereadFile();

  Plot *addFunction(const std::string &str, PlotStyle style);

  Plot *addFile(const std::string &filename, PlotStyle style,
                const UsingCol &col1, const UsingCol &col2,
                int indexStart=1, int indexEnd=1, int indexStep=1,
                int everyStart=1, int everyEnd=INT_MAX, int everyStep=1);

  void drawPlot();

  void getXRange(double *xmin, double *xmax) const;
  void setXRange(double xmin, double xmax);

  void getYRange(double *ymin, double *ymax) const;
  void setYRange(double ymin, double ymax);

  void getNumX(int *nx) const;

  void setNameValue(const std::string &name, const std::string &value);

  std::string getNameValue(const std::string &name) const;

 private:
  typedef std::map<std::string,std::string> NameValues;

  bool       debug_;
  Plots      plots_;
  double     xmin_, xmax_;
  double     ymin_, ymax_;
  PlotStyle  dataStyle_;
  PlotStyle  functionStyle_;
  Smooth     smooth_;
  bool       fit_;
  NameValues nameValues_;
};

class CGnuPlotRenderer {
 public:
  typedef CGnuPlot::Point Point;

 public:
  CGnuPlotRenderer() { }

  virtual ~CGnuPlotRenderer() { }

  virtual void drawPoint (const Point &p) = 0;
  virtual void drawLine  (const Point &p1, const Point &p2) = 0;
  virtual void drawBezier(const Point &p1, const Point &p2, const Point &p3, const Point &p4) = 0;

  virtual void drawText(const Point &p, const std::string &text) = 0;

  virtual double pixelWidthToWindowWidth(double w) = 0;
  virtual double pixelHeightToWindowHeight(double h) = 0;

  virtual CFontPtr getFont() const = 0;

  virtual void setFont(CFontPtr font) = 0;
};

#endif
