#ifndef CGnuPlot_H
#define CGnuPlot_H

#include <CExpr.h>
#include <CFont.h>
#include <CStrUtil.h>
#include <COptVal.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CRGBA.h>
#include <CColor.h>
#include <CAlignType.h>
#include <CAngle.h>
#include <COSNaN.h>

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <climits>
#include <memory>
#include <sys/types.h>

class CGnuPlotAxis;
class CGnuPlotDevice;
class CGnuPlotGroup;
class CGnuPlotKey;
class CGnuPlotPlot;
class CGnuPlotReadLine;
class CGnuPlotSVGDevice;
class CGnuPlotPSDevice;
class CGnuPlotLogDevice;
class CGnuPlotWindow;
class CGnuPlotTimeStamp;
class CGnuPlotStyleBase;
class CGnuPlotPm3D;
class CGnuPlotIndexData;
class CGnuPlotEveryData;

class CUnixFile;
class CParseLine;

typedef std::shared_ptr<CGnuPlotWindow> CGnuPlotWindowP;
typedef std::shared_ptr<CGnuPlotGroup>  CGnuPlotGroupP;
typedef std::shared_ptr<CGnuPlotPlot>   CGnuPlotPlotP;

//------

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotDash.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotLineType.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPointStyle.h>
#include <CGnuPlotCoordValue.h>
#include <CGnuPlotKeyData.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotSize.h>
#include <CGnuPlotAxisData.h>
#include <CGnuPlotTipData.h>
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
#include <CGnuPlotTicLabel.h>
#include <CGnuPlotUsingCols.h>
#include <CGnuPlotImageStyle.h>
#include <CGnuPlotHistogramData.h>
#include <CGnuPlotMultiplot.h>
#include <CGnuPlotPrefValue.h>
#include <CGnuPlotBoxWidth.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotMouseEvent.h>
#include <CGnuPlotKeyEvent.h>
#include <CGnuPlotRectStyle.h>
#include <CGnuPlotTimeStampData.h>
#include <CGnuPlotMouseData.h>
#include <CGnuPlotClip.h>
#include <CGnuPlotBinaryFormat.h>
#include <CGnuPlotContourData.h>
#include <CGnuPlotSurfaceData.h>
#include <CGnuPlotAxesData.h>
#include <CGnuPlotPrintFile.h>
#include <CGnuPlotBlock.h>
#include <CGnuPlotPm3DData.h>
#include <CGnuPlotPlotSize.h>
#include <CGnuPlotFilledCurve.h>
#include <CGnuPlotFitData.h>
#include <CGnuPlotHidden3DData.h>
#include <CGnuPlotKeyTitle.h>
#include <CGnuPlotMargin.h>
#include <CGnuPlotLineProp.h>
#include <CGnuPlotBars.h>
#include <CGnuPlotMatrixData.h>
#include <CGnuPlotSamples.h>
#include <CGnuPlotISOSamples.h>
#include <CGnuPlotSampleVars.h>
#include <CGnuPlotDummyVars.h>

#include <CGnuPlotStyleData.h>
#include <CGnuPlotBoxPlotStyleValue.h>
#include <CGnuPlotCirclesStyleValue.h>
#include <CGnuPlotEllipsesStyleValue.h>
#include <CGnuPlotPieChartStyleValue.h>
#include <CGnuPlotVectorsStyleValue.h>

class CGnuPlot {
 public:
  typedef CGnuPlotTypes::AngleType               AngleType;
  typedef CGnuPlotTypes::AxisDirection           AxisDirection;
  typedef CGnuPlotTypes::AxisType                AxisType;
  typedef CGnuPlotTypes::CommandName             CommandName;
  typedef CGnuPlotTypes::CoordSys                CoordSys;
  typedef CGnuPlotTypes::DataFileVar             DataFileVar;
  typedef CGnuPlotTypes::DrawLayer               DrawLayer;
  typedef CGnuPlotTypes::HistogramStyle          HistogramStyle;
  typedef CGnuPlotTypes::PlotStyle               PlotStyle;
  typedef CGnuPlotTypes::PlotVar                 PlotVar;
  typedef CGnuPlotTypes::Smooth                  Smooth;
  typedef CGnuPlotTypes::StyleIncrementType      StyleIncrementType;
  typedef CGnuPlotTypes::StyleVar                StyleVar;
  typedef CGnuPlotTypes::VariableName            VariableName;
  typedef CGnuPlotTypes::Endian                  Endian;

  typedef std::map<PlotStyle,CGnuPlotStyleBase*> PlotStyles;
  typedef std::map<int,CGnuPlotLineStyleP>       LineStyles;
  typedef std::map<int,CGnuPlotLineTypeP>        LineTypes;
  typedef std::vector<CExprValuePtr>             Values;
  typedef std::vector<Values>                    Matrix;
  typedef std::map<std::string,CExprValuePtr>    Params;
  typedef CGnuPlotTypes::AxisTypeId              AxisTypeId;
  typedef std::set<AxisTypeId>                   AxisTypeIdSet;
  typedef std::vector<std::string>               StringArray;
  typedef std::vector<CPoint2D>                  Points2D;
  typedef std::vector<CPoint3D>                  Points3D;
  typedef std::vector<CSize2D>                   Sizes;
  typedef std::vector<CRGBA>                     Colors;
  typedef std::vector<double>                    Reals;
  typedef std::vector<float>                     Floats;
  typedef std::vector<int>                       Integers;

  //---

  typedef std::map<int,CGnuPlotAxisData> IAxisMap;

  struct StyleIncrement {
    StyleIncrement() { }

    StyleIncrementType type      { StyleIncrementType::DEFAULT };
    int                styleInd  { 0 };
    int                increment { 8 };
  };

