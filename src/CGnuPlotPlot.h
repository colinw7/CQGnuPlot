#ifndef CGnuPlotPlot_H
#define CGnuPlotPlot_H

#include <CGnuPlotContour.h>
#include <CGnuPlotSurface.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotPoint.h>
#include <CGnuPlotCache.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotRotate.h>
#include <CGnuPlotProbeEvent.h>

#include <CExpr.h>
#include <CBBox2D.h>
#include <vector>
#include <map>

typedef unsigned char uchar;

class CGnuPlot;
class CGnuPlotWindow;
class CGnuPlotGroup;

class CGnuPlotArrowObject;
class CGnuPlotBoxBarObject;
class CGnuPlotEndBar;
class CGnuPlotBoxObject;
class CGnuPlotBubbleObject;
class CGnuPlotEllipseObject;
class CGnuPlotErrorBarObject;
class CGnuPlotFinanceBarObject;
class CGnuPlotImageObject;
class CGnuPlotLabelObject;
class CGnuPlotPathObject;
class CGnuPlotPieObject;
class CGnuPlotPointObject;
class CGnuPlotPolygonObject;
class CGnuPlotRectObject;

class CGnuPlotFill;
class CGnuPlotStroke;
class CGnuPlotMark;

class CGnuPlotBBoxRenderer;

//------

#include <CGnuPlotPlotCacheFactoriesDcl.h>
#include <CGnuPlotSamples.h>
#include <CGnuPlotISOSamples.h>
#include <CGnuPlotDummyVars.h>
#include <CGnuPlotSampleVars.h>
#include <CGnuPlotMatrixData.h>
#include <CGnuPlotBars.h>
#include <CGnuPlotFile.h>
#include <CGnuPlotUsingCols.h>
#include <CGnuPlotKeyTitle.h>

#include <CGnuPlotImageStyle.h>
#include <CGnuPlotLabelStyle.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotPointStyle.h>
#include <CGnuPlotTextBoxStyle.h>
#include <CGnuPlotTextStyle.h>
#include <CGnuPlotFilledCurve.h>
#include <CGnuPlotHidden3DData.h>
#include <CGnuPlotSurfaceData.h>
#include <CGnuPlotPrintFile.h>
#include <CGnuPlotAxesData.h>
#include <CGnuPlotStyleData.h>

//------

class CGnuPlotPlot {
 public:
  typedef CGnuPlotTypes::PlotStyle            PlotStyle;
  typedef CGnuPlotTypes::BoxWidthType         BoxWidthType;
  typedef CGnuPlotTypes::FillType             FillType;
  typedef CGnuPlotTypes::FillPattern          FillPattern;
  typedef CGnuPlotTypes::Smooth               Smooth;
  typedef CGnuPlotTypes::SymbolType           SymbolType;
  typedef CGnuPlotTypes::AngleType            AngleType;

  typedef std::vector<CExprValuePtr>          Values;
  typedef std::map<std::string,CExprValuePtr> Params;
  typedef std::vector<CGnuPlotPoint>          Points2D;
  typedef std::map<int,Points2D>              Points3D;
  typedef std::vector<CPoint3D>               Point3DArray;
  typedef std::vector<std::string>            StringArray;

  typedef CGnuPlotCache<CGnuPlotArrowObject>      ArrowCache;
  typedef CGnuPlotCache<CGnuPlotBoxBarObject>     BoxBarCache;
  typedef CGnuPlotCache<CGnuPlotBoxObject>        BoxCache;
  typedef CGnuPlotCache<CGnuPlotBubbleObject>     BubbleCache;
  typedef CGnuPlotCache<CGnuPlotEllipseObject>    EllipseCache;
  typedef CGnuPlotCache<CGnuPlotErrorBarObject>   ErrorBarCache;
  typedef CGnuPlotCache<CGnuPlotFinanceBarObject> FinanceBarCache;
  typedef CGnuPlotCache<CGnuPlotImageObject>      ImageCache;
  typedef CGnuPlotCache<CGnuPlotLabelObject>      LabelCache;
  typedef CGnuPlotCache<CGnuPlotPathObject>       PathCache;
  typedef CGnuPlotCache<CGnuPlotPieObject>        PieCache;
  typedef CGnuPlotCache<CGnuPlotPointObject>      PointCache;
  typedef CGnuPlotCache<CGnuPlotPolygonObject>    PolygonCache;
  typedef CGnuPlotCache<CGnuPlotRectObject>       RectCache;

