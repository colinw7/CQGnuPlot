#ifndef CGnuPlot_H
#define CGnuPlot_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <climits>
#include <sys/types.h>

#include <CExpr.h>
#include <CFont.h>
#include <CAutoPtr.h>
#include <CStrUtil.h>
#include <COptVal.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <CCoordFrame3D.h>
#include <CMatrix3DH.h>
#include <CRGBA.h>
#include <CAlignType.h>
#include <NaN.h>

#define ACCESSOR(T,V,G,S) \
const T &G() const { return V; } \
void S(const T &t) { V = t; }

class CGnuPlotWindow;
class CGnuPlotPlot;
class CGnuPlotReadLine;
class CParseLine;

typedef CRefPtr<CExprValue> CExprValuePtr;

//------

#include <CGnuPlotLineStyle.h>
#include <CGnuPlotObject.h>

//------

class CGnuPlot {
 public:
  enum class CommandName {
    NONE,
    EXIT,
    HELP,
    HISTORY,
    PRINT,
    QUIT,
    CD,
    PWD,
    CALL,
    LOAD,
    SAVE,
    PLOT,
    REPLOT,
    SPLOT,
    SET,
    SHOW,
    RESET,
    UNSET,
    SHELL,
    SYSTEM,
    CLEAR,
    LOWER,
    RAISE,
    TEST,
    FIT,
    UPDATE,
    BIND,
    IF,
    PAUSE,
    REREAD
  };

  enum class VariableName {
    NONE,

    DATAFILE,
    DECIMALSIGN,
    ENCODING,
    FIT,
    FONTPATH,
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
    YRANGE,
    TICS,
    XTICS,
    YTICS,
    X2TICS,
    Y2TICS,
    MXTICS,
    TICSCALE,
    ZEROAXIS,

    LOCALE,
    TIMEFMT,
    XDATA,
    YDATA,
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

  enum class ChangeState {
    PLOT_ADDED,
    AXIS_DISPLAY,
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
    INDEX,
    EVERY,
    USING,
    AXES,
    TITLE,
    NOTITLE,
    SMOOTH,
    WITH,
    POINTSIZE
  };

  enum class PlotStyle {
    NONE,
    LINES,
    POINTS,
    LINES_POINTS,
    DOTS,
    IMPULSES,
    LABELS,
    STEPS,
    FSTEPS,
    HISTEPS,
    FINANCEBARS,
    ERRORBARS,
    ERRORLINES,
    XERRORBARS,
    XERRORLINES,
    YERRORBARS,
    YERRORLINES,
    XYERRORBARS,
    XYERRORLINES,
    VECTORS,
    BOXES,
    BOXERRORBARS,
    BOXXYERRORBARS,
    BOXPLOT,
    CANDLESTICKS,
    FILLEDCURVES,
    HISTOGRAMS,
    IMAGE,
    RGBIMAGE,
    RGBALPHA,
    CIRCLES,
    ELLIPSES,
    PM3D,
    TEST_TERMINAL,
    TEST_PALETTE
  };

  enum class SeparatorType {
    WHITESPACE,
    CHAR
  };

  enum class FillType {
    EMPTY,
    SOLID,
    PATTERN
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

  enum class FillPattern {
    NONE,
    HATCH,
    DENSE,
    FG,
    FDIAG,
    BDIAG,
    FDIAG1,
    BDIAG1,
    BG,
    LAST=BG
  };

  enum class ObjectType {
    CIRCLE,
    ELLIPSE,
    POLYGON,
    RECTANGLE
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

  class FillStyle {
   public:
    FillStyle()  { }

    const FillType &style() const { return style_; }
    void setStyle(FillType style) { style_ = style; }

    double density() const { return density_; }
    void setDensity(double d) { density_ = d; }

    const FillPattern &pattern() const { return pattern_; }
    void setPattern(FillPattern p) { pattern_ = p; }

    bool border() const { return border_; }
    void setBorder(bool b) { border_ = b; }

    int borderLine() const { return borderLine_; }
    void setBorderLine(int b) { borderLine_ = b; }

   private:
    FillType    style_     { FillType::EMPTY };
    double      density_   { 1.0 };
    FillPattern pattern_   { FillPattern::NONE };
    bool        border_    { true };
    int         borderLine_{ -1};
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

  struct Title {
    Title() { }

    std::string str;
    double      offset { 0 };
    std::string font;
    CRGBA       color;
    bool        enhanced { false };
  };

  //---

  struct AxisData {
    typedef std::map<int,std::string> TicLabelMap;

    AxisData(int i) : ind(i) { }

    int         ind;
    bool        displayed { true };
    bool        grid      { false };
    bool        mirror    { true };
    bool        isTime    { false };
    COptReal    min;
    COptReal    max;
    std::string str;
    TicLabelMap ticlabel;
    double      offset    { 0 };
    std::string format;
    bool        showTics  { true };
  };

  struct AxesData {
    AxesData() { }

