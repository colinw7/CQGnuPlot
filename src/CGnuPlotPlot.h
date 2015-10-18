#ifndef CGnuPlotPlot_H
#define CGnuPlotPlot_H

#include <CGnuPlot.h>
#include <CGnuPlotContour.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotPoint.h>
#include <CGnuPlotCache.h>
#include <CGnuPlotKey.h>

#include <CExpr.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <vector>
#include <map>

typedef unsigned char uchar;

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
class CGnuPlotStyleAdjacencyRenderer;
class CGnuPlotStyleChordDiagramRenderer;

class CAdjacency;
class ChordDiagram;

//------

template<>
class CGnuPlotCacheFactory<CGnuPlotArrowObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotArrowObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBoxBarObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBoxBarObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBoxObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBoxObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBubbleObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBubbleObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotEllipseObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotEllipseObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotErrorBarObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotErrorBarObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotFinanceBarObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotFinanceBarObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotLabelObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotLabelObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPathObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPathObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPieObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPieObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPointObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPointObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPolygonObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPolygonObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotRectObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotRectObject *make();

 private:
  CGnuPlotPlot *plot_;
};

//------

class CGnuPlotAdjacencyData {
 public:
  CGnuPlotAdjacencyData() { }
 ~CGnuPlotAdjacencyData();

  CAdjacency *adjacency() const { return adjacency_; }
  void setAdjacency(CAdjacency *a) { adjacency_ = a; }

  CGnuPlotStyleAdjacencyRenderer *renderer() const { return renderer_; }
  void setRenderer(CGnuPlotStyleAdjacencyRenderer *r) { renderer_ = r; }

 private:
  CAdjacency                     *adjacency_ { 0 };
  CGnuPlotStyleAdjacencyRenderer *renderer_  { 0 };
};

//------

class CGnuPlotChordDiagramData {
 public:
  CGnuPlotChordDiagramData() { }
 ~CGnuPlotChordDiagramData();

  ChordDiagram *chordDiagram() const { return chord_; }
  void setChordDiagram(ChordDiagram *c) { chord_ = c; }

  CGnuPlotStyleChordDiagramRenderer *renderer() const { return renderer_; }
  void setRenderer(CGnuPlotStyleChordDiagramRenderer *r) { renderer_ = r; }

 private:
  ChordDiagram                      *chord_    { 0 };
  CGnuPlotStyleChordDiagramRenderer *renderer_ { 0 };
};

//------

class CGnuPlotPlot {
 public:
  typedef CGnuPlotTypes::PlotStyle            PlotStyle;
  typedef CGnuPlotTypes::BoxWidthType         BoxWidthType;
  typedef CGnuPlotTypes::FillType             FillType;
  typedef CGnuPlotTypes::FillPattern          FillPattern;
  typedef CGnuPlotTypes::Smooth               Smooth;
  typedef CGnuPlotTypes::SymbolType           SymbolType;
  typedef CGnuPlot::Bars                      Bars;
  typedef std::vector<CExprValuePtr>          Values;
  typedef std::map<std::string,CExprValuePtr> Params;
  typedef std::vector<CGnuPlotPoint>          Points2D;
  typedef std::map<int,Points2D>              Points3D;
  typedef std::vector<CRGBA>                  ImageData;

  typedef CGnuPlotCache<CGnuPlotArrowObject>      ArrowCache;
  typedef CGnuPlotCache<CGnuPlotBoxBarObject>     BoxBarCache;
  typedef CGnuPlotCache<CGnuPlotBoxObject>        BoxCache;
  typedef CGnuPlotCache<CGnuPlotBubbleObject>     BubbleCache;
  typedef CGnuPlotCache<CGnuPlotEllipseObject>    EllipseCache;
  typedef CGnuPlotCache<CGnuPlotErrorBarObject>   ErrorBarCache;
  typedef CGnuPlotCache<CGnuPlotFinanceBarObject> FinanceBarCache;
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
  typedef std::vector<CGnuPlotLabelObject *>      LabelObjects;
  typedef std::vector<CGnuPlotPathObject *>       PathObjects;
  typedef std::vector<CGnuPlotPieObject *>        PieObjects;
  typedef std::vector<CGnuPlotPointObject *>      PointObjects;
  typedef std::vector<CGnuPlotPolygonObject *>    PolygonObjects;
  typedef std::vector<CGnuPlotRectObject *>       RectObjects;

  typedef std::vector<double>           DoubleVector;
  typedef std::map<double,DoubleVector> MappedPoints;
  typedef std::vector<CGnuPlotKeyLabel> KeyLabels;

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

  PlotStyle getStyle() const { return style_; }
  void setStyle(PlotStyle style);

