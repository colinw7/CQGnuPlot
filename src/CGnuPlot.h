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
#include <CAngle.h>
#include <NaN.h>

#define ACCESSOR(T,V,G,S) \
const T &G() const { return V; } \
void S(const T &t) { V = t; }

class CGnuPlotAxis;
class CGnuPlotDevice;
class CGnuPlotGroup;
class CGnuPlotKey;
class CGnuPlotColorBox;
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
#include <CGnuPlotPointStyle.h>
#include <CGnuPlotCoordValue.h>
#include <CGnuPlotKeyData.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotSize.h>
#include <CGnuPlotAxisData.h>
#include <CGnuPlotTitle.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotArrow.h>
#include <CGnuPlotCircle.h>
#include <CGnuPlotEllipse.h>
#include <CGnuPlotLabel.h>
#include <CGnuPlotPolygon.h>
#include <CGnuPlotRectangle.h>
#include <CGnuPlotFile.h>
#include <CGnuPlotCamera.h>
#include <CGnuPlotPalette.h>
#include <CGnuPlotColorBox.h>
#include <CGnuPlotUsingCol.h>
#include <CGnuPlotImageStyle.h>
#include <CGnuPlotHistogramData.h>
#include <CGnuPlotMultiplot.h>
#include <CGnuPlotPrefValue.h>
#include <CGnuPlotBoxWidth.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotBoxPlot.h>

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
    GET,
    HELP,
    HISTORY,
    IF,
    LOAD,
    LOWER,
    PAUSE,
    PLOT,
    PRINT,
    PRINTF,
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

    ANGLES,
    ARROW,
    AUTOSCALE,
    BARS,
    BIND,
    BMARGIN,
    BORDER,
    BOXWIDTH,
    CBLABEL,
    CBRANGE,
    CBTICS,
    CIRCLE,
    CLIP,
    CNTRLABEL,
    CNTRPARAM,
    COLORBOX,
    COLORNAMES,
    COLORSEQUENCE,
    CONTOUR,
    CPLANE,
    DASHTYPE,
    DATAFILE,
    DEBUG,
    DECIMALSIGN,
    DGRID3D,
    DUMMY,
    EDEBUG,
    ELLIPSE,
    ENCODING,
    FIT,
    FONTPATH,
    FORMAT,
    FUNCTIONS,
    GRID,
    HIDDEN3D,
    HISTORY,
    HISTORYSIZE,
    ISOSAMPLES,
    KEY,
    LABEL,
    LINETYPE,
    LINK,
    LMARGIN,
    LOADPATH,
    LOCALE,
    LOGSCALE,
    MACROS,
    MAPPING,
    MARGINS,
    MOUSE,
    MULTIPLOT,
    MX2TICS,
    MXTICS,
    MY2TICS,
    MYTICS,
    MZTICS,
    OBJECT,
    OFFSETS,
    ORIGIN,
    OUTPUT,
    PALETTE,
    PARAMETRIC,
    PAXIS,
    PM3D,
    POINTINTERVALBOX,
    POINTSIZE,
    POLAR,
    POLYGON,
    PRINT,
    RAXIS,
    RECTANGLE,
    RMARGIN,
    RRANGE,
    RTICS,
    SAMPLES,
    SIZE,
    STYLE,
    SURFACE,
    TABLE,
    TERMINAL,
    TERMOPTION,
    TICS,
    TICSCALE,
    TIMEFMT,
    TIMESTAMP,
    TITLE,
    TMARGIN,
    TRANGE,
    VARIABLES,
    VERSION,
    VIEW,
    X2DATA,
    X2LABEL,
    X2RANGE,
    X2TICS,
    XDATA,
    XDTICS,
    XLABEL,
    XMTICS,
    XRANGE,
    XTICS,
    XYPLANE,
    Y2DATA,
    Y2LABEL,
    Y2RANGE,
    Y2TICS,
    YDATA,
    YLABEL,
    YRANGE,
    YTICS,
    ZERO,
    ZEROAXIS,
    ZLABEL,
    ZRANGE,
    ZTICS,
  };

  enum class DataFileVar {
    NONE,
    FORTRAN,
    NOFPE_TRAP,
    MISSING,
    SEPARATOR,
    COMMENTS_CHARS,
    BINARY,
  };

  enum class StyleVar {
    NONE,
    ARROW,
    BOXPLOT,
    CIRCLE,
    DATA,
    ELLIPSE,
    FILL,
    FUNCTION,
    HISTOGRAM,
    INCREMENT,
    LINE,
    RECTANGLE,
    TEXTBOX
  };

  enum class StyleIncrementType {
    USER,
    DEFAULT
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
    BUBBLECHART,
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

  enum class TestType {
    NONE,
    TERMINAL,
    PALETTE
  };

  typedef CGnuPlotTypes::Smooth         Smooth;
  typedef CGnuPlotTypes::HistogramStyle HistogramStyle;
  typedef CGnuPlotTypes::LogScale       LogScale;
  typedef std::map<LogScale,int>        LogScaleMap;
  typedef CGnuPlotTypes::AngleType      AngleType;
  typedef CGnuPlotTypes::DrawLayer      DrawLayer;

  //---

  typedef std::map<int,CGnuPlotLineStyleP> LineStyles;

  //---

  struct Bars {
    Bars(double s=1, bool f=true) :
     size(s), front(f) {
    }

    double size { 1 };
    bool   front { true };

    void show(std::ostream &os) const {
      if (size <= 0.0)
        os << "errors are plotted without bars" << std::endl;
      else
        os << "errorbars are plotted in " << (front ? "front" : "back") <<
              " with bars of size " << size << std::endl;
    }

    void save(std::ostream &os) const {
      os << "set bar " << size << " " << (front ? "front" : "back") << std::endl;
    }
  };

  //---

  typedef std::map<int,CGnuPlotAxisData> IAxisMap;

  struct BorderData {
    int       sides     { 0xFF };
    DrawLayer layer     { DrawLayer::FRONT };
    double    lineWidth { 1.0 };
    COptInt   lineStyle;
    COptInt   lineType;

    void unset() {
      sides = 0;
    }

    void show(std::ostream &os) const;
    void save(std::ostream &os) const;
  };

  struct GridData {
    bool      enabled        { false };
    DrawLayer layer          { DrawLayer::DEFAULT };
    double    polarAngle     { 0 };
    int       majorLineStyle { -1 };
    int       minorLineStyle { -1 };
    int       majorLineType  { -1 };
    int       minorLineType  { -1 };
    double    majorLineWidth { 1.0 };
    double    minorLineWidth { 1.0 };
  };

  struct AxesData {
    AxesData() { }

    GridData         grid;
    IAxisMap         xaxis;
    IAxisMap         yaxis;
    IAxisMap         zaxis;
    IAxisMap         paxis;
    IAxisMap         taxis;
    CGnuPlotAxisData cbaxis;
    CGnuPlotAxisData raxis;
    BorderData       border;
  };

  struct StyleIncrement {
    StyleIncrement() { }

    StyleIncrementType type      { StyleIncrementType::USER };
    int                styleInd  { 0 };
    int                increment { 8 };
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

  //---

  typedef std::map<int,CGnuPlotArrowStyle>      ArrowStyles;
  typedef std::map<int,CLineDash>               LineDashes;
  typedef std::vector<CGnuPlotGroupAnnotationP> Annotations;

  //---

  struct MarginValue {
    COptReal value;
    bool     screen { false };

    MarginValue() { }

    MarginValue(double v, bool s=false) { value = v; screen = s; }

    void setValue(double v, bool b) { value = v; screen = b; }

    void resetValue() { value.setInvalid(); screen = false; }

    void show(std::ostream &os, const std::string &id) {
      os << id << " is ";

      if (! value.isValid())
        os << "computed automatically" << std::endl;
      else {
        os << "set to";

        if (screen)
          os << " screen";

        os << " " << value.getValue() << std::endl;
      }
    }
  };

  struct Margin {
    Margin() { }

    Margin(double l, double b, double r, double t) :
     lmargin(l), bmargin(b), rmargin(r), tmargin(t) {
    }

    const COptReal &left  () const { return lmargin.value; }
    const COptReal &bottom() const { return bmargin.value; }
    const COptReal &right () const { return rmargin.value; }
    const COptReal &top   () const { return tmargin.value; }

    void setLeft  (double l, bool s=false) { lmargin.setValue(l, s); }
    void setBottom(double b, bool s=false) { bmargin.setValue(b, s); }
    void setRight (double r, bool s=false) { rmargin.setValue(r, s); }
    void setTop   (double t, bool s=false) { tmargin.setValue(t, s); }

    void resetLeft  () { lmargin.resetValue(); }
    void resetBottom() { bmargin.resetValue(); }
    void resetRight () { rmargin.resetValue(); }
    void resetTop   () { tmargin.resetValue(); }

    CRange2D range() const {
      return CRange2D(left ().getValue(10), bottom().getValue(10),
                      right().getValue(10), top   ().getValue(10));
    }

    void showLeft  (std::ostream &os) { lmargin.show(os, "lmargin"); }
    void showBottom(std::ostream &os) { bmargin.show(os, "bmargin"); }
    void showRight (std::ostream &os) { rmargin.show(os, "rmargin"); }
    void showTop   (std::ostream &os) { tmargin.show(os, "tmargin"); }

    MarginValue lmargin;
    MarginValue bmargin;
    MarginValue rmargin;
    MarginValue tmargin;
  };

  //---

  typedef std::deque<std::string> BufferLines;

  struct FileData {
    CUnixFile*  file { 0 };
    int         lineNum { 0 };
    BufferLines bufferLines;
  };

  struct FunctionData {
    std::string function;
    bool        isAssign = false;
    std::string assign;
  };

  struct HistoryData {
    int  size    { 500 };
    bool numbers { true };
    bool trim    { true };
  };

  struct Samples {
    int samples1 { 100 };
    int samples2 { 100 };

    void getValues(int &s1, int &s2) const { s1 = samples1; s2 = samples2; }
    void setValues(int s1, int s2) { samples1 = s1; samples2 = s2; }

    void show(std::ostream &os) const {
      os << "set samples " << samples1 << ", " << samples2 << std::endl;
    }

    void print(std::ostream &os) {
      os << "sampling rate is " << samples1 << ", " << samples2 << std::endl;
    }
  };

  struct LinkAxisData {
    bool        enabled { false };
    std::string expr;
    std::string iexpr;
  };

  struct LinkData {
    LinkAxisData linkX2;
    LinkAxisData linkY2;
  };

  class ISOSamples {
   public:
    ISOSamples() { }

    void getValues(int &s1, int &s2) const { s1 = samples1_; s2 = samples2_; }
    void setValues(int s1, int s2) { samples1_ = s1; samples2_ = s2; }

    void unset() { setValues(10, 10); }

    void save(std::ostream &os) const {
      os << "set isosamples " << samples1_ << ", " << samples2_ << std::endl;
    }

    void show(std::ostream &os) const {
      os << "iso sampling rate is " << samples1_ << ", " << samples2_ << std::endl;
    }

   private:
    int samples1_ { 10 };
    int samples2_ { 10 };
  };

  struct Clip {
    bool points { false };
    bool one    { true  };
    bool two    { false };

    bool isOn() const { return (points || one || two); }
  };

  struct DecimalSign {
    void setChar(char c1) { c = c1; }

    void setLocale(const std::string &str) { locale = str; }

    char        c { '\0' };
    std::string locale;
  };

  class Offsets {
   public:
    void setValues(const CGnuPlotCoordValue &l, const CGnuPlotCoordValue &r,
                   const CGnuPlotCoordValue &t, const CGnuPlotCoordValue &b) {
      l_ = l; r_ = r;
      t_ = t; b_ = b;
    }

    void show(std::ostream &os) {
      os << "offsets are " << l_ << ", " << r_ << ", " << t_ << ", " << b_ << std::endl;
    }

    void unset() {
      l_ = CGnuPlotCoordValue();
      r_ = CGnuPlotCoordValue();
      t_ = CGnuPlotCoordValue();
      b_ = CGnuPlotCoordValue();
    }

   private:
    CGnuPlotCoordValue l_, r_, t_, b_;
  };

  struct FitData {
    std::string logfile { false };
    bool        quiet { false };
    bool        results { false };
    bool        brief { false };
    bool        verbose { false };
    bool        errorvariables { false };
    bool        covariancevariables { false };
    bool        errorscaling { false };
    bool        prescale { false };
    int         maxiter { -1 };
    double      limit { 0 };
    double      limit_abs { 0 };
    double      start_lambda { 0 };
    double      lambda_factor { 0 };
    std::string script;
    int         version { 5 };
  };

  class MouseData {
   public:
    enum class PolarDistanceType {
      NONE,
      DEG,
      TAN
    };

   public:
    bool enabled() const { return enabled_; }
    void setEnabled(bool b) { enabled_ = b; }

    void setDoubleClick(double ms) { dclick_ = ms; }
    void resetDoubleClick() { dclick_.setInvalid(); }

    void setZoomCoordinates(bool b) { zoomCoords_ = b; }

    void setZoomFactors(double x, double y) { zoomX_ = x; zoomY_ = y; }

    void setRulerPos(const CPoint2D &p) { rulerPos_ = p; }
    void resetRulerPos() { rulerPos_.setInvalid(); }

    void setPolarDisance(PolarDistanceType type) { polarDistType_ = type; }

    void setFormat(const std::string &fmt) { format_ = fmt; }

    void setMouseFormat(const std::string &fmt) { mouseFormatStr_ = fmt; }
    void setMouseFormat(int i) { mouseFormatInt_ = i; }

    void setLabels(const std::string &str) { labels_ = str; }
    void resetLabels() { labels_ = ""; }

    bool zoomJump() const { return zoomJump_; }
    void setZoomJump(bool b) { zoomJump_ = b; }

    bool verbose() const { return verbose_; }
    void setVerbose(bool b) { verbose_ = b; }

    void show(std::ostream &os) {
      if (enabled_) {
        os << "mouse is on" << std::endl;

        if (zoomCoords_)
          os << "zoom coordinates will be drawn" << std::endl;
        else
          os << "no zoom coordinates will be drawn" << std::endl;

        if (polarDistType_ == PolarDistanceType::NONE)
          os << "no polar distance to ruler will be shown" << std::endl;
        else
          os << "distance to ruler will be show in polar coordinates" << std::endl;

        if (dclick_.isValid())
          os << "double click resolution is " << dclick_.getValue() << " ms" << std::endl;
        else
          os << "double click resolution is 300 ms" << std::endl;

        if (format_.isValid())
          os << "formatting numbers with \"" << format_.getValue() << "\"" << std::endl;
        else
          os << "formatting numbers with \"%g\"" << std::endl;

        if (mouseFormatInt_.isValid())
          os << "format for Button 2 is " << mouseFormatInt_.getValue() << std::endl;
        else
          os << "format for Button 2 is 0" << std::endl;

        if (mouseFormatStr_ != "")
          os << "Button 2 draws persistent labels with options \"" <<
                mouseFormatStr_ << "\"" << std::endl;

        if (! zoomX_.isValid() || ! zoomY_.isValid())
          os << "zoom factors are x: 1   y: 1" << std::endl;
        else
          os << "zoom factors are x: " << zoomX_.getValue() <<
                "   y: " << zoomY_.getValue() << std::endl;

        if (zoomJump_)
          os << "zoomjump is on" << std::endl;
        else
          os << "zoomjump is off" << std::endl;

        if (verbose_)
          os << "communication commands will be shown" << std::endl;
        else
          os << "communication commands will not be shown" << std::endl;
      }
      else
        os << "mouse is off" << std::endl;
    }

   private:
    bool               enabled_        { true };
    COptReal           dclick_;
    bool               zoomCoords_     { true };
    COptReal           zoomX_;
    COptReal           zoomY_;
    COptValT<CPoint2D> rulerPos_;
    PolarDistanceType  polarDistType_  { PolarDistanceType::NONE };
    COptString         format_;
    std::string        mouseFormatStr_;
    COptInt            mouseFormatInt_;
    std::string        labels_;
    bool               zoomJump_       { false };
    bool               verbose_        { false };
  };

  struct Hidden3DData {
    bool      enabled         { false };
    DrawLayer layer           { DrawLayer::FRONT };
    COptReal  offset;
    int       trianglePattern { 3 };
    COptInt   undefined;
    bool      altdiagonal     { false };
    bool      bentover        { false };
  };

  struct Surface3DData {
    bool enabled { false };
  };

  struct Contour3DData {
    enum class DrawPos {
      DRAW_BASE,
      DRAW_SURFACE,
      DRAW_BOTH
    };

    bool    enabled { false };
    DrawPos pos     { DrawPos::DRAW_BASE };
  };

  //---

  typedef std::vector<CGnuPlotPlot *> Plots;
  typedef StringArray                 Statements;

  typedef std::map<CGnuPlot::VariableName,CGnuPlotPrefValueBase *> VarPrefs;

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

  const CGnuPlotPointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const CGnuPlotPointStyle &s) { pointStyle_ = s; }

  const LineStyles &lineStyles() const { return lineStyles_; }
  CGnuPlotLineStyleP lineStyle(int i) const;

  const CGnuPlotHistogramData &histogramData() { return histogramData_; }
  void setHistogramData(const CGnuPlotHistogramData &data) { histogramData_ = data; }

  const CGnuPlotArrowStyle &arrowStyle() const { return arrowStyle_; }
  void setArrowStyle(const CGnuPlotArrowStyle &as) { arrowStyle_ = as; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &r) { region_ = r; }

  const Margin &margin() const { return margin_; }
  void setMargin(const Margin &b) { margin_ = b; }

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  int xind() const { return xind_; }
  int yind() const { return yind_; }
  int zind() const { return zind_; }

  const PlotSize &plotSize() const { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

  void setMacros(bool b) { macros_ = b; }
  bool isMacros() const { return macros_; }

  //---

  const COptValT<CBBox2D> &clearRect() const { return clearRect_; }

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  void getXRange(double *xmin, double *xmax) {
    *xmin = axesData_.xaxis[1].min().getValue(-10);
    *xmax = axesData_.xaxis[1].max().getValue( 10);
  }
  void getYRange(double *ymin, double *ymax) {
    *ymin = axesData_.yaxis[1].min().getValue(-10);
    *ymax = axesData_.yaxis[1].max().getValue( 10);
  }
  void getZRange(double *zmin, double *zmax) {
    *zmin = axesData_.zaxis[1].min().getValue(-10);
    *zmax = axesData_.zaxis[1].max().getValue( 10);
  }
  void getTRange(double *tmin, double *tmax) {
    *tmin = axesData_.taxis[1].min().getValue(0);
    *tmax = axesData_.taxis[1].max().getValue(M_PI);
  }

  //---

  const Bars &bars() const { return bars_; }
  void setBars(const Bars &s) { bars_ = s; }

  double barsSize() const { return bars_.size; }
  void setBarsSize(double s) { bars_.size = s; }

  bool barsFront() const { return bars_.front; }
  void setBarsFront(bool b) { bars_.front = b; }

  //---

  const CGnuPlotBoxWidth &boxWidth() const { return boxWidth_; }
  void setBoxWidth(const CGnuPlotBoxWidth &w) { boxWidth_ = w; }

  //---

  void setBoxPlot(const CGnuPlotBoxPlot &b) { boxPlot_ = b; }
  const CGnuPlotBoxPlot &getBoxPlot() const { return boxPlot_; }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const CGnuPlotPalette &palette() const { return palette_; }
  void setPalette(const CGnuPlotPalette &p) { palette_ = p; }

  const CGnuPlotColorBox &colorBox() const { return colorBox_; }
  void setColorBox(const CGnuPlotColorBox &c) { colorBox_ = c; }

  const FilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const FilledCurve &c) { filledCurve_ = c; }

  const LogScaleMap &logScaleMap() const { return logScale_; }
  void setLogScale(LogScale scale, int base) { logScale_[scale] = base; }
  bool isLogScale(LogScale scale) const { return logScale_.find(scale) != logScale_.end(); }
  int getLogScale(LogScale scale) const { auto p = logScale_.find(scale); return (*p).second; }
  void resetLogScale() { logScale_.clear(); }

  double whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }

  const CGnuPlotCamera &camera() const { return camera_; }
  void setCamera(const CGnuPlotCamera &c) { camera_ = c; }

  //---

  const Hidden3DData &hidden3D() const { return hidden3D_; }
  void setHidden3D(const Hidden3DData &h) { hidden3D_ = h; }

  //---

  bool surface3D() const { return surface3D_.enabled; }
  void setSurface3D(bool b) { surface3D_.enabled = b; }

  //---

  bool contour3D() const { return contour3D_.enabled; }
  void setContour3D(bool b) { contour3D_.enabled = b; }

  //---

  bool pm3D() const { return pm3D_; }
  void setPm3D(bool b) { pm3D_ = b; }

  //------

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool m) { matrix_ = m; }

  const CGnuPlotImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const CGnuPlotImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  void setTimeFmt(const std::string &f) { timeFmt_ = f; }
  const std::string &timeFmt() { return timeFmt_; }

  //------

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  //------

  const Annotations &annotations() const { return annotations_; }
  void setAnnotations(const Annotations &annotations) { annotations_ = annotations; }

  //------

  void setOutputFile(const std::string &file);
  const std::string &getOutputFile() const { return outputFile_; }

  bool load(const std::string &filename, const StringArray &args=StringArray());

  bool exec(const std::string &cmd);

  void initReadLine();

  void loop();

  void prompt(const std::string &msg);

  CGnuPlotWindow *createWindow();

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group);

  CGnuPlotLineStyle *createLineStyle();

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir);
  CGnuPlotKey  *createKey (CGnuPlotGroup *group);

  CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group);

  CGnuPlotPalette *createPalette(CGnuPlotGroup *group);

  CGnuPlotTitle *createTitle(CGnuPlotGroup *group);

  CGnuPlotRenderer *renderer();

  //----

  template<typename T>
  T *getAnnotation(int ind) const {
    for (auto &ann : annotations_)
      if (ann->getInd() == ind && (dynamic_cast<T *>(ann.get()) != 0))
        return dynamic_cast<T *>(ann.get());

    return 0;
  }

  template<typename T>
  T *lookupAnnotation(int ind) {
    T *annotation = getAnnotation<T>(ind);

    if (! annotation) {
      // TODO: unique ind ?
      if (ind <= 0)
        ind = annotations_.size() + 1;

      annotation = new T(0);

      annotation->setInd(ind);

      annotations_.push_back(CGnuPlotGroupAnnotationP(annotation));
    }

    return annotation;
  }

  template<typename T>
  void showAnnotations(std::ostream &os=std::cout, int ind=-1) {
    for (const auto &ann : annotations_) {
      const T *obj = dynamic_cast<const T *>(ann.get());
      if (! obj) continue;

      if (ind != -1 && obj->getInd() != ind) continue;

      os << T::getName() << " " << obj->getInd();
      obj->print(os);
      os << std::endl;
    }
  }

  template<typename T>
  void clearAnnotations(int ind=-1) {
    Annotations annotations;

    for (auto &ann : annotations_) {
      T *obj = dynamic_cast<T *>(ann.get());

      if (obj) {
        if (ind != -1 && obj->getInd() != ind)
          annotations.push_back(ann);
      }
      else
        annotations.push_back(ann);
    }

    annotations_ = annotations;
  }

  //----

  void timeout();

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state);

  std::string getField(int i) const {
    if (i < 1 || i > int(fields_.size())) return "";
    return fields_[i - 1];
  }

  void setPointNum(int n) { pointNum_ = n; }
  int pointNum() const { return pointNum_; }

  CExprValueP getFieldValue(int i, int ival, int setNum, int pointNum, bool &skip);

  void resetLineStyle();

  void incLineStyle();

  void drawWindows();

  CGnuPlotArrowStyle arrowStyle(int id) const;

  void errorMsg(const std::string &msg) const;

 private:
  bool parseLine(const std::string &str);

  std::string replaceEmbedded(const std::string &str) const;

  bool execCmd(const std::string &cmd, StringArray &lines) const;

  bool parseStatement(int &i, const Statements &statements);

  void exitCmd   (const std::string &args);
  void helpCmd   (const std::string &args);
  void historyCmd(const std::string &args);
  void printCmd  (const std::string &args);
  void printfCmd (const std::string &args);
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

  bool parseModifiers2D(CParseLine &line, CGnuPlotLineStyle &lineStyle,
                        CGnuPlotFillStyle &fillStyle);
  bool parseModifiers3D(CParseLine &line);

  bool parseFor(CParseLine &line, std::string &var, std::string &start, std::string &end,
                std::string &inc, std::string &lcmd, std::string &rcmd);

  bool setCmd     (const std::string &args);
  bool getCmd     (const std::string &args);
  bool showCmd    (const std::string &args);
  void resetCmd   (const std::string &args);
  void undefineCmd(const std::string &args);
  bool unsetCmd   (const std::string &args);

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

  void showColorNames();
  void showVariables(std::ostream &os, const StringArray &args=StringArray());
  void showFunctions(std::ostream &os);

  void readBlockLines(Statements &lines, std::string &eline, int depth);

  void parseUsing(CParseLine &line, CGnuPlotUsingCols &usingCols);

  void parseIndex(CParseLine &line, int &indexStart, int &indexEnd, int &indexStep);
  void parseEvery(CParseLine &line, int &everyStart, int &everyEnd, int &everyStep);

  bool parseFont(CParseLine &line, CFontPtr &font);

  bool parseBoolExpression(CParseLine &line, bool &res);

  bool parseExpression(CParseLine &line, std::string &expr);

  bool parseFillStyle(CParseLine &line, CGnuPlotFillStyle &fillStyle);

  CGnuPlotPlot *addFunction2D(CGnuPlotGroup *group, const StringArray &functions, PlotStyle style);
  CGnuPlotPlot *addFunction3D(CGnuPlotWindowP window, const std::string &str, PlotStyle style);

  Plots addFile2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                  const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlot *addImage2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                           const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlot *addFile3D(CGnuPlotWindowP window, const std::string &filename);

  bool parseAxisRange(CParseLine &line, CGnuPlotAxisData &axis, bool hasArgs=true);

  bool parseRange(CParseLine &line, StringArray &fields);

  bool decodeRange(const StringArray &xfields, CGnuPlotAxisData &axis);

  CExprValueP decodeUsingCol(int i, const CGnuPlotUsingCol &col, int setNum,
                             int pointNum, bool &skip, bool &ignore);

  bool evaluateExpression(const std::string &expr, CExprValueP &value, bool quiet=false) const;

  CExprCTokenStack compileExpression(const std::string &expr) const;

  void setPrintFile(const std::string &file) { printFile_ = file; }
  const std::string &getPrintFile() const { return printFile_; }

  void setPrintAppend(bool b) { printAppend_ = b; }
  bool getPrintAppend() { return printAppend_; }

  void setTableFile(const std::string &file) { tableFile_ = file; }
  const std::string &getTableFile() const { return tableFile_; }

  //---

  void setAngleType(AngleType type);
  AngleType getAngleType() const { return getPrefValue<AngleType>(VariableName::ANGLES); }
  void resetAngleType() { resetPrefValue(VariableName::ANGLES); }
  void printAngleType(std::ostream &os) { printPrefValue(os, VariableName::ANGLES); }
  void showAngleType(std::ostream &os) { showPrefValue(os, VariableName::ANGLES); }
  std::string getAngleTypeString() const {
    return getPrefValueString<AngleType>(VariableName::ANGLES);
  }

  //---

  void setStyleIncrementType(StyleIncrementType s) { styleIncrement_.type = s; }
  StyleIncrementType getStyleIncrementType() { return styleIncrement_.type; }

  const CGnuPlotTitle &title() const { return title_; }
  void setTitle(const CGnuPlotTitle &t) { title_ = t; }

  void setDummyVars(const std::string &dummyVar1, const std::string &dummyVar2);
  void getDummyVars(std::string &dummyVar1, std::string &dummyVar2) const;
  void resetDummyVars();

  const Samples    &samples   () const { return samples_; }
  const ISOSamples &isoSamples() const { return isoSamples_; }

  bool parseAxesTics(CParseLine &line, CGnuPlotAxisData &axis);

  bool parseColor(CParseLine &line, CRGBA &c);
  bool parseColorSpec(CParseLine &line, CGnuPlotColorSpec &c);

  bool parseInteger(CParseLine &line, int &i) const;
  bool parseReal(CParseLine &line, double &r) const;
  bool parseString(CParseLine &line, std::string &str, const std::string &msg="") const;

  bool parseBracketedString(CParseLine &line, std::string &str) const;

  bool getIntegerVariable(const std::string &name, int &value) const;
  bool getRealVariable   (const std::string &name, double &value) const;
  bool getStringVariable (const std::string &name, std::string &value) const;

  bool parsePosition(CParseLine &line, CGnuPlotPosition &pos);
  bool parseCoordValue(CParseLine &line, CGnuPlotCoordValue &v);
  bool parsePoint(CParseLine &line, CPoint2D &point);
  bool parseSize(CParseLine &line, CGnuPlotSize &size);
  bool parseSize(CParseLine &line, CSize2D &size);

  const CGnuPlotMultiplot &multiplot() const { return multiplot_; }
  void setMultiplot(const CGnuPlotMultiplot &m) { multiplot_ = m; }

  //---

  bool parseFunction(CParseLine &line, FunctionData &functionData);

  bool processAssignFunction(const std::string &lhs, const std::string &rhs);

  bool readIdentifier(CParseLine &line, std::string &identifier) const;

  std::string readNonSpace(CParseLine &line);
  std::string readNonSpaceNonComma(CParseLine &line);
  std::string readNonSpaceNonChar(CParseLine &line, char c);
  std::string readNonSpaceNonChar(CParseLine &line, const std::string &c);
  std::string readName(CParseLine &line);

  bool fileReadLine(std::string &line);

  bool fieldToReal(const std::string &field, double &r) const;

  template<typename T>
  T getPrefValue(VariableName name) const {
    return static_cast<CGnuPlotPrefValue<T> *>
     (const_cast<CGnuPlot *>(this)->varPrefs_[name])->get();
  }

  template<typename T>
  void setPrefValue(VariableName name, const T &value) {
    static_cast<CGnuPlotPrefValue<T> *>(varPrefs_[name])->set(value);
  }

  void setPrefValue(VariableName name, const std::string &str) {
    varPrefs_[name]->set(str);
  }

  void resetPrefValue(VariableName name) {
    varPrefs_[name]->reset();
  }

  template<typename T>
  std::string getPrefValueString(VariableName name) const {
    return static_cast<CGnuPlotPrefValue<T> *>
     (const_cast<CGnuPlot *>(this)->varPrefs_[name])->toString();
  }

  void showPrefValue(std::ostream &os, VariableName name) {
    varPrefs_[name]->show(os);
  }

  void printPrefValue(std::ostream &os, VariableName name) {
    varPrefs_[name]->print(os);
  }

 private:
  typedef std::map<std::string,CGnuPlotDevice*> Devices;
  typedef std::vector<CGnuPlotWindowP>          Windows;
  typedef StringArray                           Fields;
  typedef StringArray                           FileLines;
  typedef std::vector<FileData>                 FileDataArray;
  typedef CAutoPtr<CGnuPlotReadLine>            ReadLineP;
  typedef std::map<std::string,std::string>     DummyVarMap;
  typedef std::vector<std::string>              PathList;

  bool                   debug_  { false };
  bool                   edebug_ { false };
  CGnuPlotSVGDevice*     svgDevice_ { 0 };
  CGnuPlotDevice*        device_ { 0 };
  Devices                devices_;
  FileData               fileData_;
  FileDataArray          fileDataArray_;
  Windows                windows_;
  CGnuPlotFile           dataFile_;
  CGnuPlotBoxWidth       boxWidth_;
  Bars                   bars_;
  CGnuPlotBoxPlot        boxPlot_;
  std::string            outputFile_;
  std::string            printFile_;
  std::string            lastPlotCmd_;
  bool                   printAppend_ { false };
  std::string            tableFile_;
  int                    pointNum_ { 0 };
  CISize2D               terminalSize_   { 800, 800 };                  // terminal size
  PlotStyle              dataStyle_      { PlotStyle::POINTS };
  PlotStyle              functionStyle_  { PlotStyle::LINES };
  Smooth                 smooth_         { Smooth::NONE };
  CGnuPlotHistogramData  histogramData_;
  CGnuPlotFillStyle      fillStyle_;
  CGnuPlotLineStyleP     lineStyle_;
  CGnuPlotPointStyle     pointStyle_;
  LineStyles             lineStyles_;
  StyleIncrement         styleIncrement_;
  CGnuPlotTitle          title_;
  VarPrefs               varPrefs_;
  AxesData               axesData_;
  CGnuPlotKeyData        keyData_;
  CBBox2D                region_ { 0, 0, 1, 1 };
  Margin                 margin_;
  CRGBA                  backgroundColor_ { 1, 1, 1};
  std::string            colorSequence_ { "default" };
  int                    xind_ { 1 };
  int                    yind_ { 1 };
  int                    zind_ { 1 };
  ArrowStyles            arrowStyles_;
  CGnuPlotArrowStyle     arrowStyle_;
  LineDashes             lineDashes_;
  bool                   binary_ { false };
  bool                   matrix_ { false };
  Clip                   clip_;
  bool                   parametric_ { false };
  bool                   polar_ { false };
  bool                   enhanced_ { true };
  bool                   macros_ { false };
  HistoryData            historyData_;
  CGnuPlotImageStyle     imageStyle_;
  Annotations            annotations_;
  CGnuPlotCamera         camera_;
  CGnuPlotPalette        palette_;
  CGnuPlotColorBox       colorBox_;
  FilledCurve            filledCurve_;
  std::string            timeFmt_ { "%d/%m/%y,%H:%M" };
  LogScaleMap            logScale_;
  DummyVarMap            dummyVars_;
  Samples                samples_;
  LinkData               linkData_;
  ISOSamples             isoSamples_;
  CGnuPlotTypes::Mapping mapping_ { CGnuPlotTypes::Mapping::CARTESIAN_MAPPING };
  PlotSize               plotSize_;
  DecimalSign            decimalSign_;
  Offsets                offsets_;
  FitData                fitData_;
  PathList               loadPaths_;
  PathList               fontPath_;
  std::string            encoding_;
  std::string            locale_;
  MouseData              mouseData_;
  CGnuPlotMultiplot      multiplot_;
  CGnuPlotWindowP        multiWindow_;
  COptValT<CBBox2D>      clearRect_;
  Hidden3DData           hidden3D_;
  Surface3DData          surface3D_;
  Contour3DData          contour3D_;
  bool                   pm3D_      { false };
  double                 whiskerBars_ { 0 };
  ReadLineP              readLine_;
  mutable Fields         fields_;
};

#endif
