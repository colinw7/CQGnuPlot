#ifndef CGnuPlot_H
#define CGnuPlot_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <climits>
#include <memory>
#include <sys/types.h>

#include <CExpr.h>
#include <CFont.h>
#include <CAutoPtr.h>
#include <CStrUtil.h>
#include <COptVal.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <CLineDash.h>
#include <CCoordFrame3D.h>
#include <COrientation.h>
#include <CMatrix3DH.h>
#include <CRGBA.h>
#include <CAlignType.h>
#include <NaN.h>

#define ACCESSOR(T,V,G,S) \
const T &G() const { return V; } \
void S(const T &t) { V = t; }

class CGnuPlotAxis;
class CGnuPlotDevice;
class CGnuPlotGroup;
class CGnuPlotPlot;
class CGnuPlotReadLine;
class CGnuPlotSVGDevice;
class CGnuPlotWindow;

class CUnixFile;
class CParseLine;

typedef CRefPtr<CExprValue>             CExprValueP;
typedef std::shared_ptr<CGnuPlotWindow> CGnuPlotWindowP;

//------

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotCoordValue.h>
#include <CGnuPlotKeyData.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotObject.h>

//------

class CGnuPlot {
 public:
  enum class CommandName {
    NONE,
    BIND,
    CALL,
    CD,
    CLEAR,
    DO,
    EVALUATE,
    EXIT,
    FIT,
    HELP,
    HISTORY,
    IF,
    LOAD,
    LOWER,
    PAUSE,
    PLOT,
    PRINT,
    PWD,
    QUIT,
    RAISE,
    REPLOT,
    REFRESH,
    REREAD,
    RESET,
    SAVE,
    SET,
    SHELL,
    SHOW,
    SPLOT,
    STATS,
    SYSTEM,
    TEST,
    UNDEFINE,
    UNSET,
    UPDATE,
    WHILE
  };

  enum class VariableName {
    NONE,

    DATAFILE,
    DECIMALSIGN,
    ENCODING,
    FIT,
    FONTPATH,
    HISTORY,
    HISTORYSIZE,
    LOADPATH,
    MACROS,
    MOUSE,

    ANGLES,
    DUMMY,
    MAPPING,
    PARAMETRIC,
    POLAR,
    SAMPLES,
    ZERO,

    BORDER,
    CLIP,
    LOGSCALE,
    BMARGIN,
    LMARGIN,
    RMARGIN,
    TMARGIN,
    MARGINS,
    MULTIPLOT,
    OFFSETS,
    ORIGIN,
    SIZE,

    ARROW,
    KEY,
    LABEL,
    OBJECT,
    TIMESTAMP,
    TITLE,

    STYLE,
    DASHTYPE,
    LINETYPE,

    BARS,
    BOXWIDTH,
    POINTSIZE,

    AUTOSCALE,
    FORMAT,
    GRID,
    XLABEL,
    YLABEL,
    X2LABEL,
    Y2LABEL,
    XRANGE,
    X2RANGE,
    YRANGE,
    Y2RANGE,
    ZRANGE,
    CBRANGE,
    TRANGE,
    RRANGE,
    TICS,
    XTICS,
    YTICS,
    X2TICS,
    Y2TICS,
    ZTICS,
    CBTICS,
    RTICS,
    MXTICS,
    MYTICS,
    MZTICS,
    MX2TICS,
    MY2TICS,
    TICSCALE,
    ZEROAXIS,
    RAXIS,
    PAXIS,
    LINK,

    LOCALE,
    TIMEFMT,
    XDATA,
    YDATA,
    X2DATA,
    Y2DATA,
    XDTICS,
    XMTICS,

    DGRID3D,
    HIDDEN3D,
    ISOSAMPLES,
    SURFACE,
    VIEW,
    XYPLANE,

    CPLANE,
    CNTRPARAM,
    CONTOUR,

    COLORBOX,
    COLORSEQUENCE,
    PALETTE,
    PM3D,

    OUTPUT,
    PRINT,
    TABLE,
    TERMINAL,
    TERMOPTION,