  typedef std::vector<CGnuPlotArrowObject *>      ArrowObjects;
  typedef std::vector<CGnuPlotBoxBarObject *>     BoxBarObjects;
  typedef std::vector<CGnuPlotBoxObject *>        BoxObjects;
  typedef std::vector<CGnuPlotBubbleObject *>     BubbleObjects;
  typedef std::vector<CGnuPlotEllipseObject *>    EllipseObjects;
  typedef std::vector<CGnuPlotErrorBarObject *>   ErrorBarObjects;
  typedef std::vector<CGnuPlotFinanceBarObject *> FinanceBarObjects;
  typedef std::vector<CGnuPlotImageObject *>      ImageObjects;
  typedef std::vector<CGnuPlotLabelObject *>      LabelObjects;
  typedef std::vector<CGnuPlotPathObject *>       PathObjects;
  typedef std::vector<CGnuPlotPieObject *>        PieObjects;
  typedef std::vector<CGnuPlotPointObject *>      PointObjects;
  typedef std::vector<CGnuPlotPolygonObject *>    PolygonObjects;
  typedef std::vector<CGnuPlotRectObject *>       RectObjects;

  typedef std::vector<CRGBA>            Colors;
  typedef std::vector<double>           Reals;
  typedef std::map<double,Reals>        MappedPoints;
  typedef std::vector<CGnuPlotKeyLabel> KeyLabels;
  typedef std::map<int,CRGBA>           XColor;
  typedef std::map<int,XColor>          YXColor;
  typedef std::map<int,Reals>           RowValues;

 public:
  CGnuPlotPlot(CGnuPlotGroup *group, PlotStyle plotStyle);

  virtual ~CGnuPlotPlot();

  //---

  CGnuPlot *app() const;

  CGnuPlotWindow *window() const;

  CGnuPlotGroup *group() const { return group_; }
  void setGroup(CGnuPlotGroup *g) { group_ = g; }

  //---

  int id() const { return id_; }

  int xind() const { return xind_; }
  int yind() const { return yind_; }
  int zind() const { return zind_; }

  //---

  void setDisplayed(bool b) { displayed_ = b; }
  bool isDisplayed() const { return displayed_; }

  //---

  int setNum() const { return setNum_; }
  void setSetNum(int i) { setNum_ = i; }

  const CGnuPlotFile &dataFile() const { return dataFile_; }
  void setDataFile(const CGnuPlotFile &v) { dataFile_ = v; }

  const CGnuPlotUsingCols &usingCols() const { return usingCols_; }
  void setUsingCols(const CGnuPlotUsingCols &v) { usingCols_ = v; }

  const CGnuPlotKeyTitle &keyTitle() const { return keyTitle_; }
  void setKeyTitle(const CGnuPlotKeyTitle &v) { keyTitle_ = v; }

  const StringArray &functions() const { return functions_; }
  void setFunctions(const StringArray &v) { functions_ = v; }

  const CGnuPlotSamples &samples() const { return samples_; }
  void setSamples(const CGnuPlotSamples &s) { samples_ = s; }

  const CGnuPlotISOSamples &isoSamples() const { return isoSamples_; }
  void setISOSamples(const CGnuPlotISOSamples &s) { isoSamples_ = s; }

  const CGnuPlotSampleVars &sampleVars() const { return sampleVars_; }
  void setSampleVars(const CGnuPlotSampleVars &v) { sampleVars_ = v; }

  const CGnuPlotDummyVars &dummyVars() const { return dummyVars_; }
  void setDummyVars(const CGnuPlotDummyVars &v) { dummyVars_ = v; }

  //---

  PlotStyle style() const { return style_; }
  void setStyle(PlotStyle style);