  //---

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool b) { matrix_ = b; }

  bool isPolar() const { return polar_; }
  void setPolar(bool b) { polar_ = b; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  bool is3D() const;

  //---

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

  //---

  void resetSurface() { surfaceSet_ = false; }

  //---

  const ImageData &imageData() const { return imageData_; }
  void setImageData(const ImageData &imageData) { imageData_ = imageData; }

  const CGnuPlotImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const CGnuPlotImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  bool isImageStyle() const {
    return (style_ == PlotStyle::IMAGE || style_ == PlotStyle::RGBIMAGE ||
            style_ == PlotStyle::RGBALPHA);
  }

  double imageAngle() const { return imageStyle_.a.getValue(0.0); }
  void setImageAngle(double a) { imageStyle_.a = a; }

  //---

  const Bars &bars() const { return bars_; }
  void setBars(const Bars &s) { bars_ = s; }

  double barsSize() const { return bars_.size(); }
  void setBarsSize(double s) { bars_.setSize(s); }

  bool barsFront() const { return bars_.isFront(); }
  void setBarsFront(bool b) { bars_.setFront(b); }

  //---

  const CGnuPlotBoxWidth &boxWidth() const { return boxWidth_; }
  void setBoxWidth(const CGnuPlotBoxWidth &w) { boxWidth_ = w; }

  CGnuPlotTypes::BoxWidthType getBoxWidthType() const { return boxWidth_.type(); }
  void setBoxWidthType(const CGnuPlotTypes::BoxWidthType &t) { boxWidth_.setType(t); }

  double getBoxWidthValue() const { return boxWidth_.width(); }
  void setBoxWidthValue(double w) { boxWidth_.setWidth(w); }

  bool getBoxWidthCalc() const { return boxWidth_.isCalc(); }
  void setBoxWidthCalc(bool b) { boxWidth_.setCalc(b); }

  //---

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  //---

  void setBoxPlot(const CGnuPlotBoxPlot &b) { boxPlot_ = b; }
  const CGnuPlotBoxPlot &getBoxPlot() const { return boxPlot_; }

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

  //---

  const CGnuPlotArrowStyle &arrowStyle() const { return styleData_.arrow; }
  void setArrowStyle(const CGnuPlotArrowStyle &as) { styleData_.arrow = as; }

  //---

  const CGnuPlotTextBoxStyle &textBoxStyle() const { return styleData_.textBox; }
  void setTextBoxStyle(const CGnuPlotTextBoxStyle &ts) { styleData_.textBox = ts; }

  //---

  const CGnuPlotTextStyle &textStyle() const { return styleData_.text; }
  void setTextStyle(const CGnuPlotTextStyle &ts) { styleData_.text = ts; }

  //---

  const CGnuPlotEllipseStyle &ellipseStyle() const { return styleData_.ellipse; }
  void setEllipseStyle(const CGnuPlotEllipseStyle &es) { styleData_.ellipse = es; }

  //---

  const CGnuPlotLabelStyle &labelStyle() const { return styleData_.label; }
  void setLabelStyle(const CGnuPlotLabelStyle &s) { styleData_.label = s; }

  //---

  const CGnuPlotPieStyle &pieStyle() const { return styleData_.pie; }
  void setPieStyle(const CGnuPlotPieStyle &s) { styleData_.pie = s; }

  //---

  const CGnuPlotKeyTitle &keyTitle() const { return keyTitle_; }

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

  void setPoint2DLabel(int i, std::string &str) {
    assert(i >= 0 && i < int(points2D_.size()));

    points2D_[i].setLabel(str);
  }

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
  int addPoint3D(int iy, const Values &values, bool discontinuity=false, bool bad=false);

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

  const CBBox2D &getBBox() const { return bbox_; }

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

  bool isCacheActive() const { return cacheActive_; }
  void setCacheActive(bool b) { cacheActive_ = b; }

  void updateArrowCacheSize     (int n);
  void updateBoxBarCacheSize    (int n);
  void updateBoxCacheSize       (int n);
  void updateBubbleCacheSize    (int n);
  void updateEllipseCacheSize   (int n);
  void updateErrorBarCacheSize  (int n);
  void updateFinanceBarCacheSize(int n);
  void updateLabelCacheSize     (int n);
  void updatePathCacheSize      (int n);
  void updatePieCacheSize       (int n);
  void updatePointCacheSize     (int n);
  void updatePolygonCacheSize   (int n);
  void updateRectCacheSize      (int n);

  const ArrowObjects      &arrowObjects     () const { return arrowCache_     .objects(); }
  const BoxBarObjects     &boxBarObjects    () const { return boxBarCache_    .objects(); }
  const BoxObjects        &boxObjects       () const { return boxCache_       .objects(); }
  const BubbleObjects     &bubbleObjects    () const { return bubbleCache_    .objects(); }
  const EllipseObjects    &ellipseObjects   () const { return ellipseCache_   .objects(); }
  const ErrorBarObjects   &errorBarObjects  () const { return errorBarCache_  .objects(); }
  const FinanceBarObjects &financeBarObjects() const { return financeBarCache_.objects(); }
  const LabelObjects      &labelObjects     () const { return labelCache_     .objects(); }
  const PathObjects       &pathObjects      () const { return pathCache_      .objects(); }
  const PieObjects        &pieObjects       () const { return pieCache_       .objects(); }
  const PointObjects      &pointObjects     () const { return pointCache_     .objects(); }
  const PolygonObjects    &polygonObjects   () const { return polygonCache_   .objects(); }
  const RectObjects       &rectObjects      () const { return rectCache_      .objects(); }

  //---

  struct StyleValue {
    virtual ~StyleValue() { }
  };

  typedef std::map<std::string, StyleValue *> StyleValues;

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

  void initRenderer(CGnuPlotRenderer *renderer);

  void printValues() const;
  void printSurfaceValues() const;
  void printContourValues() const;

  virtual void draw();

  bool isSurfaceData() const;

  void draw2D(CGnuPlotRenderer *renderer);
  void draw3D(CGnuPlotRenderer *renderer);

  void drawContour(CGnuPlotRenderer *renderer);
  void drawSurface(CGnuPlotRenderer *renderer);

  void initContour() const;

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

  void setStyleValue(const std::string &name, StyleValue *value);
  StyleValue *styleValue(const std::string &name) const;

  //------

  bool isParametric() const { return parametric_; }
  void setParametric(bool b) { parametric_ = b; }

  //------

  CGnuPlotTypes::Mapping mapping() const { return mapping_; }
  void setMapping(CGnuPlotTypes::Mapping m) { mapping_ = m; }

  //---

  const CGnuPlotSurfaceData &surfaceData() const { return surfaceData_; }
  void setSurfaceData(const CGnuPlotSurfaceData &d) { surfaceData_ = d; }

  bool isSurfaceEnabled() const { return surfaceData_.isEnabled(); }
  void setSurfaceEnabled(bool b) { surfaceData_.setEnabled(b); }

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

  virtual bool mouseTip  (const CGnuPlotTypes::InsideData &insideData, CGnuPlotTipData &tipData);
  virtual void mousePress(const CGnuPlotTypes::InsideData &insideData);

  //------

  const CGnuPlotAdjacencyData &adjacencyData() const { return adjacencyData_; }

  void setAdjacency(CAdjacency *a) { adjacencyData_.setAdjacency(a); }

  void setAdjacencyRenderer(CGnuPlotStyleAdjacencyRenderer *r) { adjacencyData_.setRenderer(r); }

  //------

  const CGnuPlotChordDiagramData &chordDiagramData() const { return chordDiagramData_; }

  void setChordDiagram(ChordDiagram *c) { chordDiagramData_.setChordDiagram(c); }

  void setChordDiagramRenderer(CGnuPlotStyleChordDiagramRenderer *r) {
    chordDiagramData_.setRenderer(r);
  }

  //------

  int newHistogramId() const { return newHistogramId_; }
  void setNewHistogramId(int i) { newHistogramId_ = i; }

 private:
  bool renderBBox(CGnuPlotBBoxRenderer &brenderer) const;

 protected:
  typedef std::vector<CPoint2D>         Points;
  typedef std::pair<Points,CRGBA>       PointsColor;
  typedef std::pair<double,PointsColor> ZPoints;
  typedef std::vector<ZPoints>          ZPointsArray;
  typedef std::map<double,ZPointsArray> ZPolygons;
  typedef std::map<int,Points>          IPoints;
  typedef std::map<int,IPoints>         IJPoints;

  static int nextId_;

  CGnuPlotGroup*           group_;                            // parent group
  PlotStyle                style_ { PlotStyle::POINTS};       // plot style
  int                      id_;                               // unique id
  int                      ind_       { 0 };                  // axis index
  bool                     displayed_ { true };               // is displayed
  Points2D                 points2D_;                         // 2D points
  Points3D                 points3D_;                         // 3D points
  ImageData                imageData_;                        // image data
  bool                     binary_ { false };
  bool                     matrix_ { false };
  bool                     polar_  { false };
  bool                     enhanced_ { true };
  CGnuPlotImageStyle       imageStyle_;
  Bars                     bars_;
  CGnuPlotBoxWidth         boxWidth_;                         // box widths
  CGnuPlotFilledCurve      filledCurve_;                      // filled curve data
  CGnuPlotFillStyle        fillStyle_;                        // fill style
  CGnuPlotLineStyle        lineStyle_;                        // line style
  CGnuPlotPointStyle       pointStyle_;                       // point style
  CGnuPlotStyleData        styleData_;                        // style data
  CGnuPlotKeyTitle         keyTitle_;                         // title on key
  int                      xind_ { 1 };                       // xaxis index
  int                      yind_ { 1 };                       // yaxis index
  int                      zind_ { 1 };                       // yaxis index
  COptReal                 xmin_, xmax_;                      // calculated points x range
  COptReal                 ymin_, ymax_;                      // calculated points y range
  COptReal                 zmin_, zmax_;                      // calculated points z range
  COptReal                 bymin_, bymax_;                    // calculated points bounded y range
  COptReal                 cbmin_, cbmax_;                    // color box range
  CBBox2D                  bbox_ { 0, 0, 1, 1 };              // bounding box
  Smooth                   smooth_ { Smooth::NONE };          // smooth data
  CGnuPlotBoxPlot          boxPlot_;
  CGnuPlotContour          contour_;                          // contour data
  bool                     contourSet_ { false };
  ZPolygons                surfaceZPolygons_;                 // surface data
  IJPoints                 surfaceIJPoints_;                  // surface data
  bool                     surfaceSet_ { false };
  COptReal                 surfaceZMin_, surfaceZMax_;
  CGnuPlotHidden3DData     hidden3D_;
  COptReal                 whiskerBars_;                      // whisker bar data
  bool                     cacheActive_ { true };
  ArrowCache               arrowCache_;
  BoxBarCache              boxBarCache_;
  BoxCache                 boxCache_;
  BubbleCache              bubbleCache_;
  EllipseCache             ellipseCache_;
  ErrorBarCache            errorBarCache_;
  FinanceBarCache          financeBarCache_;
  LabelCache               labelCache_;
  PathCache                pathCache_;
  PieCache                 pieCache_;
  PointCache               pointCache_;
  PolygonCache             polygonCache_;
  RectCache                rectCache_;
  StyleValues              styleValues_;
  bool                     parametric_ { false };
  CGnuPlotTypes::Mapping   mapping_ { CGnuPlotTypes::Mapping::CARTESIAN_MAPPING };
  CGnuPlotSurfaceData      surfaceData_;
  CGnuPlotContourData      contourData_;
  CGnuPlotPrintFile        tableFile_;
  CGnuPlotAdjacencyData    adjacencyData_;
  CGnuPlotChordDiagramData chordDiagramData_;
  int                      newHistogramId_ { -1 };

 private:
  CGnuPlotStyleAdjacencyRenderer *arenderer_;
};