    VARIABLES,
    FUNCTIONS,
    VERSION,

    DEBUG,
    EDEBUG
  };

  enum class DataFileVar {
    NONE,
    SEPARATOR,
    COMMENTS_CHAR,
    MISSING
  };

  enum class StyleVar {
    NONE,
    DATA,
    FUNCTION,
    INCREMENT,
    LINE,
    FILL,
    BOXPLOT,
    HISTOGRAM,
    RECTANGLE,
    CIRCLE,
    ELLIPSE,
    ARROW
  };

  enum class StyleIncrementType {
    USER,
    DEFAULT
  };

  enum class BoxWidthType {
    AUTO,
    ABSOLUTE,
    RELATIVE
  };

  enum class Smooth {
    NONE,
    UNIQUE,
    FREQUENCY,
    BEZIER,
    SBEZIER,
    CSPLINES,
    ACSPLINES
  };

  enum class PlotVar {
    ARRAY,
    ARROWSTYLE,
    AXES,
    BINARY,
    CENTER,
    DASHTYPE,
    DX,
    DY,
    DZ,
    ENDIAN,
    EVERY,
    FILETYPE,
    FILLSTYLE,
    FLIP,
    FLIPX,
    FLIPY,
    FLIPZ,
    FORMAT,
    HEADS,
    INDEX,
    LINETYPE,
    LINEWIDTH,
    MATRIX,
    NOTITLE,
    ORIGIN,
    POINTSIZE,
    ROTATION,
    SCAN,
    SKIP,
    SMOOTH,
    TITLE,
    TRANSPOSE,
    USING,
    WHISKERBARS,
    WITH
  };

  enum class PlotStyle {
    NONE,
    BOXERRORBARS,
    BOXES,
    BOXPLOT,
    BOXXYERRORBARS,
    CANDLESTICKS,
    CIRCLES,
    DOTS,
    ELLIPSES,
    ERRORBARS,
    ERRORLINES,
    FILLEDCURVES,
    FILLSTEPS,
    FINANCEBARS,
    FSTEPS,
    HISTEPS,
    HISTOGRAMS,
    IMAGE,
    IMPULSES,
    LABELS,
    LINES,
    LINES_POINTS,
    PARALLELAXES,
    PIECHART,
    PM3D,
    POINTS,
    RGBALPHA,
    RGBIMAGE,
    STEPS,
    VECTORS,
    XERRORBARS,
    XERRORLINES,
    XYERRORBARS,
    XYERRORLINES,
    YERRORBARS,
    YERRORLINES,
    TEST_TERMINAL,
    TEST_PALETTE
  };

  enum class SeparatorType {
    WHITESPACE,
    CHAR
  };

  enum class HistogramStyle {
    NONE,
    CLUSTERED,
    ERRORBARS,
    ROWSTACKED,
    COLUMNSTACKED
  };

  enum class AngleType {
    RADIANS,
    DEGREES
  };

  enum class LogScale {
    X,
    Y,
    Z,
    X2,
    Y2,
    CB
  };

  typedef std::map<LogScale,int> LogScaleMap;

  enum class TestType {
    NONE,
    TERMINAL,
    PALETTE
  };

  enum class ColorModel {
    RGB,
    HSV,
    CMY,
    YIQ,
    XYZ,
  };

  //---

  struct Separator {
    Separator() { }

    Separator(char c) : type(SeparatorType::CHAR), c(c) { }

    SeparatorType type { SeparatorType::WHITESPACE };
    char          c    { '\0' };
  };

  //---

  struct UsingCol {
    UsingCol(int i) :
     str(""), isInt(true), ival(i) {
    }

    UsingCol(const std::string &str1) :
     str(str1), isInt(false), ival(-1) {
      isInt = CStrUtil::toInteger(str1, &ival);
    }

    std::string str;
    bool        isInt { true };
    int         ival  { 0 };
  };

  typedef std::vector<UsingCol> UsingCols;

  //---