  //---

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  const CGnuPlotMatrixData &matrixData() const { return matrixData_; }
  CGnuPlotMatrixData &matrixData() { return matrixData_; }
  void setMatrixData(const CGnuPlotMatrixData &m) { matrixData_ = m; }

  //---

  bool isPolar() const { return polar_; }
  void setPolar(bool b) { polar_ = b; }

  bool isParametric() const { return parametric_; }
  void setParametric(bool b) { parametric_ = b; }

  //------

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  bool is2D() const;
  void set2D(bool b);

  bool is3D() const;
  void set3D(bool b);

  //---

  void resetSurface() { surfaceSet_ = false; }

  //---

  int samplesNX() const { int nx, ny; samples().get(nx, ny); return nx; }
  void setSampleNX(int nx) { int nx1, ny; samples().get(nx1, ny); samples_.set(nx, ny); }

  int samplesNY() const { int nx, ny; samples().get(nx, ny); return ny; }
  void setSampleNY(int ny) { int nx, ny1; samples().get(nx, ny1); samples_.set(nx, ny); }

  //---

  const CBBox2D &imageRange() const { return imageRange_; }
  void setImageRange(const CBBox2D &r) { imageRange_ = r; }

  const YXColor &imageColors() const { return imageColors_; }
  void setImageColors(const YXColor &c) { imageColors_ = c; }

  const RowValues &imageValues() const { return imageValues_; }
  void setImageValues(const RowValues &r) { imageValues_ = r; }

  const CGnuPlotImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const CGnuPlotImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  bool isImageStyle() const {
    return (style_ == PlotStyle::IMAGE || style_ == PlotStyle::RGBIMAGE ||
            style_ == PlotStyle::RGBALPHA);
  }

  double imageAngle() const { return imageStyle_.angle().getValue(0.0); }
  void setImageAngle(double a) { imageStyle_.setAngle(a); }

  //---

  const CGnuPlotBars &bars() const { return bars_; }
  void setBars(const CGnuPlotBars &s) { bars_ = s; }

  double barsSize() const { return bars_.size(); }
  void setBarsSize(double s) { bars_.setSize(s); }

  bool barsFront() const { return bars_.isFront(); }
  void setBarsFront(bool b) { bars_.setFront(b); }

