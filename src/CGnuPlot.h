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

//------

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

    BORDER_VAR,
    CLIP_VAR,
    LOGSCALE_VAR,
    BMARGIN_VAR,
    LMARGIN_VAR,
    RMARGIN_VAR,
    TMARGIN_VAR,
    MULTIPLOT_VAR,
    OFFSETS_VAR,
    ORIGIN_VAR,
    SIZE_VAR,

    ARROW_VAR,
    KEY_VAR,
    LABEL_VAR,
    OBJECT_VAR,
    TIMESTAMP_VAR,
    TITLE_VAR,

    STYLE_VAR,

    BARS_VAR,
    BOXWIDTH_VAR,
    POINTSIZE_VAR,

    AUTOSCALE_VAR,
    FORMAT_VAR,
    GRID_VAR,
    XLABEL_VAR,
    YLABEL_VAR,
    XRANGE_VAR,
    YRANGE_VAR,
    TICS_VAR,
    XTICS_VAR,
    YTICS_VAR,
    MXTICS_VAR,
    TICSCALE_VAR,
    ZEROAXIS_VAR,

    LOCALE_VAR,
    TIMEFMT_VAR,
    XDATA_VAR,
    XDTICS_VAR,
    XMTICS_VAR,

    DGRID3D_VAR,
    HIDDEN3D_VAR,
    ISOSAMPLES_VAR,
    SURFACE_VAR,
    VIEW_VAR,
    XYPLANE_VAR,

    CPLANE_VAR,
    CNTRPARAM_VAR,
    CONTOUR_VAR,

    COLORBOX_VAR,
    PALETTE_VAR,
    PM3D_VAR,

    OUTPUT_VAR,
    PRINT_VAR,
    TABLE_VAR,
    TERMINAL_VAR,
    TERMOPTION_VAR,

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
    STYLE_INCREMENT_VAR,
    STYLE_LINE_VAR,
    STYLE_FILL_VAR,
    STYLE_HISTOGRAM_VAR,
    STYLE_RECTANGLE_VAR,
    STYLE_CIRCLE_VAR,
    STYLE_ELLIPSE_VAR,
    STYLE_ARROW_VAR
  };

  enum StyleIncrementType {
    STYLE_INCREMENT_USER,
    STYLE_INCREMENT_DEFAULT
  };

  enum BoxWidthType {
    BOXWIDTH_AUTO,
    BOXWIDTH_ABSOLUTE,
    BOXWIDTH_RELATIVE
  };

  enum ChangeState {
    CHANGE_STATE_PLOT_ADDED,
    CHANGE_STATE_AXIS_DISPLAY,
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

  enum PlotVar {
    PLOT_USING_VAR,
    PLOT_INDEX_VAR,
    PLOT_EVERY_VAR,
    PLOT_WITH_VAR,
    PLOT_TITLE_VAR,
    PLOT_NOTITLE_VAR,
    PLOT_SMOOTH_VAR,
    PLOT_POINTSIZE_VAR,
  };

  enum PlotStyle {
    NO_STYLE,
    LINES_STYLE,
    POINTS_STYLE,
    LINES_POINTS_STYLE,
    DOTS_STYLE,
    IMPULSES_STYLE,
    LABELS_STYLE,
    STEPS_STYLE,
    FSTEPS_STYLE,
    HISTEPS_STYLE,
    FINANCEBARS_STYLE,
    ERRORBARS_STYLE,
    ERRORLINES_STYLE,
    XERRORBARS_STYLE,
    XERRORLINES_STYLE,
    YERRORBARS_STYLE,
    YERRORLINES_STYLE,
    XYERRORBARS_STYLE,
    XYERRORLINES_STYLE,
    VECTORS_STYLE,
    BOXES_STYLE,
    BOXERRORBARS_STYLE,
    BOXXYERRORBARS_STYLE,
    BOXPLOT_STYLE,
    CANDLESTICKS_STYLE,
    FILLEDCURVES_STYLE,
    HISTOGRAMS_STYLE,
    IMAGE_STYLE,
    RGBIMAGE_STYLE,
    RGBALPHA_STYLE,
    CIRCLES_STYLE,
    ELLIPSES_STYLE,
    PM3D_STYLE,
    TEST_TERMINAL_STYLE,
    TEST_PALETTE_STYLE,
  };

  enum SeparatorType {
    SEP_WHITESPACE,
    SEP_CHAR
  };

  enum FillType {
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

  enum AngleType {
    ANGLE_RADIANS,
    ANGLE_DEGREES
  };

  enum LogScale {
    X_LOGSCALE,
    Y_LOGSCALE,
    Z_LOGSCALE,
    X2_LOGSCALE,
    Y2_LOGSCALE,
    CB_LOGSCALE
  };

  typedef std::map<LogScale,int> LogScaleMap;

  enum TestType {
    TEST_NONE,
    TEST_TERMINAL,
    TEST_PALETTE
  };

  enum ColorModel {
    COLOR_MODEL_RGB,
    COLOR_MODEL_HSV,
    COLOR_MODEL_CMY,
    COLOR_MODEL_YIQ,
    COLOR_MODEL_XYZ,
  };

  enum SymbolType {
    SYMBOL_NONE,
    SYMBOL_POINT,
    SYMBOL_PLUS,
    SYMBOL_CROSS,
    SYMBOL_STAR,
    SYMBOL_BOX,
    SYMBOL_FILLED_BOX,
    SYMBOL_CIRCLE,
    SYMBOL_FILLED_CIRCLE,
    SYMBOL_TRIANGLE,
    SYMBOL_FILLED_TRIANGLE,
    SYMBOL_INV_TRIANGLE,
    SYMBOL_FILLED_INV_TRIANGLE,
    SYMBOL_DIAMOND,
    SYMBOL_FILLED_DIAMOND,
    SYMBOL_LAST=SYMBOL_FILLED_DIAMOND
  };

  enum PatternType {
    PATTERN_NONE,
    PATTERN_HATCH,
    PATTERN_DENSE,
    PATTERN_FG,
    PATTERN_FDIAG,
    PATTERN_BDIAG,
    PATTERN_FDIAG1,
    PATTERN_BDIAG1,
    PATTERN_BG,
    PATTERN_LAST=PATTERN_BG
  };

  //---

  struct Separator {
    Separator() : type(SEP_WHITESPACE), c('\0') { }

    Separator(char c) : type(SEP_CHAR), c(c) { }

    SeparatorType type;
    char          c;
  };

  //---

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

  typedef std::vector<UsingCol> UsingCols;

  //---

  struct Index {
    int start;
    int end;
    int step;

    Index() : start(1), end(1), step(1) { }
  };

  //---

  struct Every {
    int start;
    int end;
    int step;

    Every() : start(0), end(INT_MAX), step(1) { }
  };

  //---

  class FillStyle {
   public:
    FillStyle() :
     style_(FILL_EMPTY), density_(1.0), pattern_(CGnuPlot::PATTERN_NONE),
     border_(true), borderLine_(-1) {
    }

    FillType style() const { return style_; }
    void setStyle(FillType style) { style_ = style; }

    double density() const { return density_; }
    void setDensity(double d) { density_ = d; }

    CGnuPlot::PatternType pattern() const { return pattern_; }
    void setPattern(CGnuPlot::PatternType p) { pattern_ = p; }

    bool border() const { return border_; }
    void setBorder(bool b) { border_ = b; }

    int borderLine() const { return borderLine_; }
    void setBorderLine(int b) { borderLine_ = b; }

   private:
    FillType              style_;
    double                density_;
    CGnuPlot::PatternType pattern_;
    bool                  border_;
    int                   borderLine_;
  };

  //---

  class LineStyle {
   public:
    LineStyle() :
     type_(1), width_(1), color_() {
    }

    int type() const { return type_; }
    void setType(int type) { type_ = type; }

    double width() const { return width_; }
    void setWidth(double width) { width_ = width; }

    CRGBA color(const CRGBA &c) const { return color_.getValue(c); }
    void setColor(const CRGBA &c) { color_ = c; }

   private:
    int             type_;
    double          width_;
    COptValT<CRGBA> color_;
  };

  //---

  class PointStyle {
   public:
    PointStyle() :
     type_(SYMBOL_PLUS), size_(1), varSize_(false) {
    }

    int type() const { return type_; }
    void setType(int type) { type_ = type; }

    double size() const { return size_; }
    void setSize(double s) { size_ = s; }

    bool varSize() const { return varSize_; }
    void setVarSize(bool b) { varSize_ = b; }

  private:
    int    type_;
    double size_;
    bool   varSize_;
  };

  //---

  struct BoxWidth {
    double       width;
    BoxWidthType type;

    BoxWidth() :
     width(1), type(BOXWIDTH_AUTO) {
    }
  };

  //---

  struct Title {
    std::string str;
    double      offset;

    Title() : offset(0) { }
  };

  //---

  struct AxisData {
    bool        displayed;
    int         borders;
    bool        grid;
    COptReal    min, max;
    std::string str;
    double      offset;
    bool        showTics;

    AxisData() :
     displayed(true), borders(0xFF), grid(false), offset(0), showTics(true) {
    }
  };

  struct AxesData {
    AxisData xaxis;
    AxisData yaxis;
    AxisData zaxis;
    int      borders;

    AxesData() : borders(0xFF) { }
  };

  struct StyleIncrement {
    StyleIncrementType type;
    int                colorInd;

    StyleIncrement() :
     type(STYLE_INCREMENT_USER), colorInd(1) {
    }
  };

  //---

  struct KeyData {
    bool        displayed;
    std::string title;

    KeyData() : displayed(true) { }
  };

  //---

  struct Position {
    CPoint2D p;

    Position(double x=0.0, double y=0.0) : p(x, y) { }

    Position(const CPoint2D &p1) : p(p1) { }
  };

  //---

  struct PlotSize {
    bool     square;
    COptReal ratio;
    COptReal x;
    COptReal y;
    COptReal xsize;
    COptReal ysize;

    PlotSize() :
     square(false) {
    }
  };

  struct Multiplot {
    bool        enabled;
    int         rows, cols;
    std::string title;

    Multiplot() :
     enabled(false), rows(1), cols(1) {
    }
  };

  //---

  struct Arrow {
    Arrow() :
     ind(-1), from(0,0), to(1,1), relative(false), length(-1), angle(-1), backAngle(-1),
     fhead(false), thead(true), filled(false), front(false), lineType(-1), lineWidth(-1),
     c(0,0,0) {
    }

    int      ind;
    Position from;
    Position to;
    bool     relative;
    double   length, angle, backAngle;
    bool     fhead, thead;
    bool     filled;
    bool     empty;
    bool     front;
    int      lineType;
    double   lineWidth;
    CRGBA    c;
  };

  typedef std::vector<Arrow> Arrows;

  //---

  struct Label {
    Label() :
     ind(-1), text(), align(CHALIGN_TYPE_LEFT), pos(0,0), angle(-1), offset(0.0) {
    }

    int         ind;
    std::string text;
    CHAlignType align;
    Position    pos;
    std::string font;
    double      angle;
    bool        front;
    double      offset;
  };

  typedef std::vector<Label> Labels;

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

      ColorMap::const_iterator p = colors.begin();

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
    bool          enabled;
    CCoordFrame3D coordFrame;
    CVector3D     direction;
    double        fov;
    double        xmin, xmax;
    double        ymin, ymax;
    double        near, far;
    CMatrix3DH    projMatrix;
    double        rotateX, rotateY, rotateZ;

    Camera() :
     enabled(true), direction(0,0,1), fov(90), xmin(-1), xmax(1), ymin(-1), ymax(1),
     near(0.1), far(100), rotateX(60.0), rotateY(0.0), rotateZ(45.0) {
      init();
    }

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

  const LineStyle &lineStyle() const { return lineStyle_; }
  LineStyle &lineStyle() { return lineStyle_; }
  void setLineStyle(const LineStyle &s) { lineStyle_ = s; }

  const PointStyle &pointStyle() const { return pointStyle_; }
  PointStyle &pointStyle() { return pointStyle_; }
  void setPointStyle(const PointStyle &s) { pointStyle_ = s; }

  const COptInt &lineStyleNum() const { return lineStyleNum_; }
  void setLineStyleNum(const COptInt &style) { lineStyleNum_ = style; }
  void resetLineStyleNum() { lineStyleNum_.setInvalid(); }

  LineStyle lineStyles(int i) const {
    LineStyles::const_iterator p = lineStyles_.find(i);

    return (p != lineStyles_.end() ? (*p).second : LineStyle());
  }

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

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const Palette &palette() const { return palette_; }
  void setPalette(const Palette &p) { palette_ = p; }

  const Camera &camera() const { return camera_; }
  Camera &camera() { return camera_; }
  void setCamera(const Camera &c) { camera_ = c; }

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

  void setSeparator(Separator sep) { separator_ = sep; }
  const Separator &getSeparator() const { return separator_; }

  void setCommentChars(const std::string &chars) { commentChars_ = chars; }
  const std::string &getCommentChars() const { return commentChars_; }

  void setMissingStr(const std::string &chars) { missingStr_ = chars; }
  const std::string &getMissingStr() const { return missingStr_; }

  void setBoxWidth(double w, BoxWidthType type) { boxWidth_.width = w; boxWidth_.type = type; }
  double getBoxWidth() { return boxWidth_.width; }
  BoxWidthType getBoxWidthType() { return boxWidth_.type; }

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

  const LogScaleMap &logScaleMap() const { return logScale_; }

  void setLogScale(LogScale scale, int base) {
    logScale_[scale] = base;
  }

  const Arrows &arrows() const { return arrows_; }
  void setArrows(const Arrows &arrows) { arrows_ = arrows; }

  const Labels &labels() const { return labels_; }
  void setLabels(const Labels &labels) { labels_ = labels; }

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

  bool parseColor(CParseLine &line, CRGBA &c);

  bool parseString(const std::string &line, std::string &filename);
  bool parseString(CParseLine &line, std::string &filename);

  bool parsePosition(CParseLine &line, Position &pos);

  const PlotSize &plotSize() const { return plotSize_; }
  PlotSize &plotSize() { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

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

  int trianglePattern3D() const { return trianglePattern3D_; }
  void setTrianglePattern3D(int n) { trianglePattern3D_ = n; }

  std::string readFunction  (CParseLine &line);
  std::string readIdentifier(CParseLine &line);

  std::string readNonSpace(CParseLine &line);
  std::string readNonSpaceNonComma(CParseLine &line);

 private:
  typedef std::vector<CGnuPlotWindow *> Windows;
  typedef std::map<int,LineStyle>       LineStyles;

  bool                             debug_, edebug_;
  Windows                          windows_;
  Separator                        separator_;
  std::string                      commentChars_;
  std::string                      missingStr_;
  BoxWidth                         boxWidth_;
  std::string                      outputFile_;
  std::string                      printFile_;
  bool                             printAppend_;
  std::string                      tableFile_;
  PlotStyle                        dataStyle_;
  PlotStyle                        functionStyle_;
  Smooth                           smooth_;
  HistogramStyle                   histogramStyle_;
  AngleType                        angleType_;
  FillStyle                        fillStyle_;
  LineStyle                        lineStyle_;
  PointStyle                       pointStyle_;
  COptInt                          lineStyleNum_;
  LineStyles                       lineStyles_;
  StyleIncrement                   styleIncrement_;
  Title                            title_;
  AxesData                         axesData_;
  KeyData                          keyData_;
  CRange2D                         margin_;
  Arrows                           arrows_;
  Labels                           labels_;
  Palette                          palette_;
  Camera                           camera_;
  bool                             fitX_, fitY_, fitZ_;
  LogScaleMap                      logScale_;
  std::string                      dummyVar1_, dummyVar2_;
  int                              isamples1_, isamples2_;
  int                              isoSamples1_, isoSamples2_;
  PlotSize                         plotSize_;
  Multiplot                        multiplot_;
  COptValT<CBBox2D>                clearRect_;
  bool                             hidden3D_;
  bool                             surface3D_;
  bool                             contour3D_;
  bool                             pm3D_;
  int                              trianglePattern3D_;
  CAutoPtr<CGnuPlotReadLine>       readLine_;
  mutable std::vector<std::string> fields_;
};

#endif