  struct ImageStyle {
    int         w      { 1 };
    int         h      { 1 };
    COptReal    xo, yo;
    COptReal    cx, cy;
    COptReal    dx, dy;
    COptReal    a;
    bool        flipy  { false };
    std::string format { "" };
    UsingCols   usingCols;
  };

  //---

  struct Index {
    Index() { }

    int start { 1 };
    int end   { 1 };
    int step  { 1 };
  };

  //---

  struct Every {
    Every() { }

    int start { 0 };
    int end   { INT_MAX };
    int step  { 1 };
  };

  //---

  typedef std::map<int,CGnuPlotLineStyleP> LineStyles;

  //---

  class PointStyle {
   public:
    PointStyle() { }

    bool varSize() const { return varSize_; }
    void setVarSize(bool b) { varSize_ = b; }

  private:
    bool varSize_ { false };
  };

  //---

  struct BoxWidth {
    BoxWidth(double w=1, BoxWidthType t=BoxWidthType::AUTO) :
     width(w), type(t) {
    }

    double       width { 1 };
    BoxWidthType type  { BoxWidthType::AUTO };

    double getSpacing(double s) const {
      if      (type == BoxWidthType::ABSOLUTE) return width;
      else if (type == BoxWidthType::RELATIVE) return width*s;
      else                                     return s;
    }
  };

  //---

  struct BarSize {
    BarSize(double s=1, bool f=true) :
     size(s), front(f) {
    }

    double size { 1 };
    bool   front { true };
  };

  //---

  struct BoxPlot {
    BoxPlot() { }

    bool outliers  { true };
    int  pointtype { -1 };
    int  sorted    { false };
  };

  //---

  struct Title {
    Title() { }

    std::string str;
    CPoint2D    offset { 0, 0 };
    std::string font;
    CRGBA       color;
    bool        enhanced { false };
  };

  //---

  struct AxisData {
    typedef std::map<int,std::string> TicLabelMap;

    AxisData(int i=1) : ind(i) { }

    int         ind;
    bool        displayed { true  };
    bool        grid      { false };
    bool        mirror    { true  };
    bool        reverse   { false };
    bool        writeback { false };
    bool        isTime    { false };
    COptReal    min;
    COptReal    max;
    std::string str;
    TicLabelMap ticlabel;
    double      offset    { 0 };
    std::string format;
    std::string font;
    bool        showTics  { true };
  };

  typedef std::map<int,AxisData> IAxisMap;

  struct AxesData {
    AxesData() { }

    IAxisMap xaxis;
    IAxisMap yaxis;
    IAxisMap zaxis;
    IAxisMap paxis;
    IAxisMap taxis;
    int      borders     { 0xFF };
    double   borderWidth { 1.0 };
    COptInt  borderStyle;
    COptInt  borderType;
  };

  struct StyleIncrement {
    StyleIncrement() { }

    StyleIncrementType type     { StyleIncrementType::USER };
    int                styleInd { 0 };
  };

  //---

  struct FilledCurve {
    bool               closed { true };
    bool               above  { false  };
    bool               below  { false  };
    int                xaxis  { 0 };
    int                yaxis  { 0 };
    COptReal           xval;
    COptReal           yval;
    COptValT<CPoint2D> xyval;
  };

  //---

  struct PlotSize {
    PlotSize() { }

    bool     square { false };
    COptReal ratio;
    COptReal x;
    COptReal y;
    COptReal xsize;
    COptReal ysize;
  };

  struct Multiplot {
    Multiplot() { }

    bool        enabled { false };
    int         rows    { 1 };
    int         cols    { 1 };
    std::string title;
  };

  //---

  typedef std::map<int,CGnuPlotArrowStyle> ArrowStyles;
  typedef std::map<int,CLineDash>          LineDashes;

  typedef std::vector<CGnuPlotArrowP>     Arrows;
  typedef std::vector<CGnuPlotLabelP>     Labels;
  typedef std::vector<CGnuPlotEllipseP>   Ellipses;
  typedef std::vector<CGnuPlotPolygonP>   Polygons;
  typedef std::vector<CGnuPlotRectangleP> Rectangles;