  //---

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  //---

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &f) { fillStyle_ = f; }

  //---

  const CGnuPlotLineStyle &lineStyle() const { return lineStyle_; }
  void setLineStyle(const CGnuPlotLineStyle &ls) { lineStyle_ = ls; }

  const COptInt &lineStyleId() const { return lineStyle().ind(); }
  void setLineStyleId(int ind);

  CRGBA lineColor(const CRGBA &c) const { return lineStyle().calcColor(this->group(), c); }
  void setLineColor(const CGnuPlotColorSpec &c) { lineStyle_.setLineColor(c); }

  double lineWidth() const { return lineStyle().calcWidth(); }
  void setLineWidth(double w) { lineStyle_.setLineWidth(w); }

  //---

  SymbolType pointType() const { return lineStyle().calcPointType(); }
  void setPointType(int type) { lineStyle_.setPointType(type); }

  std::string pointTypeStr() const { return lineStyle().pointTypeStr(); }

  double pointSize(double s=1) const { return lineStyle().calcPointSize(s); }
  void setPointSize(double s) { lineStyle_.setPointSize(s); }

  const CGnuPlotPointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const CGnuPlotPointStyle &s) { pointStyle_ = s; }

  const CGnuPlotStyleData &styleData() const { return styleData_; }
  void setStyleData(const CGnuPlotStyleData &v) { styleData_ = v; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  //---

  const CGnuPlotTextBoxStyle &textBoxStyle() const { return styleData_.textBox; }
  void setTextBoxStyle(const CGnuPlotTextBoxStyle &ts) { styleData_.textBox = ts; }

  const CGnuPlotTextStyle &textStyle() const { return styleData_.text; }
  void setTextStyle(const CGnuPlotTextStyle &ts) { styleData_.text = ts; }

  const CGnuPlotLabelStyle &labelStyle() const { return styleData_.label; }
  void setLabelStyle(const CGnuPlotLabelStyle &s) { styleData_.label = s; }

#if 0
  const CGnuPlotBoxWidth &boxWidth() const { return boxWidth_; }
  void setBoxWidth(const CGnuPlotBoxWidth &w) { boxWidth_ = w; }

  CGnuPlotTypes::BoxWidthType getBoxWidthType() const { return boxWidth_.type(); }
  void setBoxWidthType(const CGnuPlotTypes::BoxWidthType &t) {
    styleData_.boxErrorBarsStyleValue.setBoxWidthType(t); }

  double getBoxWidthValue() const { return boxWidth_.width(); }
  void setBoxWidthValue(double w) { boxWidth_.setWidth(w); }

  bool getBoxWidthCalc() const { return boxWidth_.isCalc(); }
  void setBoxWidthCalc(bool b) { boxWidth_.setCalc(b); }
#endif

  const CGnuPlotBoxWidth &boxWidth() const { return styleData_.boxWidth; }

  const CGnuPlotBoxPlotStyleValue  &boxPlotStyleValue () const {
    return styleData_.boxPlotStyleValue ; }
  const CGnuPlotCirclesStyleValue  &circlesStyleValue () const {
    return styleData_.circlesStyleValue ; }
  const CGnuPlotEllipsesStyleValue &ellipsesStyleValue() const {
    return styleData_.ellipsesStyleValue; }
  const CGnuPlotPieChartStyleValue &pieChartStyleValue() const {
    return styleData_.pieChartStyleValue; }
  const CGnuPlotVectorsStyleValue  &vectorsStyleValue () const {
    return styleData_.vectorsStyleValue ; }

  //---

  const std::string &keyTitleString() const { return keyTitle_.str; }
  void setKeyTitleString(const std::string &s) { keyTitle_.str = s; }

  bool isKeyTitleEnabled() const { return keyTitle_.enabled; }
  void setKeyTitleEnabled(bool b) { keyTitle_.enabled = b; }

  //---

  void getKeyLabels(KeyLabels &labels) const;

  //---

  const CGnuPlotFilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const CGnuPlotFilledCurve &c) { filledCurve_ = c; }

  const COptReal &whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }
  void resetWhiskerBars() { whiskerBars_.setInvalid(); }

  //---

  const CGnuPlotHidden3DData &hidden3D() const { return hidden3D_; }
  void setHidden3D(const CGnuPlotHidden3DData &h) { hidden3D_ = h; }

  int trianglePattern3D() const { return hidden3D_.trianglePattern; }
  void setTrianglePattern3D(int i) { hidden3D_.trianglePattern = i; }

  //---

  void init();

  //---

  const Points2D &getPoints2D() const { return points2D_; }

  uint numPoints2D() const { return points2D_.size(); }

  const CGnuPlotPoint &getPoint2D(int i) const {
    assert(i >= 0 && i < int(points2D_.size()));

    return points2D_[i];
  }

#if 0
  void setPoint2DLabel(int i, std::string &str) {
    assert(i >= 0 && i < int(points2D_.size()));

    points2D_[i].setLabel(str);
  }
