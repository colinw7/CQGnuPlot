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
#include <COptVal.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <NaN.h>

class CAxis2D;
class CGnuPlotRenderer;
class CGnuPlotReadLine;

class CGnuPlot {
 public:
  enum CommandName {
    NO_CMD,
    EXIT_CMD,
    HELP_CMD,
    HISTORY_CMD,
    PRINT_CMD,
    QUIT_CMD,
    CD_CMD,
    PWD_CMD,
    CALL_CMD,
    LOAD_CMD,
    SAVE_CMD,
    PLOT_CMD,
    REPLOT_CMD,
    SPLOT_CMD,
    SET_CMD,
    SHOW_CMD,
    RESET_CMD,
    UNSET_CMD,
    SHELL_CMD,
    SYSTEM_CMD,
    CLEAR_CMD,
    LOWER_CMD,
    RAISE_CMD,
    TEST_CMD,
    FIT_CMD,
    UPDATE_CMD,
    BIND_CMD,
    IF_CMD,
    PAUSE_CMD,
    REREAD_CMD
  };

  enum VariableName {
    NO_VAR,

    DATAFILE_VAR,
    DECIMALSIGN_VAR,
    ENCODING_VAR,
    FIT_VAR,
    FONTPATH_VAR,
    HISTORYSIZE_VAR,
    LOADPATH_VAR,
    MACROS_VAR,
    MOUSE_VAR,

    ANGLES_VAR,
    DUMMY_VAR,
    MAPPING_VAR,
    PARAMETRIC_VAR,
    POLAR_VAR,
    SAMPLES_VAR,
    ZERO_VAR,

    STYLE_VAR,
    POINT_SIZE_VAR,
    TABLE_VAR,
    BOX_WIDTH_VAR,
    VARIABLES_VAR,
    FUNCTIONS_VAR,
    VERSION_VAR,
    DEBUG_VAR,
    EDEBUG_VAR
  };

  enum DataFileVar {
    DATAFILE_NO_VAR,
    DATAFILE_SEPARATOR_VAR,
    DATAFILE_COMMENTS_CHAR_VAR,
    DATAFILE_MISSING_VAR
  };