  //---

  struct Palette {
    typedef std::map<double,CRGBA> ColorMap;

    ColorModel colorModel;
    ColorMap   colors;

    void addColor(double v, const CRGBA &c) {
      colors[v] = c;
    }

    CRGBA getColor(double x) const {
      if (colors.empty()) {
        Palette *th = const_cast<Palette *>(this);

        th->addColor(0.0, CRGBA(0,0,0));
        th->addColor(1.0, CRGBA(1,1,1));
      }

      double min = colors. begin()->first;
      double max = colors.rbegin()->first;

      auto p = colors.begin();

      double x1    = ((*p).first - min)/(max - min);
      CRGBA  rgba1 = (*p).second;

      if (x <= x1) return rgba1;

      for (++p; p != colors.end(); ++p) {
        double x2    = ((*p).first - min)/(max - min);
        CRGBA  rgba2 = (*p).second;

        if (x <= x2) {
          double m = (x - x1)/(x2 - x1);

          return (1.0 - m)*rgba1 + m*rgba2;
        }

        x1    = x2;
        rgba1 = rgba2;
      }

      return rgba1;
    }
  };

  struct Camera {
    bool          enabled    { true };
    CCoordFrame3D coordFrame;
    CVector3D     direction  { 0, 0, 1 };
    double        fov        { 90 };
    double        xmin       { -1 };
    double        xmax       {  1 };
    double        ymin       { -1 };
    double        ymax       {  1 };
    double        near       { 0.1 };
    double        far        { 100 };
    CMatrix3DH    projMatrix;
    double        rotateX    { 60.0 };
    double        rotateY    {  0.0 };
    double        rotateZ    { 45.0 };

    Camera() { init(); }

    void init() {
      coordFrame.init();

      setDirection(CVector3D(0,0,1));

      //projMatrix.buildPerspective(fov, 1.0, near, far);
      projMatrix.buildOrtho(xmin, xmax, ymin, ymax, near, far);
      //projMatrix.buildFrustrum(-2, 2, -2, 2, near, far);

      rotateDX(M_PI*rotateX/180.0);
      rotateDY(M_PI*rotateY/180.0);
      rotateDZ(M_PI*rotateZ/180.0);
    }

    void setPosition(const CPoint3D &position) {
      coordFrame.setOrigin(position);
    }

    void setDirection(const CVector3D &dir) {
      CVector3D right, up, dir1;

      coordFrame.getBasis(right, up, dir1);

      dir1 = dir.unit();

      right = dir1 .crossProduct(up );
      up    = right.crossProduct(dir1);

      if (COrthonormalBasis3DT<double>::validate(right, up, dir1)) {
        coordFrame.setBasis(right, up, dir1);

        direction = dir;
      }
    }

    void moveDX(double dx) { coordFrame.moveX(dx); }
    void moveDY(double dy) { coordFrame.moveY(dy); }
    void moveDZ(double dz) { coordFrame.moveZ(dz); }

    void rotateDX(double dx) { coordFrame.rotateAboutX(dx); }
    void rotateDY(double dy) { coordFrame.rotateAboutY(dy); }
    void rotateDZ(double dz) { coordFrame.rotateAboutZ(dz); }

    CPoint3D transform(const CPoint3D &p) const {
      if (! enabled) return p;

      CPoint3D p1 = coordFrame.transformTo(p);

      CPoint3D p2;

      projMatrix.multiplyPoint(p1, p2);

      return p2;
    }
  };

  typedef std::vector<CGnuPlotPlot *> Plots;
  typedef std::vector<std::string>    Statements;

  class PrefValueBase {
   public:
    PrefValueBase() { }

    virtual ~PrefValueBase() { }

    virtual void set(const std::string &str) = 0;

    virtual void reset() = 0;

    virtual void print(std::ostream &os) const = 0;
  };

  typedef std::map<CGnuPlot::VariableName,PrefValueBase *> VarPrefs;

 public:
  CGnuPlot();

  virtual ~CGnuPlot();