#endif

  //---

  const Points3D &getPoints3D() const { assert(is3D()); return points3D_; }

  std::pair<int,int> numPoints3D() const;

  const CGnuPlotPoint &getPoint3D(int ix, int iy) const;

  //---

  void clearPoints();

  //---

  int addPoint2D(double x, double y);
  int addPoint2D(const std::vector<double> &rvals);
  int addPoint2D(double x, CExprValuePtr y);
  int addPoint2D(const Values &values, bool discontinuity=false, bool bad=false,
                 const Params &params=Params());

  int addPoint3D(int iy, double x, double y, double z);
  int addPoint3D(int iy, double x, double y, CExprValuePtr z);
  int addPoint3D(int iy, const CPoint3D &p);
  int addPoint3D(int iy, const Values &values, bool discontinuity=false, bool bad=false,
                 const Params &params=Params());

  //---

  void reset3D();

  void smooth();

  void mapPoints(MappedPoints &points);

  void calcXRange(double *xmin, double *xmax) const;
  void calcYRange(double *ymin, double *ymax) const;
  void calcZRange(double *zmin, double *zmax) const;

  bool calcBoundedYRange  (double *ymin, double *ymax) const;
  void recalcBoundedYRange(double *ymin, double *ymax) const;

  void getGroupXRange(double *xmin, double *xmax) const;

  void getPointsRange(CBBox2D &bbox) const;

  //---

  void updateGroupRange();

  const CBBox2D &bbox2D() const { return bbox2D_; }
  const CBBox3D &bbox3D() const { return bbox3D_; }

  //---

  double getXMin() const { return xmin_.getValue(-10); }
  double getXMax() const { return xmax_.getValue( 10); }
  double getYMin() const { return ymin_.getValue(-10); }
  double getYMax() const { return ymax_.getValue( 10); }
  double getZMin() const { return zmin_.getValue(-10); }
  double getZMax() const { return zmax_.getValue( 10); }

  double getBYMin() const { return bymin_.getValue(-10); }
  double getBYMax() const { return bymax_.getValue( 10); }

  //void setXMin(const COptReal &x) { xmin_ = x; }
  //void setXMax(const COptReal &x) { xmax_ = x; }
  //void setYMin(const COptReal &y) { ymin_ = y; }
  //void setYMax(const COptReal &y) { ymax_ = y; }

  const COptReal &cbmin() const { return cbmin_; }
  const COptReal &cbmax() const { return cbmax_; }

  //---

  void setAxesData(const CGnuPlotAxesData &a) { axesData_ = a; }

#if 0
  const CGnuPlotAxesData &axesData() const { return axesData_; }

  const CGnuPlotAxisData &xaxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->xaxis(ind);
  }
  const CGnuPlotAxisData &yaxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->yaxis(ind);
  }
  const CGnuPlotAxisData &zaxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->zaxis(ind);
  }
  const CGnuPlotAxisData &paxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->paxis(ind);
  }
  const CGnuPlotAxisData &taxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->taxis(ind);
  }

  CGnuPlotAxisData &xaxis(int ind) { return axesData_.xaxis(ind); }
  CGnuPlotAxisData &yaxis(int ind) { return axesData_.yaxis(ind); }
  CGnuPlotAxisData &zaxis(int ind) { return axesData_.zaxis(ind); }
  CGnuPlotAxisData &paxis(int ind) { return axesData_.paxis(ind); }
  CGnuPlotAxisData &taxis(int ind) { return axesData_.taxis(ind); }
