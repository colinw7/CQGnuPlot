#ifndef CGnuPlotPlot_H
#define CGnuPlotPlot_H

#include <CGnuPlot.h>
#include <CGnuPlotContour.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotPoint.h>
#include <CGnuPlotCache.h>

#include <CExpr.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <vector>
#include <map>

typedef unsigned char uchar;

class CGnuPlotWindow;
class CGnuPlotGroup;
class CGnuPlotBar;
class CGnuPlotPie;
class CGnuPlotBubble;

typedef CRefPtr<CExprValue> CExprValueP;

//------

template<>
class CGnuPlotCacheFactory<CGnuPlotBar> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBar *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPie> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPie *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBubble> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBubble *make();

 private:
  CGnuPlotPlot *plot_;
};

//------

class CGnuPlotPlot {
 public:
  typedef CGnuPlot::PlotStyle           PlotStyle;
  typedef CGnuPlotTypes::BoxWidthType   BoxWidthType;
  typedef CGnuPlotTypes::FillType       FillType;
  typedef CGnuPlotTypes::FillPattern    FillPattern;
  typedef CGnuPlotTypes::Smooth         Smooth;
  typedef CGnuPlotTypes::SymbolType     SymbolType;
  typedef CGnuPlot::Bars                Bars;
  typedef CGnuPlot::AxesData            AxesData;
  typedef CGnuPlot::FilledCurve         FilledCurve;
  typedef CGnuPlot::Hidden3DData        Hidden3DData;
  typedef CGnuPlotCache<CGnuPlotBar>    BarCache;
  typedef CGnuPlotCache<CGnuPlotPie>    PieCache;
  typedef CGnuPlotCache<CGnuPlotBubble> BubbleCache;
  typedef std::vector<CExprValueP>      Values;
  typedef std::vector<CGnuPlotPoint>    Points2D;
  typedef std::map<int,Points2D>        Points3D;
  typedef std::vector<uchar>            ImageData;
  typedef std::vector<CGnuPlotBar *>    BarObjects;
  typedef std::vector<CGnuPlotPie *>    PieObjects;
  typedef std::vector<CGnuPlotBubble *> BubbleObjects;

 public:
  CGnuPlotPlot(CGnuPlotGroup *group);

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
  void setMatrix(bool m) { matrix_ = m; }

  bool is3D() const;

  //---

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

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

  double barsSize() const { return bars_.size; }
  void setBarsSize(double s) { bars_.size = s; }