  //---

  typedef std::map<int,CGnuPlotVectorsStyleValue> ArrowStyles;
  typedef std::map<int,CLineDash>                 LineDashes;
  typedef std::vector<CGnuPlotGroupAnnotationP>   Annotations;
  typedef std::map<VariableName,Annotations>      VarAnnotations;

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

  struct LinkAxisData {
    bool        enabled { false };
    std::string expr;
    std::string iexpr;
  };

  struct LinkData {
    LinkAxisData linkX2;
    LinkAxisData linkY2;
  };

  struct DecimalSign {
    void setChar(char c1) { c = c1; }

    void setLocale(const std::string &str) { locale = str; }

    char        c { '\0' };
    std::string locale;
  };

  class Offsets {
   public:
    void set(const CGnuPlotCoordValue &l, const CGnuPlotCoordValue &r,
             const CGnuPlotCoordValue &t, const CGnuPlotCoordValue &b) {
      l_ = l; r_ = r;
      t_ = t; b_ = b;
    }

    void unset() {
      l_ = CGnuPlotCoordValue();
      r_ = CGnuPlotCoordValue();
      t_ = CGnuPlotCoordValue();
      b_ = CGnuPlotCoordValue();
    }

    void show(std::ostream &os) {
      os << "offsets are " << l_ << ", " << r_ << ", " << t_ << ", " << b_ << std::endl;
    }

   private:
    CGnuPlotCoordValue l_, r_, t_, b_;
  };

  //---

  struct ForCmd {
    bool        isIn { false };
    std::string var;
    std::string start;
    std::string end;
    std::string inc;
  };

  //---

  struct LoadDataParams {
    std::string filename;

    bool columnheaders { false };
    bool binary        { false };
    bool matrix        { false };
    bool csv           { false };
    bool header        { false };
    char separator     { '\0' };

    Sizes                sizes;
    CGnuPlotBinaryFormat binaryFormat;
  };

  //---

  struct FunctionDataParams {
    bool     is3D { false };
    COptReal xmin;
    COptReal xmax;
    COptReal ymin;
    COptReal ymax;
  };

  //---

  typedef std::vector<CGnuPlotPlotP> Plots;
  typedef StringArray                Statements;

  typedef std::map<CGnuPlot::VariableName,CGnuPlotPrefValueBase *> VarPrefs;

 public:
  CGnuPlot();

  virtual ~CGnuPlot();

  //----

  void setDebug(bool b) { debug_ = b; }
  bool isDebug() const { return debug_; }

  void setExprDebug(bool b);
  bool isExprDebug() const { return edebug_; }

  void setAutoContinue(bool b) { autoContinue_ = b; }
  bool isAutoContinue() const { return autoContinue_; }

  CExpr *expr() const { return expr_; }

  //----

  CGnuPlotDevice *device() const { return device_; }

  void loadStatup();

  void addDevice(const std::string &name, CGnuPlotDevice *device);
  bool setDevice(const std::string &name);

  void pushDevice();
  void popDevice();

  //----

  void addWindow(CGnuPlotWindowP window);

  void removeWindow(CGnuPlotWindow *window);

  //----

  const PlotStyles &plotStyles() const { return plotStyles_; }

  void addPlotStyle(PlotStyle plotStyle, CGnuPlotStyleBase *style);
  CGnuPlotStyleBase *getPlotStyle(PlotStyle plotStyle) const;

  //----

  PlotStyle dataStyle() const { return dataStyle_; }
  void setDataStyle(PlotStyle style) { dataStyle_ = style; }