#endif

  //---

  void getXRange(double *xmin, double *xmax) const {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);
    *xmin = th->axesData_.xaxis(1).min().getValue(-10);
    *xmax = th->axesData_.xaxis(1).max().getValue( 10);
  }
  void getYRange(double *ymin, double *ymax) const {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);
    *ymin = th->axesData_.yaxis(1).min().getValue(-10);
    *ymax = th->axesData_.yaxis(1).max().getValue( 10);
  }
  void getZRange(double *zmin, double *zmax) const {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);
    *zmin = th->axesData_.zaxis(1).min().getValue(-10);
    *zmax = th->axesData_.zaxis(1).max().getValue( 10);
  }
  void getParametricTRange(double *tmin, double *tmax) const {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);
    *tmin = th->axesData_.taxis(1).min().getValue(-5);
    *tmax = th->axesData_.taxis(1).max().getValue( 5);
  }
  void getPolarTRange(double *tmin, double *tmax) const {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);
    *tmin = th->axesData_.taxis(1).min().getValue(0);

    if (isAngleDegrees())
      *tmax = th->axesData_.taxis(1).max().getValue(360.0);
    else
      *tmax = th->axesData_.taxis(1).max().getValue(2*M_PI);
  }

  void setXRangeMin(double x) { axesData_.xaxis(1).setMin(x); }
  void setYRangeMin(double y) { axesData_.yaxis(1).setMin(y); }
  void setXRangeMax(double x) { axesData_.xaxis(1).setMax(x); }
  void setYRangeMax(double y) { axesData_.yaxis(1).setMax(y); }

  //---

  bool isCacheActive() const { return cacheActive_; }
  void setCacheActive(bool b) { cacheActive_ = b; }

  void clearCaches();

  void updateArrowCacheSize     (int n);
  void updateBoxBarCacheSize    (int n);
  void updateBoxCacheSize       (int n);
  void updateBubbleCacheSize    (int n);
  void updateEllipseCacheSize   (int n);
  void updateErrorBarCacheSize  (int n);
  void updateFinanceBarCacheSize(int n);
  void updateImageCacheSize     (int n);
  void updateLabelCacheSize     (int n);
  void updatePathCacheSize      (int n);
  void updatePieCacheSize       (int n);
  void updatePointCacheSize     (int n);
  void updatePolygonCacheSize   (int n);
  void updateRectCacheSize      (int n);

  void clearBoxCache();
  void clearPieCache();
  void clearRectCache();

  void resetArrowCache();
  void resetBubbleCache();
  void resetLabelCache();
  void resetPolygonCache();
  void resetRectCache();

  const ArrowObjects      &arrowObjects     () const { return arrowCache_     .objects(); }
  const BoxBarObjects     &boxBarObjects    () const { return boxBarCache_    .objects(); }
  const BoxObjects        &boxObjects       () const { return boxCache_       .objects(); }
  const BubbleObjects     &bubbleObjects    () const { return bubbleCache_    .objects(); }
  const EllipseObjects    &ellipseObjects   () const { return ellipseCache_   .objects(); }
  const ErrorBarObjects   &errorBarObjects  () const { return errorBarCache_  .objects(); }
  const FinanceBarObjects &financeBarObjects() const { return financeBarCache_.objects(); }
  const ImageObjects      &imageObjects     () const { return imageCache_     .objects(); }
  const LabelObjects      &labelObjects     () const { return labelCache_     .objects(); }
  const PathObjects       &pathObjects      () const { return pathCache_      .objects(); }
  const PieObjects        &pieObjects       () const { return pieCache_       .objects(); }
  const PointObjects      &pointObjects     () const { return pointCache_     .objects(); }
  const PolygonObjects    &polygonObjects   () const { return polygonCache_   .objects(); }
  const RectObjects       &rectObjects      () const { return rectCache_      .objects(); }

  //---

  typedef std::map<std::string, CGnuPlotStyleValue *> StyleValues;

  //---

  struct DrawHistogramData {
    double x2         { 0.0 };   // start x
    double y2         { 0.0 };   // start y
    double d          { 1.0 };   // delta
    double w          { 1.0 };   // width
    int    np         { 1 };     // num plots
    int    i          { 1 };     // plot index
    double xb         { 1.0 };   // border
    bool   horizontal { false }; // horizontal
  };

  //---

  struct Delta {
    double dx { 1 };
    double dy { 1 };
    double dz { 1 };
    double dt { 1 };

    Delta(double dx1, double dy1, double dz1, double dt1) :
     dx(dx1), dy(dy1), dz(dz1), dt(dt1) {
    }
  };

  //---

  void initRenderer(CGnuPlotRenderer *renderer);

  void printValues() const;
  void printContourValues() const;
  void printSurfaceValues() const;

  virtual void draw();

  bool isSurfaceData() const;

  void draw2D(CGnuPlotRenderer *renderer);
  void draw3D(CGnuPlotRenderer *renderer);

  void drawContour(CGnuPlotRenderer *renderer);
  void drawSurface(CGnuPlotRenderer *renderer);

  void initContour() const;
  void initSurface() const;

  void drawClusteredHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &data);
  void drawErrorBarsHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &data);

  void drawStackedHistogram(CGnuPlotRenderer *renderer, int i,
                            const CBBox2D &bbox, bool isColumn);

  void drawBars(CGnuPlotRenderer *renderer);

  double getXSpacing() const;

  CGnuPlotArrowObject      *createArrowObject     () const;
  CGnuPlotBoxBarObject     *createBoxBarObject    () const;
  CGnuPlotEndBar           *createEndBar          () const;
  CGnuPlotBoxObject        *createBoxObject       () const;
  CGnuPlotBubbleObject     *createBubbleObject    () const;
  CGnuPlotEllipseObject    *createEllipseObject   () const;
  CGnuPlotErrorBarObject   *createErrorBarObject  () const;
  CGnuPlotFinanceBarObject *createFinanceBarObject() const;
  CGnuPlotImageObject      *createImageObject     () const;
  CGnuPlotLabelObject      *createLabelObject     () const;
  CGnuPlotPathObject       *createPathObject      () const;
  CGnuPlotPieObject        *createPieObject       () const;
  CGnuPlotPointObject      *createPointObject     () const;
  CGnuPlotPolygonObject    *createPolygonObject   () const;
  CGnuPlotRectObject       *createRectObject      () const;

  CGnuPlotFill   *createFill  () const;
  CGnuPlotStroke *createStroke() const;
  CGnuPlotMark   *createMark  () const;

  bool mapPoint3D(const CGnuPlotPoint &p, CPoint3D &p1, int &ind) const;

  AngleType angleType() const { return angleType_; }
  void setAngleType(AngleType type) { angleType_ = type; }

  bool isAngleDegrees() const { return (angleType() == CGnuPlotTypes::AngleType::DEGREES); }

  CPoint2D convertPolarAxisPoint(const CPoint2D &p, bool &inside) const;

  CPoint2D convertPolarPoint(const CPoint2D &p) const;

  double angleToRad(double a) const;
  double angleToDeg(double a) const;

  void setStyleValue(const std::string &name, CGnuPlotStyleValue *value);
  CGnuPlotStyleValue *styleValue(const std::string &name) const;

  //------

  CGnuPlotTypes::Mapping mapping() const { return mapping_; }
  void setMapping(CGnuPlotTypes::Mapping m) { mapping_ = m; }

  //---

  const CGnuPlotSurfaceData &surfaceData() const { return surfaceData_; }
  void setSurfaceData(const CGnuPlotSurfaceData &d) { surfaceData_ = d; }

  //---

  const CGnuPlotContourData &contourData() const { return contourData_; }
  void setContourData(const CGnuPlotContourData &d) { contourData_ = d; }

  bool isContourEnabled() const { return contourData_.isEnabled(); }
  void setContourEnabled(bool b) { contourData_.setEnabled(b); }

  int getContourLevels() const { return contourData_.levels().getValue(10); }
  void setContourLevels(int n);

  //------

  const CGnuPlotPrintFile &tableFile() const { return tableFile_; }
  void setTableFile(const CGnuPlotPrintFile &v) { tableFile_ = v; }

  //------

  virtual void mousePress  (const CGnuPlotMouseEvent &mouseEvent);
  virtual void mouseMove   (const CGnuPlotMouseEvent &mouseEvent, bool pressed);
  virtual void mouseRelease(const CGnuPlotMouseEvent &mouseEvent);

  virtual bool mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData);

  virtual bool mouseProbe(CGnuPlotProbeEvent &probeEvent);

  //------

  int newHistogramId() const { return newHistogramId_; }
  void setNewHistogramId(int i) { newHistogramId_ = i; }

  //------

  const Delta &delta() const { return delta_; }
  void setDelta(const Delta &v) { delta_ = v; }

  const CGnuPlotRotate &rotate() const { return rotate_; }
  void setRotate(const CGnuPlotRotate &v) { rotate_ = v; }

  const COptPoint3D &center() const { return center_; }
  void setCenter(const COptPoint3D &v) { center_ = v; }

  const Point3DArray &originArray() const { return originArray_; }
  void setOriginArray(const Point3DArray &o) { originArray_ = o; }

  //------

  void printPoints();

 private:
  bool renderBBox(CGnuPlotBBoxRenderer &brenderer) const;

 protected:
  typedef std::vector<CPoint2D> Points;

  static int nextId_;

  CGnuPlotGroup*           group_;                            // parent group
  PlotStyle                style_ { PlotStyle::POINTS};       // plot style
  int                      id_;                               // unique id
  bool                     displayed_ { true };               // is displayed
  int                      setNum_    { 0 };                  // set num
  CGnuPlotFile             dataFile_;                         // data file
  CGnuPlotUsingCols        usingCols_;                        // using column data
  CGnuPlotKeyTitle         keyTitle_;                         // title on key
  StringArray              functions_;                        // functions
  CGnuPlotSamples          samples_;
  CGnuPlotISOSamples       isoSamples_;
  CGnuPlotSampleVars       sampleVars_;
  CGnuPlotDummyVars        dummyVars_;
  Points2D                 points2D_;                         // 2D points
  Points3D                 points3D_;                         // 3D points
  bool                     binary_ { false };
  CGnuPlotMatrixData       matrixData_;
  bool                     rowheaders_ { false };
  bool                     columnheaders_ { false };
  bool                     polar_  { false };
  bool                     enhanced_ { true };

  CBBox2D                  imageRange_;                       // image range
  YXColor                  imageColors_;                      // image colors
  RowValues                imageValues_;                      // image values
  CGnuPlotImageStyle       imageStyle_;

  CGnuPlotBars             bars_;
  CGnuPlotFilledCurve      filledCurve_;                      // filled curve data
  CGnuPlotFillStyle        fillStyle_;                        // fill style
  CGnuPlotLineStyle        lineStyle_;                        // line style
  CGnuPlotPointStyle       pointStyle_;                       // point style
  CGnuPlotStyleData        styleData_;                        // style data
  CGnuPlotKeyData          keyData_;                          // key data

  int                      xind_ { 1 };                       // xaxis index
  int                      yind_ { 1 };                       // yaxis index
  int                      zind_ { 1 };                       // yaxis index
  COptReal                 xmin_, xmax_;                      // calculated points x range
  COptReal                 ymin_, ymax_;                      // calculated points y range
  COptReal                 zmin_, zmax_;                      // calculated points z range
  COptReal                 bymin_, bymax_;                    // calculated points bounded y range
  COptReal                 cbmin_, cbmax_;                    // color box range
  CGnuPlotAxesData         axesData_;

  CBBox2D                  bbox2D_ { 0, 0, 1, 1 };            // bounding box (2D)
  CBBox3D                  bbox3D_ { 0, 0, 0, 1, 1, 1 };      // bounding box (3D)
  Smooth                   smooth_ { Smooth::NONE };          // smooth data

  CGnuPlotContour          contour_;                          // contour data
  bool                     contourSet_ { false };
  CGnuPlotSurface          surface_;
  bool                     surfaceSet_ { false };

  CGnuPlotHidden3DData     hidden3D_;
  COptReal                 whiskerBars_;                      // whisker bar data

  // object caches
  bool                     cacheActive_ { true };
  ArrowCache               arrowCache_;
  BoxBarCache              boxBarCache_;
  BoxCache                 boxCache_;
  BubbleCache              bubbleCache_;
  EllipseCache             ellipseCache_;
  ErrorBarCache            errorBarCache_;
  FinanceBarCache          financeBarCache_;
  ImageCache               imageCache_;
  LabelCache               labelCache_;
  PathCache                pathCache_;
  PieCache                 pieCache_;
  PointCache               pointCache_;
  PolygonCache             polygonCache_;
  RectCache                rectCache_;

  StyleValues              styleValues_;
  AngleType                angleType_ { CGnuPlotTypes::AngleType::RADIANS };
  bool                     parametric_ { false };
  CGnuPlotTypes::Mapping   mapping_ { CGnuPlotTypes::Mapping::CARTESIAN_MAPPING };
  CGnuPlotSurfaceData      surfaceData_;
  CGnuPlotContourData      contourData_;
  CGnuPlotPrintFile        tableFile_;
  int                      newHistogramId_ { -1 };
  Delta                    delta_ { 1, 1, 1, 1 };
  CGnuPlotRotate           rotate_;
  COptPoint3D              center_;
  Point3DArray             originArray_;
};

typedef std::shared_ptr<CGnuPlotPlot> CGnuPlotPlotP;

//------

#include <CGnuPlotPlotCacheFactoriesDef.h>

#endif