    AxisData xaxis       { 1 };
    AxisData yaxis       { 1 };
    AxisData zaxis       { 1 };
    AxisData x2axis      { 2 };
    AxisData y2axis      { 2 };
    int      borders     { 0xFF };
    double   borderWidth { 1.0 };
    COptInt  borderStyle;
    COptInt  borderType;
  };

  struct StyleIncrement {
    StyleIncrement() { }

    StyleIncrementType type     { StyleIncrementType::USER };
    int                colorInd { 1 };
  };

  //---

  struct KeyData {
    KeyData() { }

    typedef std::vector<std::string> Columns;

    bool        displayed  { true };               // key displayed
    CHAlignType halign     { CHALIGN_TYPE_RIGHT }; // key horizontal side
    CVAlignType valign     { CVALIGN_TYPE_TOP   }; // key vertical side
    bool        vertical   { true };               // ??
    bool        right      { true };               // text justification
    bool        reverse    { false };              // reverse text and sample
    bool        invert     { false };              // invert plot order
    bool        autotitle  { true };
    COptReal    sampLen;
    COptReal    spacing;
    std::string title;
    bool        box { false };
    bool        columnhead { false };
    COptInt     columnNum;
    Columns     columns;
  };

  //---

  struct Position {
    CPoint2D p;

    Position(double x=0.0, double y=0.0) : p(x, y) { }

    Position(const CPoint2D &p1) : p(p1) { }
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

  typedef CGnuPlotArrow::ArrowStyle ArrowStyle;
  typedef std::map<int,ArrowStyle>  ArrowStyles;

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

 public:
  CGnuPlot();

  virtual ~CGnuPlot();

  void setDebug(bool b);
  bool isDebug() const { return debug_; }

  void setExprDebug(bool b);
  bool isExprDebug() const { return edebug_; }

  PlotStyle getDataStyle() const { return dataStyle_; }
  void setDataStyle(PlotStyle style) { dataStyle_ = style; }

  PlotStyle getFunctionStyle() const { return functionStyle_; }
  void setFunctionStyle(PlotStyle style) { functionStyle_ = style; }

  const FillStyle &fillStyle() const { return fillStyle_; }
  FillStyle &fillStyle() { return fillStyle_; }
  void setFillStyle(const FillStyle &s) { fillStyle_ = s; }

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