//------

inline CGnuPlotArrowObject *
CGnuPlotCacheFactory<CGnuPlotArrowObject>::
make()
{
  return plot_->createArrowObject();
}

inline CGnuPlotBoxBarObject *
CGnuPlotCacheFactory<CGnuPlotBoxBarObject>::
make()
{
  return plot_->createBoxBarObject();
}

inline CGnuPlotBoxObject *
CGnuPlotCacheFactory<CGnuPlotBoxObject>::
make()
{
  return plot_->createBoxObject();
}

inline CGnuPlotBubbleObject *
CGnuPlotCacheFactory<CGnuPlotBubbleObject>::
make()
{
  return plot_->createBubbleObject();
}

inline CGnuPlotEllipseObject *
CGnuPlotCacheFactory<CGnuPlotEllipseObject>::
make()
{
  return plot_->createEllipseObject();
}

inline CGnuPlotErrorBarObject *
CGnuPlotCacheFactory<CGnuPlotErrorBarObject>::
make()
{
  return plot_->createErrorBarObject();
}

inline CGnuPlotFinanceBarObject *
CGnuPlotCacheFactory<CGnuPlotFinanceBarObject>::
make()
{
  return plot_->createFinanceBarObject();
}

inline CGnuPlotLabelObject *
CGnuPlotCacheFactory<CGnuPlotLabelObject>::
make()
{
  return plot_->createLabelObject();
}

inline CGnuPlotPathObject *
CGnuPlotCacheFactory<CGnuPlotPathObject>::
make()
{
  return plot_->createPathObject();
}

inline CGnuPlotPieObject *
CGnuPlotCacheFactory<CGnuPlotPieObject>::
make()
{
  return plot_->createPieObject();
}

inline CGnuPlotPointObject *
CGnuPlotCacheFactory<CGnuPlotPointObject>::
make()
{
  return plot_->createPointObject();
}

inline CGnuPlotPolygonObject *
CGnuPlotCacheFactory<CGnuPlotPolygonObject>::
make()
{
  return plot_->createPolygonObject();
}

inline CGnuPlotRectObject *
CGnuPlotCacheFactory<CGnuPlotRectObject>::
make()
{
  return plot_->createRectObject();
}

#endif