  enum StyleVar {
    STYLE_NO_VAR,
    STYLE_DATA_VAR,
    STYLE_FUNCTION_VAR,
    STYLE_LINE_VAR,
    STYLE_FILL_VAR,
    STYLE_HISTOGRAM_VAR,
    STYLE_RECTANGLE_VAR,
    STYLE_CIRCLE_VAR,
    STYLE_ELLIPSE_VAR,
    STYLE_ARROW_VAR
  };

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
    DOTS_STYLE         = (1<<2),
    HISTOGRAMS_STYLE   = (1<<3),
    LINES_POINTS_STYLE = (LINES_STYLE | POINTS_STYLE),
    LABELS_STYLE       = (1<<4)
  };

  enum SeparatorType {
    SEP_WHITESPACE,
    SEP_CHAR
  };

  enum FillStyle {
    FILL_EMPTY,
    FILL_SOLID,
    FILL_PATTERN
  };

  enum HistogramStyle {
    HISTOGRAM_STYLE_NONE,
    HISTOGRAM_STYLE_CLUSTERED,
    HISTOGRAM_STYLE_ERRORBARS,
    HISTOGRAM_STYLE_ROWSTACKED,
    HISTOGRAM_STYLE_COLUMNSTACKED
  };

  struct Separator {
    Separator() : type(SEP_WHITESPACE), c('\0') { }

    Separator(char c) : type(SEP_CHAR), c(c) { }

    SeparatorType type;
    char          c;
  };

  struct Point {
    COptReal   x;
    COptReal   y;
    COptString str;
    bool       discontinuity;

    Point() :
     x(), y(), str(), discontinuity(false) {
    }

    Point(const COptReal &x1, const COptReal &y1, const COptString &str1, bool discontinuity1) :
     x(x1), y(y1), str(str1), discontinuity(discontinuity1) {
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
      os << "(" << p.x << "," << p.y << ")";

      return os;
    }

    bool isXValid() const { return x.isValid() && ! IsNaN(x.getValue()); }
    bool isYValid() const { return y.isValid() && ! IsNaN(y.getValue()); }

    bool isXYValid() const { return isXValid() && isYValid(); }

    double getX() const { return x.getValue(0.0); }
    double getY() const { return y.getValue(0.0); }

    CPoint2D getXY() const { return CPoint2D(x.getValue(0.0), y.getValue(0.0)); }

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

    CGnuPlot *plot() const { return plot_; }

    uint numPoints() const { return points_.size(); }

    const Point &getPoint(int i) const { return points_[i]; }

    void clearPoints();

    void addPoint(const COptReal &x, const COptReal &y, const COptString &str=COptString(),
                  bool discontinuity=false);

    void fit();
    void smooth();

    int ind() const { return ind_; }
    void setInd(int ind) { ind_ = ind; }

    void getXRange(double *xmin, double *xmax) const;
    void getYRange(double *ymin, double *ymax) const;

    PlotStyle getStyle() const { return style_; }
    void setStyle(PlotStyle style) { style_ = style; }

    void setSmooth(Smooth s) { smooth_ = s; }
    Smooth getSmooth() const { return smooth_; }

    void setFitX(bool b) { fitX_ = b; }
    bool getFitX() const { return fitX_; }
    void setFitY(bool b) { fitY_ = b; }
    bool getFitY() const { return fitY_; }

    void draw();

   private:
    CGnuPlot* plot_;
    Points    points_;
    int       ind_;
    double    xmin_, xmax_;
    double    ymin_, ymax_;
    PlotStyle style_;
    Smooth    smooth_;
    bool      fitX_, fitY_;
  };

  typedef std::vector<Plot *> Plots;

 public:
  CGnuPlot();

  virtual ~CGnuPlot();

  void setDebug(bool b);
  bool isDebug() const { return debug_; }

  void setExprDebug(bool b);
  bool isExprDebug() const { return edebug_; }

  void setRenderer(CGnuPlotRenderer *renderer);
  CGnuPlotRenderer *renderer() const { return renderer_; }

  PlotStyle getDataStyle() const { return dataStyle_; }
  void setDataStyle(PlotStyle style) { dataStyle_ = style; }

  PlotStyle getFunctionStyle() const { return functionStyle_; }
  void setFunctionStyle(PlotStyle style) { functionStyle_ = style; }

  FillStyle getFillStyle() const { return fillStyle_; }
  void setFillStyle(FillStyle style) { fillStyle_ = style; }

  bool getFillBorder() const { return fillBorder_; }
  void setFillBorder(bool b) { fillBorder_ = b; }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  void setFitX(bool b) { fitX_ = b; }
  bool getFitX() const { return fitX_; }
  void setFitY(bool b) { fitY_ = b; }
  bool getFitY() const { return fitY_; }

  bool load(const std::string &filename);

  void loop();

  virtual void timeout() { }

  virtual void stateChanged(ChangeState) { }

  uint numPlots() const { return plots_.size(); }

  Plot *getPlot(int i) const { return plots_[i]; }

  void addPlot(Plot *plot);

  void getXRange(double *xmin, double *xmax) const;
  void setXRange(double xmin, double xmax);

  void getYRange(double *ymin, double *ymax) const;
  void setYRange(double ymin, double ymax);

  std::string getField(int i) const {
    if (i < 1 || i > int(fields_.size())) return "";
    return fields_[i - 1];
  }

  void paintStart();
  void paintEnd();

  bool isShowXAxis() const { return xaxis_; }
  void showXAxis(bool show);

  bool isShowYAxis() const { return yaxis_; }
  void showYAxis(bool show);

  CAxis2D *getXAxis();
  CAxis2D *getYAxis();

 private:
  bool parseLine(const std::string &str);

  void exitCmd   (const std::string &args);
  void helpCmd   (const std::string &args);
  void historyCmd(const std::string &args);
  void printCmd  (const std::string &args);
  void quitCmd   (const std::string &args);
  void cdCmd     (const std::string &args);
  void pwdCmd    (const std::string &args);

  void callCmd(const std::string &args);
  void loadCmd(const std::string &args);
  void saveCmd(const std::string &args);

  bool plotCmd  (const std::string &args);
  void replotCmd(const std::string &args);
  void splotCmd (const std::string &args);

  void setCmd  (const std::string &args);
  void showCmd (const std::string &args);
  void resetCmd(const std::string &args);
  void unsetCmd(const std::string &args);

  void shellCmd (const std::string &args);
  void systemCmd(const std::string &args);

  void clearCmd(const std::string &args);
  void lowerCmd(const std::string &args);
  void raiseCmd(const std::string &args);
  void testCmd (const std::string &args);

  void fitCmd   (const std::string &args);
  void updateCmd(const std::string &args);

  void bindCmd  (const std::string &args);
  void ifCmd    (const std::string &args);
  void pauseCmd (const std::string &args);
  void rereadCmd(const std::string &args);

  Plot *addFunction(const std::string &str, PlotStyle style);

  Plot *addFile(const std::string &filename, PlotStyle style,
                const UsingCol &col1, const UsingCol &col2, const UsingCol &col3,
                int indexStart=1, int indexEnd=1, int indexStep=1,
                int everyStart=1, int everyEnd=INT_MAX, int everyStep=1);

  void drawPlot();

  void getNumX(int *nx) const;

  void setSeparator(Separator sep) { separator_ = sep; }
  const Separator &getSeparator() { return separator_; }

  void setCommentChars(const std::string &chars) { commentChars_ = chars; }
  const std::string &getCommentChars() { return commentChars_; }

  void setMissingStr(const std::string &chars) { missingStr_ = chars; }
  const std::string &getMissingStr() { return missingStr_; }

  void setTableFile(const std::string &file) { tableFile_ = file; }
  const std::string &getTableFile() { return tableFile_; }

  void setHistogramStyle(HistogramStyle style) { histogramStyle_ = style; }
  HistogramStyle getHistogramStyle() { return histogramStyle_; }

  void setDummyVars(const std::string &dummyVar1, const std::string &dummyVar2) {
    dummyVar1_ = dummyVar1;
    dummyVar2_ = dummyVar2;
  }

  void getDummyVars(std::string &dummyVar1, std::string &dummyVar2) const {
    if (! dummyVar1_.empty()) dummyVar1 = dummyVar1_;
    if (! dummyVar2_.empty()) dummyVar2 = dummyVar2_;
  }

  void setSamples(int isamples1, int isamples2) {
    isamples1_ = isamples1;
    isamples2_ = isamples2;
  }

  void getSamples(int &isamples1, int &isamples2) const {
    isamples1 = isamples1_;
    isamples2 = isamples2_;
  }

  bool parseString(const std::string &line, std::string &filename);

 private:
  bool                             debug_, edebug_;
  CGnuPlotRenderer*                renderer_;
  CAutoPtr<CAxis2D>                xaxis_;
  CAutoPtr<CAxis2D>                yaxis_;
  Plots                            plots_;
  COptReal                         xmin_, xmax_, ymin_, ymax_;
  Separator                        separator_;
  std::string                      commentChars_;
  std::string                      missingStr_;
  std::string                      tableFile_;
  PlotStyle                        dataStyle_;
  PlotStyle                        functionStyle_;
  FillStyle                        fillStyle_;
  bool                             fillBorder_;
  Smooth                           smooth_;
  HistogramStyle                   histogramStyle_;
  bool                             fitX_, fitY_;
  std::string                      dummyVar1_, dummyVar2_;
  int                              isamples1_, isamples2_;
  CAutoPtr<CGnuPlotReadLine>       readLine_;
  mutable std::vector<std::string> fields_;
};

class CGnuPlotRenderer {
 public:
  CGnuPlotRenderer() { }

  virtual ~CGnuPlotRenderer() { }

  virtual void drawPoint (const CPoint2D &p) = 0;
  virtual void drawSymbol(const CPoint2D &p, int i) = 0;
  virtual void drawLine  (const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void drawRect  (const CBBox2D &rect) = 0;
  virtual void drawBezier(const CPoint2D &p1, const CPoint2D &p2,
                          const CPoint2D &p3, const CPoint2D &p4) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &text) = 0;

  virtual double pixelWidthToWindowWidth  (double w) = 0;
  virtual double pixelHeightToWindowHeight(double h) = 0;

  virtual CFontPtr getFont() const = 0;

  virtual void setFont(CFontPtr font) = 0;
};

#endif