  void setDebug(bool b) { debug_ = b; }
  bool isDebug() const { return debug_; }

  void setExprDebug(bool b);
  bool isExprDebug() const { return edebug_; }

  CGnuPlotDevice *device() const { return device_; }

  void addDevice(const std::string &name, CGnuPlotDevice *device);
  bool setDevice(const std::string &name);

  const CISize2D &getTerminalSize() const { return terminalSize_; }
  void setTerminalSize(const CISize2D &s) { terminalSize_ = s; }

  PlotStyle getDataStyle() const { return dataStyle_; }
  void setDataStyle(PlotStyle style) { dataStyle_ = style; }

  PlotStyle getFunctionStyle() const { return functionStyle_; }
  void setFunctionStyle(PlotStyle style) { functionStyle_ = style; }

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &s) { fillStyle_ = s; }

  CGnuPlotLineStyleP lineStyle();
  void setLineStyle(CGnuPlotLineStyleP ls) { lineStyle_ = ls; }

  CGnuPlotLineStyleP getLineStyleInd(int ind);
  void setLineStyleInd(int ind);

  const PointStyle &pointStyle() const { return pointStyle_; }
  PointStyle &pointStyle() { return pointStyle_; }
  void setPointStyle(const PointStyle &s) { pointStyle_ = s; }

  const LineStyles &lineStyles() const { return lineStyles_; }
  CGnuPlotLineStyleP lineStyle(int i) const;

  HistogramStyle histogramStyle() { return histogramStyle_; }
  void setHistogramStyle(HistogramStyle style) { histogramStyle_ = style; }

  const CGnuPlotArrowStyle &arrowStyle() const { return arrowStyle_; }
  void setArrowStyle(const CGnuPlotArrowStyle &as) { arrowStyle_ = as; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &r) { region_ = r; }

  const CRange2D &margin() const { return margin_; }
  void setMargin(const CRange2D &b) { margin_ = b; }

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  int xind() const { return xind_; }
  int yind() const { return yind_; }

  const PlotSize &plotSize() const { return plotSize_; }
  PlotSize &plotSize() { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  void getXRange(double *xmin, double *xmax) {
    *xmin = axesData_.xaxis[1].min.getValue(-10);
    *xmax = axesData_.xaxis[1].max.getValue(10);
  }
  void getYRange(double *ymin, double *ymax) {
    *ymin = axesData_.yaxis[1].min.getValue(-1);
    *ymax = axesData_.yaxis[1].max.getValue(1);
  }
  void getZRange(double *zmin, double *zmax) {
    *zmin = axesData_.zaxis[1].min.getValue(-1);
    *zmax = axesData_.zaxis[1].max.getValue(1);
  }
  void getTRange(double *tmin, double *tmax) {
    *tmin = axesData_.taxis[1].min.getValue(0);
    *tmax = axesData_.taxis[1].max.getValue(M_PI);
  }

  //---

  void setBoxWidth(const BoxWidth &w) { boxWidth_ = w; }
  const BoxWidth &getBoxWidth() const { return boxWidth_; }

  void setBarSize(const BarSize &s) { barSize_ = s; }
  const BarSize &getBarSize() const { return barSize_; }

  void setBoxPlot(const BoxPlot &b) { boxPlot_ = b; }
  const BoxPlot &getBoxPlot() const { return boxPlot_; }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const Palette &palette() const { return palette_; }
  void setPalette(const Palette &p) { palette_ = p; }

  const FilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const FilledCurve &c) { filledCurve_ = c; }

  const LogScaleMap &logScaleMap() const { return logScale_; }

  int trianglePattern3D() const { return trianglePattern3D_; }
  void setTrianglePattern3D(int n) { trianglePattern3D_ = n; }

  double whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }

  const Camera &camera() const { return camera_; }
  Camera &camera() { return camera_; }
  void setCamera(const Camera &c) { camera_ = c; }

  //------

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool m) { matrix_ = m; }

  const ImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const ImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  //------

  const Arrows &arrows() const { return arrows_; }
  void setArrows(const Arrows &arrows) { arrows_ = arrows; }

  const Labels &labels() const { return labels_; }
  void setLabels(const Labels &labels) { labels_ = labels; }

  const Rectangles &rectangles() const { return rects_; }
  void setRectangles(const Rectangles &rects) { rects_ = rects; }

  const Ellipses &ellipses() const { return ellipses_; }
  void setEllipses(const Ellipses &ellipses) { ellipses_ = ellipses; }

  const Polygons &polygons() const { return polygons_; }
  void setPolygons(const Polygons &polygons) { polygons_ = polygons; }

  //------

  void setOutputFile(const std::string &file) { outputFile_ = file; }
  const std::string &getOutputFile() const { return outputFile_; }

  bool load(const std::string &filename);

  void initReadLine();

  void loop();

  void prompt(const std::string &msg);

  CGnuPlotWindow *createWindow();

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group);

  CGnuPlotLineStyle *createLineStyle();

  CGnuPlotArrow     *createArrow();
  CGnuPlotLabel     *createLabel();
  CGnuPlotEllipse   *createEllipse();
  CGnuPlotPolygon   *createPolygon();
  CGnuPlotRectangle *createRectangle();

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir);

  CGnuPlotRenderer *renderer();

  void timeout();

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state);

  std::string getField(int i) const {
    if (i < 1 || i > int(fields_.size())) return "";
    return fields_[i - 1];
  }

  void resetLineStyle();

  void incLineStyle();

  void drawWindows();

 private:
  bool parseLine(const std::string &str);
  bool parseStatement(int &i, const Statements &statements);

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

  void plotCmd   (const std::string &args);
  void replotCmd (const std::string &args);
  void refreshCmd(const std::string &args);
  void splotCmd  (const std::string &args);

  void plotForCmd (const std::string &var, const std::string &start, const std::string &end,
                   const std::string &inc, const std::string &args);
  void setForCmd  (const std::string &var, const std::string &start, const std::string &end,
                   const std::string &inc, const std::string &args);
  void unsetForCmd(const std::string &var, const std::string &start, const std::string &end,
                   const std::string &inc, const std::string &args);

  bool parseFor(CParseLine &line, std::string &var, std::string &start, std::string &end,
                std::string &inc, std::string &lcmd, std::string &rcmd);

  void setCmd  (const std::string &args);
  void showCmd (const std::string &args);
  void resetCmd(const std::string &args);
  void undefCmd(const std::string &args);
  void unsetCmd(const std::string &args);

  void shellCmd (const std::string &args);
  void systemCmd(const std::string &args);
  void statsCmd (const std::string &args);

  void clearCmd(const std::string &args);
  void lowerCmd(const std::string &args);
  void raiseCmd(const std::string &args);
  void testCmd (const std::string &args);

  void fitCmd   (const std::string &args);
  void updateCmd(const std::string &args);

  void bindCmd    (const std::string &args);
  void ifCmd      (int &i, const Statements &statements);
  void doCmd      (const std::string &args);
  void whileCmd   (const std::string &args);
  void evaluateCmd(const std::string &args);
  void pauseCmd   (const std::string &args);
  void rereadCmd  (const std::string &args);

  void readBlockLines(Statements &lines, std::string &eline, int depth);

  bool parseBoolExpression(CParseLine &line, bool &res);

  bool parseExpression(CParseLine &line, std::string &expr);

  bool parseFillStyle(CParseLine &line, CGnuPlotFillStyle &fillStyle);

  CGnuPlotPlot *addFunction2D(CGnuPlotGroup *group, const std::vector<std::string> &functions,
                              PlotStyle style);
  CGnuPlotPlot *addFunction3D(CGnuPlotWindowP window, const std::string &str, PlotStyle style);

  Plots addFile2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                  const UsingCols &usingCols, const Index &index=Index(),
                  const Every &every=Every());

  CGnuPlotPlot *addImage2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                           const UsingCols &usingCols);

  CGnuPlotPlot *addFile3D(CGnuPlotWindowP window, const std::string &filename);

  void parseFileLine(const std::string &str, std::vector<std::string> &fields);

  bool parseAxisRange(CParseLine &line, AxisData &axis, bool hasArgs=true);

  bool parseRange(CParseLine &line, std::vector<std::string> &fields);

  void decodeRange(const std::vector<std::string> &xfields, AxisData &axis);

  CExprValueP decodeUsingCol(int i, const CGnuPlot::UsingCol &col,
                             int setNum, int pointNum, bool &skip);

  CExprValueP evaluateExpression(const std::string &expr) const;

  CExprCTokenStack compileExpression(const std::string &expr) const;

  CExprValueP getFieldValue(int i, int ival, int setNum, int pointNum, bool &skip);

  void setSeparator(Separator sep) { separator_ = sep; }
  const Separator &getSeparator() const { return separator_; }

  void setCommentChars(const std::string &chars) { commentChars_ = chars; }
  const std::string &getCommentChars() const { return commentChars_; }

  void setMissingStr(const std::string &chars) { missingStr_ = chars; }
  const std::string &getMissingStr() const { return missingStr_; }

  void setPrintFile(const std::string &file) { printFile_ = file; }
  const std::string &getPrintFile() const { return printFile_; }

  void setPrintAppend(bool b) { printAppend_ = b; }
  bool getPrintAppend() { return printAppend_; }

  void setTableFile(const std::string &file) { tableFile_ = file; }
  const std::string &getTableFile() const { return tableFile_; }

  void setAngleType(AngleType type);
  AngleType getAngleType() const;

  void setStyleIncrementType(StyleIncrementType s) { styleIncrement_.type = s; }
  StyleIncrementType getStyleIncrementType() { return styleIncrement_.type; }

  const Title &title() const { return title_; }
  void setTitle(const Title &t) { title_ = t; }

  void setLogScale(LogScale scale, int base) {
    logScale_[scale] = base;
  }

  void setDummyVars(const std::string &dummyVar1, const std::string &dummyVar2);
  void getDummyVars(std::string &dummyVar1, std::string &dummyVar2) const;

  void setSamples(int isamples1, int isamples2) {
    isamples1_ = isamples1;
    isamples2_ = isamples2;
  }

  void getSamples(int &isamples1, int &isamples2) const {
    isamples1 = isamples1_;
    isamples2 = isamples2_;
  }

  void setIsoSamples(int isamples1, int isamples2) {
    isoSamples1_ = isamples1;
    isoSamples2_ = isamples2;
  }

  void getIsoSamples(int &isamples1, int &isamples2) const {
    isamples1 = isoSamples1_;
    isamples2 = isoSamples2_;
  }

  bool parseAxesTics(CParseLine &line, AxisData& axis);

  bool parseColor(CParseLine &line, CRGBA &c);
  bool parseColorSpec(CParseLine &line, CGnuPlotColorSpec &c);

  bool parseInteger(CParseLine &line, int &i);
  bool parseReal(CParseLine &line, double &r);
  bool parseString(CParseLine &line, std::string &str, const std::string &msg="");

  bool getIntegerVariable(const std::string &name, int &value);

  bool parsePosition(CParseLine &line, CGnuPlotPosition &pos);
  bool parseCoordReal(CParseLine &line, CGnuPlotCoordValue &v);
  bool parseSize(CParseLine &line, CSize2D &size);

  const Multiplot &multiplot() const { return multiplot_; }
  Multiplot &multiplot() { return multiplot_; }
  void setMultiplot(const Multiplot &m) { multiplot_ = m; }

  bool hidden3D() const { return hidden3D_; }
  void setHidden3D(bool b) { hidden3D_ = b; }

  bool surface3D() const { return surface3D_; }
  void setSurface3D(bool b) { surface3D_ = b; }

  bool contour3D() const { return contour3D_; }
  void setContour3D(bool b) { contour3D_ = b; }

  bool pm3D() const { return pm3D_; }
  void setPm3D(bool b) { pm3D_ = b; }

  bool parseFunction(CParseLine &line, std::string &function);

  bool readIdentifier(CParseLine &line, std::string &identifier);

  std::string readNonSpace(CParseLine &line);
  std::string readNonSpaceNonComma(CParseLine &line);
  std::string readNonSpaceNonChar(CParseLine &line, char c);
  std::string readNonSpaceNonChar(CParseLine &line, const std::string &c);
  std::string readName(CParseLine &line);

  bool fileReadLine(std::string &line);

 private:
  typedef std::map<std::string,CGnuPlotDevice*> Devices;
  typedef std::vector<CGnuPlotWindowP>          Windows;
  typedef std::vector<std::string>              Fields;
  typedef std::vector<std::string>              FileLines;
  typedef std::deque<std::string>               BufferLines;
  typedef CAutoPtr<CGnuPlotReadLine>            ReadLineP;
  typedef std::map<std::string,std::string>     DummyVarMap;

  bool                debug_  { false };
  bool                edebug_ { false };
  CGnuPlotSVGDevice*  svgDevice_ { 0 };
  CGnuPlotDevice*     device_ { 0 };
  Devices             devices_;
  CUnixFile*          file_ { 0 };
  int                 fileLineNum_ { 0 };
  BufferLines         bufferLines_;
  FileLines           fileLines_;
  Windows             windows_;
  Separator           separator_;
  std::string         commentChars_ { "#" };
  std::string         missingStr_;
  BoxWidth            boxWidth_;
  BarSize             barSize_;
  BoxPlot             boxPlot_;
  std::string         outputFile_;
  std::string         printFile_;
  std::string         lastPlotCmd_;
  bool                printAppend_ { false };
  std::string         tableFile_;
  CISize2D            terminalSize_   { 600, 600 };                  // terminal size
  PlotStyle           dataStyle_      { PlotStyle::POINTS };
  PlotStyle           functionStyle_  { PlotStyle::LINES };
  Smooth              smooth_         { Smooth::NONE };
  HistogramStyle      histogramStyle_ { HistogramStyle::CLUSTERED };
