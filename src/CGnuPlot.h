#ifndef CGnuPlot_H
#define CGnuPlot_H

#include <string>
#include <vector>
#include <iostream>
#include <sys/types.h>

#include <CFont.h>

class CAxis2D;
class CGnuPlotRenderer;

class CGnuPlot {
 public:
  enum ChangeState {
    CHANGE_STATE_PLOT_ADDED
  };

  struct Point {
    double x;
    double y;

    Point(double x1=0.0, double y1=0.0) :
     x(x1), y(y1) {
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
      os << "(" << p.x << "," << p.y << ")";

      return os;
    }
  };

  typedef std::vector<Point> Points;

  class Plot {
   public:
    Plot();
   ~Plot();

    uint numPoints() const { return points_.size(); }

    Point getPoint(int i) const { return points_[i]; }

    void addPoint(double x, double y);

    void setXRange(double xmin, double xmax);
    void setYRange(double ymin, double ymax);

    void getXRange(double &xmin, double &xmax) { xmin = xmin_; xmax = xmax_; }
    void getYRange(double &ymin, double &ymax) { ymin = ymin_; ymax = ymax_; }

    void fit();

    bool isShowXAxis() const { return xaxis_; }
    bool isShowYAxis() const { return yaxis_; }

    void showXAxis(bool show);
    void showYAxis(bool show);

    CAxis2D *getXAxis();
    CAxis2D *getYAxis();

    void draw(CGnuPlotRenderer *renderer);

   private:
    Points   points_;
    double   xmin_, xmax_;
    double   ymin_, ymax_;
    CAxis2D *xaxis_, *yaxis_;
  };

  typedef std::vector<Plot *> Plots;

 public:
  CGnuPlot();

  virtual ~CGnuPlot() { }

  void setDebug(bool b) { debug_ = b; }

  bool load(const std::string &filename);

  void loop();

  virtual void timeout() { }

  virtual void stateChanged(ChangeState) { }

  uint numPlots() const { return plots_.size(); }

  Plot *getPlot(int i) const { return plots_[i]; }

 private:
  bool parseLine(const std::string &str);

  void showHelp(const std::string &args);
  void showHistory(const std::string &args);
  void printExpression(const std::string &args);
  void changeDir(const std::string &args);
  void printCurrenrDir();

  void callCommand(const std::string &args);
  void loadCommand(const std::string &args);
  void saveSession(const std::string &args);

  bool plotCmd(const std::string &args);
  void replot();
  void addSPlot(const std::string &args);

  void setOption(const std::string &args);
  void showInfo(const std::string &args);
  void resetOptions();
  void unsetOption(const std::string &args);

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

  Plot *addFunction(const std::string &str);
  Plot *addFile(const std::string &filename, int col1=1, int col2=2);

  void drawPlot();

  void drawLine(const Point &p1, const Point &p2);

  void getXRange(double *xmin, double *xmax) const;
  void setXRange(double xmin, double xmax);

  void getYRange(double *ymin, double *ymax) const;
  void setYRange(double ymin, double ymax);

  void getNumX(int *nx) const;

 private:
  bool   debug_;
  Plots  plots_;
  double xmin_, xmax_;
  double ymin_, ymax_;
};

class CGnuPlotRenderer {
 public:
  CGnuPlotRenderer() { }

  virtual ~CGnuPlotRenderer() { }

  virtual void drawPoint(const CGnuPlot::Point &p) = 0;
  virtual void drawLine (const CGnuPlot::Point &p1, const CGnuPlot::Point &p2) = 0;

  virtual void drawText(const CGnuPlot::Point &p, const std::string &text) = 0;

  virtual double pixelLengthToWindowLength(double p) = 0;

  virtual CFontPtr getFont() const = 0;

  virtual void setFont(CFontPtr font) = 0;
};

#endif