  bool barsFront() const { return bars_.front; }
  void setBarsFront(bool b) { bars_.front = b; }

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

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &f) { fillStyle_ = f; }

  const FillType &fillType() const { return fillStyle_.style(); }
  void setFillType(const FillType &f) { fillStyle_.setStyle(f); }

  const FillPattern &fillPattern() const { return fillStyle_.pattern(); }
  void setFillPattern(const FillPattern &f) { fillStyle_.setPattern(f); }

  //---

  const CGnuPlotLineStyle &lineStyle() const { return  lineStyle_; }
  void setLineStyle(const CGnuPlotLineStyle &ls) { lineStyle_ = ls; }

  int lineStyleId() const { return lineStyle().ind(); }
  void setLineStyleId(int ind);

  const CRGBA &lineColor(const CRGBA &c) const { return lineStyle().calcColor(c); }
  void setLineColor(const CGnuPlotColorSpec &c) { lineStyle_.setColor(c); }

  double lineWidth() const { return lineStyle().width(); }
  void setLineWidth(double w) { lineStyle_.setWidth(w); }

  //---

  SymbolType pointType() const { return lineStyle().pointType(); }
  void setPointType(SymbolType type) { lineStyle_.setPointType(type); }

  double pointSize() const { return lineStyle().pointSize(); }
  void setPointSize(double s) { lineStyle_.setPointSize(s); }

  const CGnuPlotPointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const CGnuPlotPointStyle &s) { pointStyle_ = s; }

  //---

  const CGnuPlotArrowStyle &arrowStyle() const { return arrowStyle_; }
  void setArrowStyle(const CGnuPlotArrowStyle &as) { arrowStyle_ = as; }

  //---

  const std::string &keyTitle() const { return keyTitle_; }
  void setKeyTitle(const std::string &s) { keyTitle_ = s; }

  //---

  const FilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const FilledCurve &c) { filledCurve_ = c; }

  double whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }

  //---

  const Hidden3DData &hidden3D() const { return hidden3D_; }
  void setHidden3D(const Hidden3DData &h) { hidden3D_ = h; }

  int trianglePattern3D() const { return hidden3D_.trianglePattern; }
  void setTrianglePattern3D(int i) { hidden3D_.trianglePattern = i; }

  //---

  void init();

  //---

  const Points2D &getPoints2D() const { assert(! is3D()); return points2D_; }

  uint numPoints2D() const { assert(! is3D()); return points2D_.size(); }

  const CGnuPlotPoint &getPoint2D(int i) const { assert(! is3D()); return points2D_[i]; }

  //---

  const Points3D &getPoints3D() const { assert(is3D()); return points3D_; }

  std::pair<int,int> numPoints3D() const {
    assert(is3D());

    if (points3D_.empty()) return std::pair<int,int>(0,0);

    return std::pair<int,int>(points3D_.begin()->second.size(), points3D_.size());
  }

  const CGnuPlotPoint &getPoint3D(int ix, int iy) const {
    assert(is3D());

    auto p = points3D_.find(iy);

    if (p != points3D_.end())
      return (*p).second[ix];
    else
      assert(false);
  }

  //---

  void clearPoints();

  //---

  void addPoint2D(double x, double y);
  void addPoint2D(double x, CExprValueP y);
  void addPoint2D(const Values &values, bool discontinuity=false);

  void addPoint3D(int iy, double x, double y, double z);
  void addPoint3D(int iy, double x, double y, CExprValueP z);
  void addPoint3D(int iy, const Values &values, bool discontinuity=false);

  //---

  void reset3D();

  void smooth();

  void calcXRange(double *xmin, double *xmax) const;
  void calcYRange(double *ymin, double *ymax) const;
  void calcZRange(double *zmin, double *zmax) const;

  void calcBoundedYRange  (double *ymin, double *ymax) const;
  void recalcBoundedYRange(double *ymin, double *ymax) const;

  void getGroupXRange(double *xmin, double *xmax) const;

  bool renderBBox(CBBox2D &bbox) const;

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

  //void setXMin(double x) { xmin_ = x; }
  //void setXMax(double x) { xmax_ = x; }
  //void setYMin(double y) { ymin_ = y; }
  //void setYMax(double y) { ymax_ = y; }

  //---

  const BarObjects    &barObjects   () const { return barCache_   .objects(); }
  const PieObjects    &pieObjects   () const { return pieCache_   .objects(); }
  const BubbleObjects &bubbleObjects() const { return bubbleCache_.objects(); }

  //---

  struct DrawHistogramData {
    double x2 { 0.5 };
    double y2 { 0.0 };
    double d  { 1.0 };
    double w  { 1.0 };
  };

  void initRenderer();

  virtual void draw();

  void draw3D();
  void draw2D();

  void drawSurface(CGnuPlotRenderer *renderer);

  void drawBoxErrorBars      (CGnuPlotRenderer *renderer);
  void drawBoxes             (CGnuPlotRenderer *renderer);
  void drawBoxPlot           (CGnuPlotRenderer *renderer);
  void drawBoxXYErrorBars    (CGnuPlotRenderer *renderer);
  void drawBubbleChart       (CGnuPlotRenderer *renderer);
  void drawCandleSticks      (CGnuPlotRenderer *renderer);
  void drawCircles           (CGnuPlotRenderer *renderer);
  void drawEllipses          (CGnuPlotRenderer *renderer);
  void drawDots              (CGnuPlotRenderer *renderer);
  void drawFilledCurves      (CGnuPlotRenderer *renderer);
  void drawFinanceBars       (CGnuPlotRenderer *renderer);
  void drawClusteredHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &data);
  void drawErrorBarsHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &data);
  void drawStackedHistogram  (CGnuPlotRenderer *renderer, int i, const CBBox2D &bbox);
  void drawSteps             (CGnuPlotRenderer *renderer);
  void drawBinaryImage       (CGnuPlotRenderer *renderer);
  void drawImage             (CGnuPlotRenderer *renderer);
  void drawImpulses          (CGnuPlotRenderer *renderer);
  void drawLabels            (CGnuPlotRenderer *renderer);
  void drawLines             (CGnuPlotRenderer *renderer);
  void drawParallelAxes      (CGnuPlotRenderer *renderer);
  void drawPieChart          (CGnuPlotRenderer *renderer);
  void drawPoints            (CGnuPlotRenderer *renderer);
  void drawVectors           (CGnuPlotRenderer *renderer);
  void drawErrorBars         (CGnuPlotRenderer *renderer);
  void drawXErrorBars        (CGnuPlotRenderer *renderer);
  void drawXYErrorBars       (CGnuPlotRenderer *renderer);
  void drawYErrorBars        (CGnuPlotRenderer *renderer);
  void drawXErrorLines       (CGnuPlotRenderer *renderer);
  void drawXYErrorLines      (CGnuPlotRenderer *renderer);
  void drawYErrorLines       (CGnuPlotRenderer *renderer);

  void setNumBars(int n);
  void drawBars  (CGnuPlotRenderer *renderer);

  double decodeImageUsingColor(int col, const CRGBA &c) const;
  double indexImageColor(int i, const CRGBA &c) const;

  void drawTerminal();
  void drawPalette();

  double getXSpacing() const;

  CGnuPlotBar    *createBar   () const;
  CGnuPlotPie    *createPie   () const;
  CGnuPlotBubble *createBubble() const;

  bool mapPoint3D(const CGnuPlotPoint &p, CPoint3D &p1) const;

 protected:
  typedef std::vector<CPoint2D>         Points;
  typedef std::pair<double,Points>      ZPoints;
  typedef std::vector<ZPoints>          ZPointsArray;
  typedef std::map<double,ZPointsArray> ZPolygons;

  static int nextId_;

  CGnuPlotGroup*     group_;                            // parent group
  int                id_;                               // unique id
  int                ind_       { 0 };                  // axis index
  bool               displayed_ { true };               // is displayed
  Points2D           points2D_;                         // 2D points
  Points3D           points3D_;                         // 3D points
  ImageData          imageData_;                        // image data
  bool               binary_ { false };
  bool               matrix_ { false };
  CGnuPlotImageStyle imageStyle_;
  Bars               bars_;
  CGnuPlotBoxWidth   boxWidth_;                         // box widths
  FilledCurve        filledCurve_;                      // filled curve data
  PlotStyle          style_ { PlotStyle::POINTS};       // plot style
  CGnuPlotFillStyle  fillStyle_;                        // fill style
  CGnuPlotLineStyle  lineStyle_;                        // line style
  CGnuPlotPointStyle pointStyle_;                       // point style
  CGnuPlotArrowStyle arrowStyle_;                       // arrow style
  std::string        keyTitle_;                         // title on key
  int                xind_ { 1 };                       // xaxis index
  int                yind_ { 1 };                       // yaxis index
  COptReal           xmin_, xmax_;                      // calculated points x range
  COptReal           ymin_, ymax_;                      // calculated points y range
  COptReal           zmin_, zmax_;                      // calculated points z range
  COptReal           bymin_, bymax_;                    // calculated points bounded y range
  CBBox2D            bbox_ { 0, 0, 1, 1 };              // bounding box
  Smooth             smooth_ { Smooth::NONE };          // smooth data
  CGnuPlotContour    contour_;                          // contour data
  bool               contourSet_ { false };
  ZPolygons          surface_;                          // surface data
  bool               surfaceSet_ { false };
  COptReal           surfaceZMin_, surfaceZMax_;
  Hidden3DData       hidden3D_;
  double             whiskerBars_ { 0 };                // whisker bar data
  BarCache           barCache_;
  PieCache           pieCache_;
  BubbleCache        bubbleCache_;
};

//------

inline CGnuPlotBar *
CGnuPlotCacheFactory<CGnuPlotBar>::
make()
{
  return plot_->createBar();
}

inline CGnuPlotPie *
CGnuPlotCacheFactory<CGnuPlotPie>::
make()
{
  return plot_->createPie();
}

inline CGnuPlotBubble *
CGnuPlotCacheFactory<CGnuPlotBubble>::
make()
{
  return plot_->createBubble();
}

#endif