//AngleType           angleType_      { AngleType::RADIANS };
  CGnuPlotFillStyle   fillStyle_;
  CGnuPlotLineStyleP  lineStyle_;
  PointStyle          pointStyle_;
  LineStyles          lineStyles_;
  StyleIncrement      styleIncrement_;
  Title               title_;
  VarPrefs            varPrefs_;
  AxesData            axesData_;
  CGnuPlotKeyData     keyData_;
  CBBox2D             region_ { 0, 0, 1, 1 };
  CRange2D            margin_ { 10, 10, 10, 10 };
  CRGBA               backgroundColor_ { 1, 1, 1};
  int                 xind_ { 1 };
  int                 yind_ { 1 };
  Arrows              arrows_;
  ArrowStyles         arrowStyles_;
  CGnuPlotArrowStyle  arrowStyle_;
  LineDashes          lineDashes_;
  bool                binary_ { false };
  bool                matrix_ { false };
  bool                clip_ { false };
  bool                parametric_ { false };
  bool                polar_ { false };
  ImageStyle          imageStyle_;
  Labels              labels_;
  Rectangles          rects_;
  Ellipses            ellipses_;
  Polygons            polygons_;
  Palette             palette_;
  Camera              camera_;
  FilledCurve         filledCurve_;
  std::string         timeFmt_;
  LogScaleMap         logScale_;
  DummyVarMap         dummyVars_;
  int                 isamples1_   { 100 };
  int                 isamples2_   { 100 };
  int                 isoSamples1_ {  10 };
  int                 isoSamples2_ {  10 };
  PlotSize            plotSize_;
  Multiplot           multiplot_;
  CGnuPlotWindowP     multiWindow_;
  COptValT<CBBox2D>   clearRect_;
  bool                hidden3D_  { false };
  bool                surface3D_ {  true };
  bool                contour3D_ { false };
  bool                pm3D_      { false };
  int                 trianglePattern3D_ { 3 };
  double              whiskerBars_ { 0 };
  ReadLineP           readLine_;
  mutable Fields      fields_;
};

#endif