  PlotStyle functionStyle() const { return functionStyle_; }
  void setFunctionStyle(PlotStyle style) { functionStyle_ = style; }

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &s) { fillStyle_ = s; }

  //---

  // current line style
  CGnuPlotLineStyleP lineStyle();
  void setLineStyle(CGnuPlotLineStyleP ls);

  // get indexed line style
  CGnuPlotLineStyleP getLineStyleInd(int ind);
  void setLineStyleInd(int ind);
  void resetLineStyleInd(int ind);

  const LineStyles &lineStyles() const { return lineStyles_; }
  CGnuPlotLineStyleP lineStyle(int i) const;

  void clearLineStyles();

  //---

  // get indexed line type
  CGnuPlotLineTypeP getLineTypeInd(int ind);
  void resetLineTypeInd(int ind);

  const LineTypes &lineTypes() const { return lineTypes_; }
  CGnuPlotLineTypeP lineType(int i) const;

  //---

  const CGnuPlotPointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const CGnuPlotPointStyle &s) { pointStyle_ = s; }

  //---

  const CGnuPlotHistogramData &histogramData() { return histogramData_; }
  void setHistogramData(const CGnuPlotHistogramData &data) { histogramData_ = data; }

  const CGnuPlotNewHistogramDatas &newHistogramDatas() const { return newHistogramDatas_; }
  void setNewHistogramDatas(const CGnuPlotNewHistogramDatas &v) { newHistogramDatas_ = v; }
  void clearNewHistogramDatas() { newHistogramDatas_.clear(); }

  int newHistogramId() const { return newHistogramDatas_.size() - 1; }

  int histogramPointOffset() const { return histogramPointOffset_; }
  void setHistogramPointOffset(int i) { histogramPointOffset_ = i; }

  //---

  const CGnuPlotStyleData &styleData() const { return styleData_; }
  void setStyleData(const CGnuPlotStyleData &d) { styleData_ = d; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  void setKeyColumnHeadNum(int icol) {
    keyData_.setColumnHead(true);
    keyData_.setColumnNum (icol);
  }

  void setKeyAutoColumnHeadNum() {
    keyData_.setColumnHead (true);
    keyData_.resetColumnNum();
  }

  void setKeyPointLabel(int pointNum, const std::string &str) {
    keyData_.setPointLabel(pointNum, str);
  }

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &r) { region_ = r; }

  const CGnuPlotMargin &margin() const { return margin_; }
  void setMargin(const CGnuPlotMargin &b) { margin_ = b; }

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  int xind() const { return xind_; }
  int yind() const { return yind_; }
  int zind() const { return zind_; }

  CGnuPlotTypes::Mapping mapping() const { return mapping_; }
  void setMapping(CGnuPlotTypes::Mapping m) { mapping_ = m; }

  const CGnuPlotPlotSize &plotSize() const { return plotSize_; }
  void setPlotSize(const CGnuPlotPlotSize &s) { plotSize_ = s; }

  void setMacros(bool b) { macros_ = b; }
  bool isMacros() const { return macros_; }

  double zero() const { return zero_; }
  void setZero(double r) { zero_ = r; }

  //---

  const COptBBox2D &clearRect() const { return clearRect_; }

  //---

  const CGnuPlotAxesData &axesData() const { return axesData_; }
  void setAxesData(const CGnuPlotAxesData &a) { axesData_ = a; }

  const CGnuPlotAxisData &xaxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->xaxis(ind);
  }
  const CGnuPlotAxisData &yaxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->yaxis(ind);
  }
  const CGnuPlotAxisData &zaxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->zaxis(ind);
  }
  const CGnuPlotAxisData &paxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->paxis(ind);
  }
  const CGnuPlotAxisData &taxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->taxis(ind);
  }
  const CGnuPlotAxisData &uaxis() const {
    return const_cast<CGnuPlot *>(this)->uaxis();
  }
  const CGnuPlotAxisData &vaxis() const {
    return const_cast<CGnuPlot *>(this)->vaxis();
  }

  CGnuPlotAxisData &xaxis(int ind) { return axesData_.xaxis(ind); }
  CGnuPlotAxisData &yaxis(int ind) { return axesData_.yaxis(ind); }
  CGnuPlotAxisData &zaxis(int ind) { return axesData_.zaxis(ind); }

  CGnuPlotAxisData &paxis(int ind) { return this->axesData_.paxis(ind); }
  CGnuPlotAxisData &taxis(int ind) { return this->axesData_.taxis(ind); }

  CGnuPlotAxisData &raxis() { return this->axesData_.raxis(); }
  CGnuPlotAxisData &uaxis() { return this->axesData_.uaxis(); }
  CGnuPlotAxisData &vaxis() { return this->axesData_.vaxis(); }

  //---

  void getXRange(double *xmin, double *xmax) const {
    *xmin = xaxis(1).min().getValue(-10);
    *xmax = xaxis(1).max().getValue( 10);
  }
  void getYRange(double *ymin, double *ymax) const {
    *ymin = yaxis(1).min().getValue(-10);
    *ymax = yaxis(1).max().getValue( 10);
  }
  void getZRange(double *zmin, double *zmax) const {
    *zmin = zaxis(1).min().getValue(-10);
    *zmax = zaxis(1).max().getValue( 10);
  }
  void getParametricTRange(double *tmin, double *tmax) const {
    *tmin = taxis(1).min().getValue(-5);
    *tmax = taxis(1).max().getValue( 5);
  }
  void getPolarTRange(double *tmin, double *tmax) const {
    *tmin = taxis(1).min().getValue(0);

    if (isAngleDegrees())
      *tmax = taxis(1).max().getValue(360.0);
    else
      *tmax = taxis(1).max().getValue(2*M_PI);
  }
  void getURange(double *umin, double *umax) const {
    *umin = uaxis().min().getValue(-5);
    *umax = uaxis().max().getValue(5);
  }
  void getVRange(double *vmin, double *vmax) const {
    *vmin = vaxis().min().getValue(-5);
    *vmax = vaxis().max().getValue(5);
  }

  void setXRange(double xmin, double xmax) {
    xaxis(1).setMin(xmin);
    xaxis(1).setMax(xmax);
  }

  void setTRange(double tmin, double tmax) {
    taxis(1).setMin(tmin);
    taxis(1).setMax(tmax);
  }

  //---

  const CGnuPlotBars &bars() const { return bars_; }
  void setBars(const CGnuPlotBars &s) { bars_ = s; }

  double barsSize() const { return bars_.size(); }
  void setBarsSize(double s) { bars_.setSize(s); }

  bool barsFront() const { return bars_.isFront(); }
  void setBarsFront(bool b) { bars_.setFront(b); }

  //---

  const CGnuPlotFile &dataFile() const { return dataFile_; }

  //---

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const CGnuPlotPalette &palette() const { return palette_; }
  void setPalette(const CGnuPlotPalette &p) { palette_ = p; }
  void setPalette(const CGradientPalette &p) { palette_.setGradientPalette(p); }

  const CGnuPlotColorBoxData &colorBox() const { return colorBox_; }
  void setColorBox(const CGnuPlotColorBoxData &c) { colorBox_ = c; }

  const CGnuPlotFilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const CGnuPlotFilledCurve &c) { filledCurve_ = c; }

  const COptReal &whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }
  void resetWhiskerBars() { whiskerBars_.setInvalid(); }

  const CGnuPlotCamera &camera() const { return camera_; }

  //---

  const CGnuPlotHidden3DData &hidden3D() const { return hidden3D_; }
  void setHidden3D(const CGnuPlotHidden3DData &h) { hidden3D_ = h; }

  //---

  const CGnuPlotSurfaceData &surfaceData() const { return surfaceData_; }
  void setSurfaceData(const CGnuPlotSurfaceData &d) { surfaceData_ = d; }

  //---

  const CGnuPlotContourData &contourData() const { return contourData_; }
  void setContourData(const CGnuPlotContourData &d) { contourData_ = d; }

  //---

  const CGnuPlotPm3DData &pm3D() const { return pm3D_; }
  void setPm3D(const CGnuPlotPm3DData &p) { pm3D_ = p; }

  //------

  const CGnuPlotClip &clip() const { return clip_; }
  void setClip(const CGnuPlotClip &clip) { clip_ = clip; }

  //------

  bool isParametric() const { return parametric_; }
  void setParametric(bool b) { parametric_ = b; }

  bool isPolar() const { return polar_; }
  void setPolar(bool b) { polar_ = b; }

  //------

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  const CGnuPlotBinaryFormat &binaryFormat() const { return binaryFormat_; }
  void setBinaryFormat(const CGnuPlotBinaryFormat &fmt) { binaryFormat_ = fmt; }

  const CGnuPlotMatrixData &matrixData() const { return matrixData_; }
  CGnuPlotMatrixData &matrixData() { return matrixData_; }
  void setMatrixData(const CGnuPlotMatrixData &m) { matrixData_ = m; }

  const CGnuPlotImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const CGnuPlotImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  const Endian &endian() const { return endian_; }
  void setEndian(const Endian &v) { endian_ = v; }

  const Integers &recordValues() const { return recordValues_; }
  void setRecordValues(const Integers &s) { recordValues_ = s; }

  //------

  const COptString &timeFmt() { return timeFmt_; }
  void setTimeFmt(const std::string &f) { timeFmt_ = f; }
  void unsetTimeFmt() { timeFmt_.setInvalid(); }

  //------

  bool isImageStyle(PlotStyle style) const {
    return (style == PlotStyle::IMAGE || style == PlotStyle::RGBIMAGE ||
            style == PlotStyle::RGBALPHA);
  }

  //------

  const CGnuPlotTimeStampData &timeStamp() const { return timeStamp_; }

  //------

  const VarAnnotations &annotations() const { return varAnnotations_; }
  void setAnnotations(const VarAnnotations &annotations) { varAnnotations_ = annotations; }

  //------

  const CGnuPlotBoxWidth &boxWidth() const { return styleData_.boxWidth; }
  void setBoxWidth(const CGnuPlotBoxWidth &v) { styleData_.boxWidth = v; }

  const CGnuPlotBoxPlotStyleValue &boxPlotStyleValue() const {
    return styleData_.boxPlotStyleValue; }
  void setBoxPlotStyleValue(const CGnuPlotBoxPlotStyleValue &v) {
    styleData_.boxPlotStyleValue = v; }

  const CGnuPlotCirclesStyleValue &circlesStyleValue() const {
    return styleData_.circlesStyleValue; }
  void setCirclesStyleValue(const CGnuPlotCirclesStyleValue &v) {
    styleData_.circlesStyleValue = v; }

  const CGnuPlotEllipsesStyleValue &ellipsesStyleValue() const {
    return styleData_.ellipsesStyleValue; }
  void setEllipsesStyleValue(const CGnuPlotEllipsesStyleValue &v) {
    styleData_.ellipsesStyleValue = v; }

  const CGnuPlotPieChartStyleValue &pieChartStyleValue() const {
    return styleData_.pieChartStyleValue; }
  void setPieChartStyleValue(const CGnuPlotPieChartStyleValue &v) {
    styleData_.pieChartStyleValue = v; }

  const CGnuPlotVectorsStyleValue &vectorsStyleValue() const {
    return styleData_.vectorsStyleValue; }
  void setVectorsStyleValue(const CGnuPlotVectorsStyleValue &v) {
    styleData_.vectorsStyleValue = v; }

  //------

  void setOutputFile(const std::string &file);
  const std::string &getOutputFile() const { return outputFile_; }

  bool load(const std::string &filename, const StringArray &args=StringArray());

  bool exec(const std::string &cmd);

  void initReadLine() const;

  void loop();

  void prompt(const std::string &msg);

  void waitForMouse(int mask, const std::string &msg);

  void mousePress  (const CGnuPlotMouseEvent &mouseEvent);
  void mouseMove   (const CGnuPlotMouseEvent &mouseEvent, bool pressed);
  void mouseRelease(const CGnuPlotMouseEvent &mouseEvent);

  void keyPress(const CGnuPlotKeyEvent &keyEvent);

  CGnuPlotWindowP createNewWindow();

  CGnuPlotWindow *createWindow();

  bool loadData(const std::string &filename, const LoadDataParams &params);

  bool loadFunction(const std::string &function, const FunctionDataParams &params);

  CGnuPlotMultiplot *createMultiplot();

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group, PlotStyle plotStyle);

  CGnuPlotLineStyle *createLineStyle(CGnuPlot *plot);
  CGnuPlotLineType  *createLineType();

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data);
  CGnuPlotKey  *createKey (CGnuPlotGroup *group);

  CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group);

  CGnuPlotPalette *createPalette(CGnuPlotGroup *group);

  CGnuPlotTitle *createTitle(CGnuPlotGroup *group);

  CGnuPlotCamera *createCamera(CGnuPlotGroup *group);

  CGnuPlotPm3D *createPm3D(CGnuPlotGroup *group);

  CGnuPlotTimeStamp *createTimeStamp(CGnuPlotGroup *group);

  CGnuPlotRenderer *renderer();

  //----

  template<typename T>
  T *getAnnotation(VariableName varName, int ind) const {
    auto p = varAnnotations_.find(varName);

    if (p == varAnnotations_.end())
      return 0;

    for (auto &ann : (*p).second) {
      if (ann->getInd() == ind && (dynamic_cast<T *>(ann.get()) != 0))
        return dynamic_cast<T *>(ann.get());
    }

    return 0;
  }

  template<typename T>
  T *lookupAnnotation(VariableName varName, int ind, bool &created) {
    T *annotation = getAnnotation<T>(varName, ind);

    if (! annotation) {
      // TODO: unique ind ?
      if (ind <= 0) {
        auto p = varAnnotations_.find(varName);

        if (p != varAnnotations_.end())
          ind = (*p).second.size() + 1;
        else
          ind = 1;
      }

      annotation = new T(0);

      annotation->setInd(ind);

      varAnnotations_[varName].push_back(CGnuPlotGroupAnnotationP(annotation));

      created = true;
    }

    return annotation;
  }

  template<typename T>
  void showAnnotations(std::ostream &os=std::cout, int ind=-1) {
    for (const auto &vann : varAnnotations_) {
      for (auto &ann : vann.second) {
        const T *obj = dynamic_cast<const T *>(ann.get());
        if (! obj) continue;

        if (ind != -1 && obj->getInd() != ind) continue;

        os << T::getName() << " " << obj->getInd();
        obj->print(os);
        os << std::endl;
      }
    }
  }

  template<typename T>
  void clearAnnotations(int ind=-1) {
    VarAnnotations varAnnotations;

    for (auto &vann : varAnnotations_) {
      Annotations &annotations = vann.second;

      for (auto &ann : annotations) {
        T *obj = dynamic_cast<T *>(ann.get());

        if (obj) {
          if (ind != -1 && obj->getInd() != ind)
            annotations.push_back(ann);
        }
        else
          annotations.push_back(ann);
      }
    }

    varAnnotations_ = varAnnotations;
  }

  //----

  void timeout();

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state);

  //---

  int numFieldValues() const { return fieldValues_.size(); }

  CExprValuePtr fieldValue(int i) const {
    if (i < 1 || i > int(fieldValues_.size())) return CExprValuePtr();
    return fieldValues_[i - 1];
  }

  bool isParsePlotTitle() const { return parsePlotTitle_; }

  //---

  int setNum() const { return setNum_; }
  void setSetNum(int i) { setNum_ = i; }

  int pointNum() const { return pointNum_; }
  void setPointNum(int n) { pointNum_ = n; }

  bool readEndianFloat(CUnixFile *file, Endian endian, float &f);

  int getColumnIndex(const std::string &str) const;

  CExprValuePtr fieldToValue(int nf, const std::string &field, bool &missing);

  bool timeToValue(const std::string &str, double &t);

  void resetLineStyle();

  void incLineStyle();

  int numWindows() const;

  void drawWindows();

  CGnuPlotVectorsStyleValue arrowStyle(int id) const;

  CLineDash getLineDash(int dt) const;

  void clearTicLabels();

  bool isEOFStr(const std::string &line, const std::string &eofStr) const;

  void errorMsg(const std::string &msg) const;
  void warnMsg (const std::string &msg) const;
  void debugMsg(const std::string &msg) const;

  void getBlockLines(const std::string &name, CGnuPlotFile::Lines &lines);

  CGnuPlotBlock *getBlock(const std::string &name);

  const std::string &lastFilename() const { return lastFilename_; }
  void setLastFilename(const std::string &v);

  const CGnuPlotPrintFile &tableFile() const { return tableFile_; }

  const CGnuPlotSamples &samples() const { return samples_; }
  void setSamples(const CGnuPlotSamples &s) { samples_ = s; }

  const CGnuPlotISOSamples &isoSamples() const { return isoSamples_; }
  void setISOSamples(const CGnuPlotISOSamples &s) { isoSamples_ = s; }

  const CGnuPlotSampleVars &sampleVars() const { return sampleVars_; }
  void setSampleVars(const CGnuPlotSampleVars &v) { sampleVars_ = v; }

  const CGnuPlotDummyVars &dummyVars() const { return dummyVars_; }
  void setDummyVars(const CGnuPlotDummyVars &v) { dummyVars_ = v; }

  const CGnuPlotMultiplotP &multiplot() const { return multiplot_; }
  void setMultiplot(const CGnuPlotMultiplotP &m) { multiplot_ = m; }

  AngleType angleType() const { return getPrefValue<AngleType>(VariableName::ANGLES); }
  void setAngleType(AngleType type);
  void resetAngleType() { resetPrefValue(VariableName::ANGLES); }

  bool readDataFile(const std::string &filename, CGnuPlotFile &dataFile);

  void readFileLines(StringArray &lines) const;

  void setPlotValues2D(CGnuPlotPlot *plot);
  void setPlotValues3D(CGnuPlotPlot *plot);

 private:
  void addPlotStyles();

  bool parseLine(const std::string &str);

  std::string replaceEmbedded(const std::string &str) const;

  void replaceEmbeddedString(CParseLine &line, std::string &str) const;
  bool replaceEmbeddedCmd   (CParseLine &line, std::string &str) const;

  static bool replaceEscapeChar(CParseLine &line, std::string &str);

  bool execCmd(const std::string &cmd, StringArray &lines) const;

  bool parseStatement(int &i, const Statements &statements);

  void getStatements(CParseLine &line, Statements &statements) const;

  bool parseFunctionDef(const std::string &identifier, CParseLine &line);
  bool parseVariableDef(const std::string &identifier, CParseLine &line);

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

  void dataCmd   (const std::string &args);
  void windowCmd (const std::string &args);
  void plotCmd   (const std::string &args);
  void plotCmd1  (const std::string &args, CGnuPlotGroupP &group, Plots &plots,
                  bool &sample, bool &first);
  void replotCmd (const std::string &args);
  void refreshCmd(const std::string &args);
  void splotCmd  (const std::string &args);
  void splotCmd1 (const std::string &args, CGnuPlotGroupP &group, Plots &plots,
                  bool &sample, bool &first);

  void saveAxisData();
  void restoreAxisData();

  void plotForCmd (const ForCmd &forCmd, const std::string &args,
                   CGnuPlotGroupP &group, Plots &plots);
  void splotForCmd(const ForCmd &forCmd, const std::string &args,
                   CGnuPlotGroupP &group, Plots &plots);
  void setForCmd  (const ForCmd &forCmd, const std::string &args);
  void unsetForCmd(const ForCmd &forCmd, const std::string &args);

  void splitPlotCmd(const std::string &cmd, StringArray &cmds, bool is3D);

  void parseFilledCurve(CParseLine &line, CGnuPlotFilledCurve &filledCurve);

  bool parseModifiers2D(PlotStyle style, CParseLine &line, CGnuPlotLineStyle &ls,
                        CGnuPlotFillStyle &fs, CGnuPlotStyleData &styleData,
                        CGnuPlotKeyTitle &keyTitle);
  bool parseModifiers3D(PlotStyle style, CParseLine &line, CGnuPlotLineStyle &ls,
                        CGnuPlotFillStyle &fs, CGnuPlotStyleData &styleData,
                        CGnuPlotKeyTitle &keyTitle);

  bool parseFor(CParseLine &line, ForCmd &forCmd, std::string &cmd);

  void addPlotWithStyle(CGnuPlotPlotP &plot, Plots &plots, const CGnuPlotLineStyle &lineStyle,
                        const CGnuPlotFillStyle &fillStyle, const CGnuPlotStyleData &styleData,
                        const CGnuPlotKeyTitle &keyTitle);

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

  void resetPlotData();

  bool stringToAxes(const std::string &axesStr, AxisTypeIdSet &axisTypeIdSet) const;

  void showColorNames();
  void showVariables(std::ostream &os, const StringArray &args=StringArray());
  void showFunctions(std::ostream &os);
  void showRGBFormulae(std::ostream &os);

  void readBlockLines(Statements &lines, std::string &eline, int depth);

  bool readNamedBlock(const std::string &name, const std::string &eofStr);

  std::string parseUsingStr(CParseLine &line);

  bool parseIndex(CParseLine &line, CGnuPlotIndexData &indexData);
  bool parseEvery(CParseLine &line, CGnuPlotEveryData &everyData);

  bool parseFont(CParseLine &line, CFontPtr &font);

  bool parseDataFileSeparator(CParseLine &line, char &c) const;

  bool parseBoolExpression(CParseLine &line, bool &res);

  bool parseBracketedExpression(CParseLine &line, std::string &expr);
  bool parseExpression(CParseLine &line, std::string &expr);

  bool parseFillStyle(CParseLine &line, CGnuPlotFillStyle &fillStyle);

  void addFile2D(Plots &plots, CGnuPlotGroupP &group, const std::string &filename,
                 PlotStyle style, const CGnuPlotUsingCols &usingCols,
                 CGnuPlotLineStyle &ls, CGnuPlotFillStyle &fs,
                 CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle);

  void addGen1File2D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
                     const CGnuPlotUsingCols &usingCols,
                     CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
                     CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle);
  void addGen2File2D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
                     const CGnuPlotUsingCols &usingCols,
                     CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
                     CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle);

 public:
  void initGen1File2D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols);
  void initGen2File2D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols);

 private:
  void addFile3D(Plots &plots, CGnuPlotGroupP &group, const std::string &filename,
                 PlotStyle style, const CGnuPlotUsingCols &usingCols,
                 CGnuPlotLineStyle &ls, CGnuPlotFillStyle &fs,
                 CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle);

  void addGen1File3D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
                     const CGnuPlotUsingCols &usingCols,
                     CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
                     CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle);
  void addGen2File3D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
                     const CGnuPlotUsingCols &usingCols,
                     CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
                     CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle);

 public:
  void initGen1File3D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols);
  void initGen2File3D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols);

 private:
  void parseCommentStyle(CParseLine &line, CGnuPlotLineStyle &ls, CGnuPlotFillStyle &fs,
                         CGnuPlotStyleData &styleData, const CGnuPlotKeyTitle &keyTitle);

 public:
  CGnuPlotPlotP addImage2D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
                           const CGnuPlotUsingCols &usingCols);
  CGnuPlotPlotP addImage3D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
                           const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlotP addBinary2D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
                            const CGnuPlotUsingCols &usingCols);
  CGnuPlotPlotP addBinary3D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
                            const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlotP readBinaryFormatFile2D(CUnixFile *file, CGnuPlotGroupP &group, PlotStyle style,
                                       const CGnuPlotUsingCols &usingCols);
  CGnuPlotPlotP readBinaryFormatFile3D(CUnixFile *file, CGnuPlotGroupP &group, PlotStyle style,
                                       const CGnuPlotUsingCols &usingCols);

  bool readBinaryFile2D(const std::string &filename, CGnuPlotPlotP &plot);
  bool readBinaryFile3D(const std::string &filename, CGnuPlotPlotP &plot,
                        const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlotP addFunction2D(CGnuPlotGroupP &group, const StringArray &functions,
                              PlotStyle style);
  CGnuPlotPlotP addFunction3D(CGnuPlotGroupP &group, const StringArray &functions,
                              PlotStyle style);

  static void updateFunction2D(CGnuPlotPlot *plot);

 private:
  bool readBinaryFile2D(CUnixFile *file, CGnuPlotPlotP &plot);
  bool readBinaryFile3D(CUnixFile *file, CGnuPlotPlotP &plot, const CGnuPlotUsingCols &usingCols);

 private:
  void parseLineProp(CParseLine &line, CGnuPlotLineProp &lineProp);

  bool parseAxisRange   (CParseLine &line, CGnuPlotAxisData &axis, bool hasArgs=true);
  bool parseAxisLabel   (CParseLine &line, CGnuPlotAxisData &axis);
  bool parseAxesTics    (CParseLine &line, CGnuPlotAxisData &axis);
  void parseAxisZeroAxis(CParseLine &line, CGnuPlotAxisData &axis);

  bool parseRange(CParseLine &line, StringArray &fields, std::string &dummyVar);

  bool decodeRange(const StringArray &fields, CGnuPlotAxisData &axis);

  void parseArrayValues(CParseLine &line, Sizes &sizes);
  void parseArrayValues(CParseLine &line, Integers &values);

  CExprTokenStack compileExpression(const std::string &expr) const;

  //---

  bool hasTableFile() const { return tableFile_.isEnabled(); }

  void setTableStdErr() {
    tableFile_.setStdErr(); tableFile_.setEnabled(true);
  }

  const std::string &getTableFile() const { return tableFile_.getFile(); }
  void setTableFile(const std::string &file) {
    tableFile_.setFile(file); tableFile_.setEnabled(true);
  }

  const std::string &getTableDataBlock() const { return tableFile_.getDataBlock(); }
  void setTableDataBlock(const std::string &block) {
    tableFile_.setDataBlock(block); tableFile_.setEnabled(true);
  }

  void unsetTableFile() {
    tableFile_.unset(); tableFile_.setEnabled(false);
  }

  //---

  void printAngleType(std::ostream &os) { printPrefValue(os, VariableName::ANGLES); }
  void showAngleType(std::ostream &os) { showPrefValue(os, VariableName::ANGLES); }

  std::string getAngleTypeString() const {
    return getPrefValueString<AngleType>(VariableName::ANGLES);
  }

  bool isAngleDegrees() const { return (angleType() == CGnuPlotTypes::AngleType::DEGREES); }

  //---

  void setStyleIncrementType(StyleIncrementType s) { styleIncrement_.type = s; }
  StyleIncrementType getStyleIncrementType() { return styleIncrement_.type; }

  const CGnuPlotTitleData &title() const { return title_; }
  void setTitle(const CGnuPlotTitleData &t) { title_ = t; }

  bool parseLineType(CParseLine &line, int &lt);

  bool parseRGBColor(CParseLine &line, CRGB &rgb, double &a);
  bool parseHSVColor(CParseLine &line, CHSV &hsv, double &a);

  bool parseColorSpec(CParseLine &line, CGnuPlotColorSpec &c);

  bool parseDash(CParseLine &line, CGnuPlotDash &dash);
  bool parseDash(CParseLine &line, CLineDash &dash);

  bool parseAssignment(CParseLine &line);

  bool parseInteger(CParseLine &line, int &i) const;
  bool parseReal   (CParseLine &line, double &r) const;
  bool parseString (CParseLine &line, std::string &str,
                    const std::string &msg="", bool conv=true) const;
  bool parseValue  (CParseLine &line, CExprValuePtr &value, const std::string &msg="") const;

  bool skipExpression(const char *id, CParseLine &line, std::string &expr) const;

 public:
  static void skipString(CParseLine &line);

 private:
  bool parseRoundBracketedString(CParseLine &line, std::string &str) const;
  bool skipRoundBracketedString(CParseLine &line) const;

  bool parseSquareBracketedString(CParseLine &line, std::string &str) const;
  bool skipSquareBracketedString(CParseLine &line) const;

  bool getIntegerVariable(const std::string &name, int &value) const;
  bool getRealVariable   (const std::string &name, double &value) const;
  bool getStringVariable (const std::string &name, std::string &value) const;

  bool isColumnHeader(CParseLine &line, COptInt &col);

  bool parsePosition(CParseLine &line, CGnuPlotPosition &pos);
  bool parseCoordValue(CParseLine &line, CGnuPlotCoordValue &v);
  bool parseOffset(CParseLine &line, CPoint3D &point);
  bool parseOffset(CParseLine &line, CPoint2D &point);
  bool parsePoint(CParseLine &line, CPoint3D &point);
  bool parsePoint(CParseLine &line, CPoint2D &point);
  bool parseSize(CParseLine &line, CGnuPlotSize &size);
  bool parseSize(CParseLine &line, CSize2D &size);

  //---

  bool parseFunction(CParseLine &line, FunctionData &functionData);

  bool processAssignFunction(const std::string &str);

  bool readIdentifier(CParseLine &line, std::string &identifier) const;

  std::string readNonSpace(CParseLine &line) const;
  std::string readNonSpaceNonComma(CParseLine &line) const;
  std::string readNonSpaceNonChar(CParseLine &line, char c) const;
  std::string readNonSpaceNonChar(CParseLine &line, const std::string &c) const;
  std::string readName(CParseLine &line) const;

  void readDataFileLines();

  void fileReadLines(CUnixFile &file, CGnuPlotFile::Lines &lines);

  bool fileReadLine(std::string &line) const;

 public:
  static bool fieldToReal(const std::string &field, double &r);

 private:
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

  bool evaluateExpression(const std::string &expr, CExprValuePtr &value, bool quiet=false) const;
  bool evaluateExpression(const std::string &expr, Values &values, bool quiet=false) const;

  bool exprToInteger(const std::string &expr, int &i, bool quiet=false) const;
  bool exprToReal   (const std::string &expr, double &r, bool quiet=false) const;
  bool exprToString (const std::string &expr, std::string &s,
                     bool quiet=false, bool conv=true) const;

  std::string replaceSumInExpr(const std::string &expr) const;

  double scaleColorComponent(double z) const;

  void setDeviceEnhanced(bool b);

 public:
  static PlotStyle stringToPlotStyle(const std::string &str);

 private:
  typedef std::map<std::string,CGnuPlotDevice*>  Devices;
  typedef std::vector<CGnuPlotDevice*>           DeviceStack;
  typedef StringArray                            FileLines;
  typedef std::vector<FileData>                  FileDataArray;
  typedef std::unique_ptr<CGnuPlotReadLine>      ReadLineP;
  typedef std::vector<std::string>               PathList;
  typedef std::map<std::string,CGnuPlotBlock *>  Blocks;

  bool                      debug_  { false };
  bool                      edebug_ { false };
  bool                      autoContinue_ { false };
  CExpr*                    expr_ { 0 };

  // devices
  CGnuPlotSVGDevice*        svgDevice_ { 0 };
  CGnuPlotPSDevice*         psDevice_ { 0 };
  CGnuPlotLogDevice*        logDevice_ { 0 };
  CGnuPlotDevice*           device_ { 0 };
  Devices                   devices_;
  DeviceStack               deviceStack_;

  PlotStyles                plotStyles_;
  FileData                  fileData_;
  FileDataArray             fileDataArray_;
  CGnuPlotFile              dataFile_;
  CGnuPlotBars              bars_;
  std::string               outputFile_;
  CGnuPlotPrintFile         printFile_;
  std::string               lastPlotCmd_;
  std::string               lastSPlotCmd_;
  std::string               lastFilename_;
  CGnuPlotPrintFile         tableFile_;
  Smooth                    smooth_ { Smooth::NONE };
  CGnuPlotHistogramData     histogramData_;
  CGnuPlotNewHistogramDatas newHistogramDatas_;
  int                       histogramPointOffset_ { 0 };
  CGnuPlotLineStyleP        lineStyle_;
  CGnuPlotPointStyle        pointStyle_;
  CGnuPlotTitleData         title_;
  VarPrefs                  varPrefs_;
  CGnuPlotAxesData          axesData_;
  CGnuPlotAxesData          saveAxesData_;
  CGnuPlotKeyData           keyData_;
  CBBox2D                   region_ { 0, 0, 1, 1 };
  CGnuPlotMargin            margin_;
  CRGBA                     backgroundColor_ { 1, 1, 1};
  std::string               colorSequence_ { "default" };
  int                       xind_ { 1 };
  int                       yind_ { 1 };
  int                       zind_ { 1 };

  // styles
  ArrowStyles            arrowStyles_;
  PlotStyle              dataStyle_      { PlotStyle::POINTS };
  CGnuPlotFillStyle      fillStyle_;
  PlotStyle              functionStyle_  { PlotStyle::LINES };
  StyleIncrement         styleIncrement_;
  LineStyles             lineStyles_;
  LineTypes              lineTypes_;
  CGnuPlotLineTypeP      blackLineType_;
  CGnuPlotLineTypeP      nodrawLineType_;
  CGnuPlotLineTypeP      bgndLineType_;
  CGnuPlotRectStyle      rectStyle_;
  CGnuPlotStyleData      styleData_;

  LineDashes             lineDashes_;
  bool                   binary_ { false };
  CGnuPlotBinaryFormat   binaryFormat_;
  CGnuPlotMatrixData     matrixData_;
  CGnuPlotClip           clip_;
  bool                   parametric_ { false };
  bool                   polar_ { false };
  bool                   macros_ { false };
  double                 zero_ { 1E-8 };
  HistoryData            historyData_;
  CGnuPlotImageStyle     imageStyle_;
  Endian                 endian_ { CGnuPlotTypes::Endian::DEFAULT };
  Integers               recordValues_;
  VarAnnotations         varAnnotations_;
  CGnuPlotCamera         camera_;
  CGnuPlotPalette        palette_;
  CGnuPlotColorBoxData   colorBox_;
  CGnuPlotFilledCurve    filledCurve_;
  COptString             timeFmt_;
  CGnuPlotDummyVars      dummyVars_;
  CGnuPlotSamples        samples_;
  CGnuPlotISOSamples     isoSamples_;
  LinkData               linkData_;
  CGnuPlotTypes::Mapping mapping_ { CGnuPlotTypes::Mapping::CARTESIAN_MAPPING };
  CGnuPlotPlotSize       plotSize_;
  DecimalSign            decimalSign_;
  Offsets                offsets_;
  CGnuPlotFitData        fitData_;
  PathList               loadPaths_;
  PathList               fontPath_;
  std::string            psDir_;
  std::string            encoding_;
  std::string            locale_;
  CGnuPlotMouseData      mouseData_;
  CGnuPlotMultiplotP     multiplot_;
  CGnuPlotTimeStampData  timeStamp_;
  COptBBox2D             clearRect_;
  CGnuPlotHidden3DData   hidden3D_;
  CGnuPlotSurfaceData    surfaceData_;
  CGnuPlotContourData    contourData_;
  CGnuPlotPm3DData       pm3D_;
  double                 pointIntervalBox_ { 1 };
  COptReal               whiskerBars_;
  mutable ReadLineP      readLine_;
  Blocks                 blocks_;
  CGnuPlotSampleVars     sampleVars_;
  CGnuPlotSampleVars     saveSampleVars_;

  mutable CoordSys       defSystem_      { CoordSys::FIRST };
  mutable Values         fieldValues_;
  bool                   parsePlotTitle_ { false };
  mutable int            setNum_         { 0 };
  mutable int            pointNum_       { 0 };
  mutable int            interruptMask_  { 0 };
};

#endif
