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

    KEY,
    ARROW,
    ELLIPSE,
    LABEL,
    POLYGON,
    RECTANGLE,
    OBJECT,
    TIMESTAMP,
    TITLE,

    STYLE,
    DASHTYPE,
    LINETYPE,
    POINTINTERVALBOX,

    BARS,
    BOXWIDTH,
    POINTSIZE,

    AUTOSCALE,
    FORMAT,
    GRID,

    XLABEL,
    X2LABEL,
    YLABEL,
    Y2LABEL,
    ZLABEL,
    CBLABEL,

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
    TEXTBOX,
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

  enum class AngleType {
    RADIANS,
    DEGREES
  };

  enum class TestType {
    NONE,
    TERMINAL,
    PALETTE
  };

  typedef CGnuPlotTypes::BoxWidthType   BoxWidthType;
  typedef CGnuPlotTypes::Smooth         Smooth;
  typedef CGnuPlotTypes::HistogramStyle HistogramStyle;
  typedef CGnuPlotTypes::LogScale       LogScale;
  typedef std::map<LogScale,int>        LogScaleMap;

  //---

  typedef std::map<int,CGnuPlotLineStyleP> LineStyles;

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

  typedef std::map<int,CGnuPlotAxisData> IAxisMap;

  struct AxesData {
    AxesData() { }

    IAxisMap         xaxis;
    IAxisMap         yaxis;
    IAxisMap         zaxis;
    IAxisMap         paxis;
    IAxisMap         taxis;
    CGnuPlotAxisData cbaxis;
    int              borders     { 0xFF };
    double           borderWidth { 1.0 };
    COptInt          borderStyle;
    COptInt          borderType;
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

    double dx() const { return (cols > 0 ? 1.0/cols : 1.0); }
    double dy() const { return (rows > 0 ? 1.0/rows : 1.0); }

    CIPoint2D pos(int n) {
      int x = 0, y = 0;

      if (cols > 0 && rows > 0) {
        x = n % cols;
        y = n / cols;
      }

      return CIPoint2D(x, y);
    }

    bool        enabled { false };
    bool        autoFit { true };
    int         rows    { 0 };
    int         cols    { 0 };
    std::string title;
  };

  //---

  typedef std::map<int,CGnuPlotArrowStyle>      ArrowStyles;
  typedef std::map<int,CLineDash>               LineDashes;
  typedef std::vector<CGnuPlotGroupAnnotationP> Annotations;

  //---

  struct Margin {
    Margin(double l, double b, double r, double t) :
     lmargin(l), bmargin(b), rmargin(r), tmargin(t) {
    }

    double left  () const { return lmargin; }
    double bottom() const { return bmargin; }
    double right () const { return rmargin; }
    double top   () const { return tmargin; }

    void setLeft  (double l, bool s=false) { lmargin = l; lscreen = s; }
    void setBottom(double b, bool s=false) { bmargin = b; bscreen = s; }
    void setRight (double r, bool s=false) { rmargin = r; rscreen = s; }
    void setTop   (double t, bool s=false) { tmargin = t; tscreen = s; }

    double lmargin { 0 }; bool lscreen { false };
    double bmargin { 0 }; bool bscreen { false };
    double rmargin { 0 }; bool rscreen { false };
    double tmargin { 0 }; bool tscreen { false };

    CRange2D range() const { return CRange2D(lmargin, bmargin, rmargin, tmargin); }
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

  //---

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

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  void getXRange(double *xmin, double *xmax) {
    *xmin = axesData_.xaxis[1].min().getValue(-10);
    *xmax = axesData_.xaxis[1].max().getValue(10);
  }
  void getYRange(double *ymin, double *ymax) {
    *ymin = axesData_.yaxis[1].min().getValue(-1);
    *ymax = axesData_.yaxis[1].max().getValue(1);
  }
  void getZRange(double *zmin, double *zmax) {
    *zmin = axesData_.zaxis[1].min().getValue(-1);
    *zmax = axesData_.zaxis[1].max().getValue(1);
  }
  void getTRange(double *tmin, double *tmax) {
    *tmin = axesData_.taxis[1].min().getValue(0);
    *tmax = axesData_.taxis[1].max().getValue(M_PI);
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

  const CGnuPlotPalette &palette() const { return palette_; }
  void setPalette(const CGnuPlotPalette &p) { palette_ = p; }

  const CGnuPlotColorBox &colorBox() const { return colorBox_; }
  void setColorBox(const CGnuPlotColorBox &c) { colorBox_ = c; }

  const FilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const FilledCurve &c) { filledCurve_ = c; }

  const LogScaleMap &logScaleMap() const { return logScale_; }

  int trianglePattern3D() const { return trianglePattern3D_; }
  void setTrianglePattern3D(int n) { trianglePattern3D_ = n; }

  double whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }

  const CGnuPlotCamera &camera() const { return camera_; }
  void setCamera(const CGnuPlotCamera &c) { camera_ = c; }

  //------

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool m) { matrix_ = m; }

  const CGnuPlotImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const CGnuPlotImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  //------

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  //------

  const Annotations &annotations() const { return annotations_; }
  void setAnnotations(const Annotations &annotations) { annotations_ = annotations; }

  //------

  void setOutputFile(const std::string &file) { outputFile_ = file; }
  const std::string &getOutputFile() const { return outputFile_; }

  bool load(const std::string &filename);

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
  void showAnnotations() {
    for (const auto &ann : annotations_) {
      const T *obj = dynamic_cast<const T *>(ann.get());
      if (! obj) continue;

       std::cerr << T::getName() << " " << obj->getInd() << ",";
       obj->print(std::cerr);
       std::cerr << std::endl;
    }
  }

  template<typename T>
  void clearAnnotations() {
    Annotations annotations;

    for (auto &ann : annotations_) {
      T *obj = dynamic_cast<T *>(ann.get());

      if (! obj)
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

  void errorMsg(const std::string &msg) const;

 private:
  bool parseLine(const std::string &str);
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

  bool setCmd  (const std::string &args);
  bool showCmd (const std::string &args);
  void resetCmd(const std::string &args);
  void undefCmd(const std::string &args);
  bool unsetCmd(const std::string &args);

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

  void parseUsing(CParseLine &line, CGnuPlotUsingCols &usingCols);

  void parseIndex(CParseLine &line, int &indexStart, int &indexEnd, int &indexStep);
  void parseEvery(CParseLine &line, int &everyStart, int &everyEnd, int &everyStep);

  bool parseFont(CParseLine &line, CFontPtr &font);

  bool parseBoolExpression(CParseLine &line, bool &res);

  bool parseExpression(CParseLine &line, std::string &expr);

  bool parseFillStyle(CParseLine &line, CGnuPlotFillStyle &fillStyle);

  CGnuPlotPlot *addFunction2D(CGnuPlotGroup *group, const std::vector<std::string> &functions,
                              PlotStyle style);
  CGnuPlotPlot *addFunction3D(CGnuPlotWindowP window, const std::string &str, PlotStyle style);

  Plots addFile2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                  const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlot *addImage2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                           const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlot *addFile3D(CGnuPlotWindowP window, const std::string &filename);

  bool parseAxisRange(CParseLine &line, CGnuPlotAxisData &axis, bool hasArgs=true);

  bool parseRange(CParseLine &line, std::vector<std::string> &fields);

  bool decodeRange(const std::vector<std::string> &xfields, CGnuPlotAxisData &axis);

  CExprValueP decodeUsingCol(int i, const CGnuPlotUsingCol &col, int setNum,
                             int pointNum, bool &skip, bool &ignore);

  bool evaluateExpression(const std::string &expr, CExprValueP &value, bool quiet=false) const;

  CExprCTokenStack compileExpression(const std::string &expr) const;

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

  const CGnuPlotTitle &title() const { return title_; }
  void setTitle(const CGnuPlotTitle &t) { title_ = t; }

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

  bool parseAxesTics(CParseLine &line, CGnuPlotAxisData &axis);

  bool parseColor(CParseLine &line, CRGBA &c);
  bool parseColorSpec(CParseLine &line, CGnuPlotColorSpec &c);

  bool parseInteger(CParseLine &line, int &i);
  bool parseReal(CParseLine &line, double &r);
  bool parseString(CParseLine &line, std::string &str, const std::string &msg="");

  bool parseBracketedString(CParseLine &line, std::string &str);

  bool getIntegerVariable(const std::string &name, int &value);
  bool getRealVariable   (const std::string &name, double &value);
  bool getStringVariable (const std::string &name, std::string &value);

  bool parsePosition(CParseLine &line, CGnuPlotPosition &pos);
  bool parseCoordValue(CParseLine &line, CGnuPlotCoordValue &v);
  bool parsePoint(CParseLine &line, CPoint2D &point);
  bool parseSize(CParseLine &line, CGnuPlotSize &size);
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

  bool parseFunction(CParseLine &line, FunctionData &functionData);

  bool processAssignFunction(const std::string &lhs, const std::string &rhs);

  bool readIdentifier(CParseLine &line, std::string &identifier);

  std::string readNonSpace(CParseLine &line);
  std::string readNonSpaceNonComma(CParseLine &line);
  std::string readNonSpaceNonChar(CParseLine &line, char c);
  std::string readNonSpaceNonChar(CParseLine &line, const std::string &c);
  std::string readName(CParseLine &line);

  bool fileReadLine(std::string &line);

  bool fieldToReal(const std::string &field, double &r) const;

 private:
  typedef std::map<std::string,CGnuPlotDevice*> Devices;
  typedef std::vector<CGnuPlotWindowP>          Windows;
  typedef std::vector<std::string>              Fields;
  typedef std::vector<std::string>              FileLines;
  typedef std::vector<FileData>                 FileDataArray;
  typedef CAutoPtr<CGnuPlotReadLine>            ReadLineP;
  typedef std::map<std::string,std::string>     DummyVarMap;

  bool                  debug_  { false };
  bool                  edebug_ { false };
  CGnuPlotSVGDevice*    svgDevice_ { 0 };
  CGnuPlotDevice*       device_ { 0 };
  Devices               devices_;
  FileData              fileData_;
  FileDataArray         fileDataArray_;
  Windows               windows_;
  CGnuPlotFile          dataFile_;
  std::string           missingStr_;
  BoxWidth              boxWidth_;
  BarSize               barSize_;
  BoxPlot               boxPlot_;
  std::string           outputFile_;
  std::string           printFile_;
  std::string           lastPlotCmd_;
  bool                  printAppend_ { false };
  std::string           tableFile_;
  int                   pointNum_ { 0 };
  CISize2D              terminalSize_   { 600, 600 };                  // terminal size
  PlotStyle             dataStyle_      { PlotStyle::POINTS };
  PlotStyle             functionStyle_  { PlotStyle::LINES };
  Smooth                smooth_         { Smooth::NONE };
  CGnuPlotHistogramData histogramData_;
//AngleType             angleType_      { AngleType::RADIANS };
  CGnuPlotFillStyle     fillStyle_;
  CGnuPlotLineStyleP    lineStyle_;
  CGnuPlotPointStyle    pointStyle_;
  LineStyles            lineStyles_;
  StyleIncrement        styleIncrement_;
  CGnuPlotTitle         title_;
  VarPrefs              varPrefs_;
  AxesData              axesData_;
  CGnuPlotKeyData       keyData_;
  CBBox2D               region_ { 0, 0, 1, 1 };
  Margin                margin_ { 10, 10, 10, 10 };
  CRGBA                 backgroundColor_ { 1, 1, 1};
  int                   xind_ { 1 };
  int                   yind_ { 1 };
  int                   zind_ { 1 };
  ArrowStyles           arrowStyles_;
  CGnuPlotArrowStyle    arrowStyle_;
  LineDashes            lineDashes_;
  bool                  binary_ { false };
  bool                  matrix_ { false };
  bool                  clip_ { false };
  bool                  parametric_ { false };
  bool                  polar_ { false };
  bool                  enhanced_ { true };
  CGnuPlotImageStyle    imageStyle_;
  Annotations           annotations_;
  CGnuPlotCamera        camera_;
  CGnuPlotPalette       palette_;
  CGnuPlotColorBox      colorBox_;
  FilledCurve           filledCurve_;
  std::string           timeFmt_;
  LogScaleMap           logScale_;
  DummyVarMap           dummyVars_;
  int                   isamples1_   { 100 };
  int                   isamples2_   { 100 };
  int                   isoSamples1_ {  10 };
  int                   isoSamples2_ {  10 };
  PlotSize              plotSize_;
  Multiplot             multiplot_;
  CGnuPlotWindowP       multiWindow_;
  COptValT<CBBox2D>     clearRect_;
  bool                  hidden3D_  { false };
  bool                  surface3D_ {  true };
  bool                  contour3D_ { false };
  bool                  pm3D_      { false };
  int                   trianglePattern3D_ { 3 };
  double                whiskerBars_ { 0 };
  ReadLineP             readLine_;
  mutable Fields        fields_;
};

#endif