  const AxesData &axesData() const { return axesData_; }
  AxesData &axesData() { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  const KeyData &keyData() const { return keyData_; }
  KeyData &keyData() { return keyData_; }
  void setKeyData(const KeyData &k) { keyData_ = k; }

  const CRange2D &margin() const { return margin_; }
  CRange2D &margin() { return margin_; }
  void setMargin(const CRange2D &b) { margin_ = b; }

  int xind() const { return xind_; }
  int yind() const { return yind_; }

  const PlotSize &plotSize() const { return plotSize_; }
  PlotSize &plotSize() { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

  void getXRange(double *xmin, double *xmax) {
    *xmin = axesData().xaxis.min.getValue(-10);
    *xmax = axesData().xaxis.max.getValue(10);
  }
  void getYRange(double *ymin, double *ymax) {
    *ymin = axesData().yaxis.min.getValue(-1);
    *ymax = axesData().yaxis.max.getValue(1);
  }
  void getZRange(double *zmin, double *zmax) {
    *zmin = axesData().zaxis.min.getValue(-1);
    *zmax = axesData().zaxis.max.getValue(1);
  }

  void setBoxWidth(const BoxWidth &w) { boxWidth_ = w; }
  const BoxWidth &getBoxWidth() const { return boxWidth_; }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const Palette &palette() const { return palette_; }
  void setPalette(const Palette &p) { palette_ = p; }

  const LogScaleMap &logScaleMap() const { return logScale_; }

  int trianglePattern3D() const { return trianglePattern3D_; }
  void setTrianglePattern3D(int n) { trianglePattern3D_ = n; }

  const Camera &camera() const { return camera_; }
  Camera &camera() { return camera_; }
  void setCamera(const Camera &c) { camera_ = c; }

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

  void setFitX(bool b) { fitX_ = b; }
  bool getFitX() const { return fitX_; }

  void setFitY(bool b) { fitY_ = b; }
  bool getFitY() const { return fitY_; }

  void setFitZ(bool b) { fitZ_ = b; }
  bool getFitZ() const { return fitZ_; }

  bool load(const std::string &filename);

  void loop();

  virtual CGnuPlotWindow *createWindow();

  virtual CGnuPlotPlot *createPlot(CGnuPlotWindow *window);

  virtual CGnuPlotLineStyle *createLineStyle();

  virtual CGnuPlotArrow     *createArrow();
  virtual CGnuPlotLabel     *createLabel();
  virtual CGnuPlotEllipse   *createEllipse();
  virtual CGnuPlotPolygon   *createPolygon();
  virtual CGnuPlotRectangle *createRectangle();

  virtual void timeout() { }

  std::string getField(int i) const {
    if (i < 1 || i > int(fields_.size())) return "";
    return fields_[i - 1];
  }

  void incLineStyle();

  void drawWindows();

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

  void plotCmd  (const std::string &args);
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

  CGnuPlotPlot *addFunction2D(CGnuPlotWindow *window, const std::string &str,
                              const CBBox2D &region, PlotStyle style);
  CGnuPlotPlot *addFunction3D(CGnuPlotWindow *window, const std::string &str, PlotStyle style);

  Plots addFile2D(CGnuPlotWindow *window, const std::string &filename,
                  const CBBox2D &region, PlotStyle style,
                  const UsingCols &usingCols, const Index &index=Index(),
                  const Every &every=Every());

  CGnuPlotPlot *addFile3D(CGnuPlotWindow *window, const std::string &filename);

  CExprValuePtr decodeUsingCol(int i, const CGnuPlot::UsingCol &col,
                               int setNum, int pointNum, bool &skip);

  CExprValuePtr getFieldValue(int i, int ival, int setNum, int pointNum, bool &skip);

  void setSeparator(Separator sep) { separator_ = sep; }
  const Separator &getSeparator() const { return separator_; }

  void setCommentChars(const std::string &chars) { commentChars_ = chars; }
  const std::string &getCommentChars() const { return commentChars_; }

  void setMissingStr(const std::string &chars) { missingStr_ = chars; }
  const std::string &getMissingStr() const { return missingStr_; }

  void setOutputFile(const std::string &file) { outputFile_ = file; }
  const std::string &getOutputFile() const { return outputFile_; }

  void setPrintFile(const std::string &file) { printFile_ = file; }
  const std::string &getPrintFile() const { return printFile_; }

  void setPrintAppend(bool b) { printAppend_ = b; }
  bool getPrintAppend() { return printAppend_; }

  void setTableFile(const std::string &file) { tableFile_ = file; }
  const std::string &getTableFile() const { return tableFile_; }

  void setAngleType(AngleType type);
  AngleType getAngleType() { return angleType_; }

  void setStyleIncrementType(StyleIncrementType s) { styleIncrement_.type = s; }
  StyleIncrementType getStyleIncrementType() { return styleIncrement_.type; }

  const Title &title() const { return title_; }
  Title &title() { return title_; }
  void setTitle(const Title &t) { title_ = t; }

  void setLogScale(LogScale scale, int base) {
    logScale_[scale] = base;
  }

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
  bool parseColorSpec(CParseLine &line, CRGBA &c);

  bool parseInteger(CParseLine &line, int &i);
  bool parseReal(CParseLine &line, double &r);
  bool parseString(CParseLine &line, std::string &filename, const std::string &msg="");

  bool parsePosition(CParseLine &line, Position &pos);
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

  std::string readIdentifier(CParseLine &line);

  std::string readNonSpace(CParseLine &line);
  std::string readNonSpaceNonComma(CParseLine &line);

 private:
  typedef std::vector<CGnuPlotWindow *> Windows;
  typedef std::vector<std::string>      Fields;
  typedef CAutoPtr<CGnuPlotReadLine>    ReadLineP;

  bool               debug_  { false };
  bool               edebug_ { false };
  Windows            windows_;
  Separator          separator_;
  std::string        commentChars_ { "#" };
  std::string        missingStr_;
  BoxWidth           boxWidth_;
  std::string        outputFile_;
  std::string        printFile_;
  bool               printAppend_ { false };
  std::string        tableFile_;
  PlotStyle          dataStyle_      { PlotStyle::POINTS };
  PlotStyle          functionStyle_  { PlotStyle::LINES };
  Smooth             smooth_         { Smooth::NONE };
  HistogramStyle     histogramStyle_ { HistogramStyle::CLUSTERED };
  AngleType          angleType_      { AngleType::RADIANS };
  FillStyle          fillStyle_;
  CGnuPlotLineStyleP lineStyle_;
  PointStyle         pointStyle_;
  LineStyles         lineStyles_;
  StyleIncrement     styleIncrement_;
  Title              title_;
  AxesData           axesData_;
  KeyData            keyData_;
  CRange2D           margin_ { 10, 10, 10, 10 };
  int                xind_ { 1 };
  int                yind_ { 1 };
  Arrows             arrows_;
  ArrowStyles        arrowStyles_;
  Labels             labels_;
  Rectangles         rects_;
  Ellipses           ellipses_;
  Polygons           polygons_;
  Palette            palette_;
  Camera             camera_;
  bool               fitX_ { false };
  bool               fitY_ { false };
  bool               fitZ_ { false };
  std::string        timeFmt_;
  LogScaleMap        logScale_;
  std::string        dummyVar1_;
  std::string        dummyVar2_;
  int                isamples1_   { 100 };
  int                isamples2_   { 100 };
  int                isoSamples1_ {  10 };
  int                isoSamples2_ {  10 };
  PlotSize           plotSize_;
  Multiplot          multiplot_;
  CGnuPlotWindow*    multiWindow_ { 0 };
  COptValT<CBBox2D>  clearRect_;
  bool               hidden3D_  { false };
  bool               surface3D_ {  true };
  bool               contour3D_ { false };
  bool               pm3D_      { false };
  int                trianglePattern3D_ { 3 };
  ReadLineP          readLine_;
  mutable Fields     fields_;
};

#endif
