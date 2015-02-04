#ifndef CGnuPlotPlot_H
#define CGnuPlotPlot_H

#include <CGnuPlot.h>
#include <CGnuPlotContour.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotPoint.h>

#include <CExpr.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <vector>
#include <map>

typedef unsigned char uchar;

class CGnuPlotWindow;
class CGnuPlotGroup;

typedef CRefPtr<CExprValue> CExprValueP;

class CGnuPlotPlot {
 public:
  typedef std::vector<CExprValueP>   Values;
  typedef CGnuPlot::BoxWidth         BoxWidth;
  typedef CGnuPlot::BoxWidthType     BoxWidthType;
  typedef CGnuPlot::AxesData         AxesData;
  typedef CGnuPlot::AxisData         AxisData;
  typedef CGnuPlot::PlotStyle        PlotStyle;
  typedef CGnuPlotTypes::FillType    FillType;
  typedef CGnuPlotTypes::FillPattern FillPattern;
  typedef CGnuPlot::Palette          Palette;
  typedef CGnuPlot::FilledCurve      FilledCurve;
  typedef CGnuPlot::PointStyle       PointStyle;
  typedef CGnuPlot::Smooth           Smooth;
  typedef CGnuPlotTypes::SymbolType  SymbolType;
  typedef std::vector<CGnuPlotPoint> Points2D;
  typedef std::map<int,Points2D>     Points3D;
  typedef std::vector<uchar>         ImageData;
  typedef CGnuPlot::ImageStyle       ImageStyle;

 public:
  CGnuPlotPlot(CGnuPlotGroup *group);

  virtual ~CGnuPlotPlot();

  CGnuPlot *app() const;

  CGnuPlotWindow *window() const;

  CGnuPlotGroup *group() const { return group_; }
  void setGroup(CGnuPlotGroup *g) { group_ = g; }

  int id() const { return id_; }

  void set3D(bool b) { is3D_ = b; }
  bool is3D() const { return is3D_; }

  void setDisplayed(bool b) { displayed_ = b; }
  bool isDisplayed() const { return displayed_; }

  void init();

  //---

  const CBBox2D &clearRect() const { return clearRect_.getValue(); }
  void setClearRect(const CBBox2D &r) { clearRect_ = r; }

  //---

  const Points2D &getPoints2D() const { assert(! is3D_); return points2D_; }

  uint numPoints2D() const { assert(! is3D_); return points2D_.size(); }

  const CGnuPlotPoint &getPoint2D(int i) const { assert(! is3D_); return points2D_[i]; }

  //---

  const Points3D &getPoints3D() const { assert(! is3D_); return points3D_; }

  std::pair<int,int> numPoints3D() const {
    assert(is3D_);

    if (points3D_.empty()) return std::pair<int,int>(0,0);

    return std::pair<int,int>(points3D_.begin()->second.size(), points3D_.size());
  }

  const CGnuPlotPoint &getPoint3D(int ix, int iy) const {
    assert(is3D_);

    auto p = points3D_.find(iy);

    if (p != points3D_.end())
      return (*p).second[ix];
    else
      assert(false);
  }

  //---

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool m) { matrix_ = m; }

  //---

  const ImageData &imageData() const { return imageData_; }

  void setImageData(const ImageData &imageData) { imageData_ = imageData; }

  const ImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const ImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  double imageAngle() const { return imageStyle_.a.getValue(0.0); }
  void setImageAngle(double a) { imageStyle_.a = a; }

  //---

  void clearPoints();

  //---

  void addPoint2D(double x, double y);
  void addPoint2D(double x, CExprValueP y);
  void addPoint2D(const Values &value, bool discontinuity=false);

  void addPoint3D(int iy, double x, double y, double z);

  //---

  void reset3D();

  void fit();
  void smooth();

  void calcXRange(double *xmin, double *xmax) const;
  void calcYRange(double *ymin, double *ymax) const;

  void calcBoxPlotRange();

  //---

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

  //---

  void setBoxWidth(const BoxWidth &w) { boxWidth_ = w; }
  const BoxWidth &getBoxWidth() const { return boxWidth_; }

  const BoxWidthType &getBoxWidthType() const { return boxWidth_.type; }
  void setBoxWidthType(const BoxWidthType &t) { boxWidth_.type = t; }

  double getBoxWidthValue() const { return boxWidth_.width; }
  void setBoxWidthValue(double s) { boxWidth_.width = s; }

  PlotStyle getStyle() const { return style_; }
  void setStyle(PlotStyle style) { style_ = style; }

  bool isImageStyle() const {
    return (style_ == PlotStyle::IMAGE || style_ == PlotStyle::RGBIMAGE ||
            style_ == PlotStyle::RGBALPHA);
  }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &f) { fillStyle_ = f; }

  const FillType &fillType() const { return fillStyle_.style(); }
  void setFillType(const FillType &f) { fillStyle_.setStyle(f); }

  const FillPattern &fillPattern() const { return fillStyle_.pattern(); }
  void setFillPattern(const FillPattern &f) { fillStyle_.setPattern(f); }

  const CGnuPlotLineStyle &lineStyle() const { return  lineStyle_; }
  void setLineStyle(const CGnuPlotLineStyle &ls) { lineStyle_ = ls; }

  int lineStyleId() const { return lineStyle().ind(); }
  void setLineStyleId(int ind);

  const CRGBA &lineColor(const CRGBA &c) const { return lineStyle().color(c); }
  void setLineColor(const CRGBA &c) { lineStyle_.setColor(c); }

  double lineWidth() const { return lineStyle().width(); }
  void setLineWidth(double w) { lineStyle_.setWidth(w); }

  SymbolType pointType() const { return lineStyle().pointType(); }
  void setPointType(SymbolType type) { lineStyle_.setPointType(type); }

  double pointSize() const { return lineStyle().pointSize(); }
  void setPointSize(double s) { lineStyle_.setPointSize(s); }

  const PointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const PointStyle &s) { pointStyle_ = s; }

  const CGnuPlotArrowStyle &arrowStyle() const { return arrowStyle_; }
  void setArrowStyle(const CGnuPlotArrowStyle &as) { arrowStyle_ = as; }

  const std::string &keyTitle() const { return keyTitle_; }
  void setKeyTitle(const std::string &s) { keyTitle_ = s; }

  //---

  const AxisData &xaxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->axesData_.xaxis[ind];
  }
  const AxisData &yaxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->axesData_.yaxis[ind];
  }
  const AxisData &zaxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->axesData_.zaxis[ind];
  }
  const AxisData &paxis(int ind) const {
    return const_cast<CGnuPlotPlot *>(this)->axesData_.paxis[ind];
  }

  AxisData &xaxis(int ind) { return const_cast<CGnuPlotPlot *>(this)->axesData_.xaxis[ind]; }
  AxisData &yaxis(int ind) { return const_cast<CGnuPlotPlot *>(this)->axesData_.yaxis[ind]; }
  AxisData &zaxis(int ind) { return const_cast<CGnuPlotPlot *>(this)->axesData_.zaxis[ind]; }
  AxisData &paxis(int ind) { return const_cast<CGnuPlotPlot *>(this)->axesData_.paxis[ind]; }

  //---

  double getXMin() const { return xaxis(1).min.getValue(-10); }
  double getXMax() const { return xaxis(1).max.getValue( 10); }
  double getYMin() const { return yaxis(1).min.getValue(-1); }
  double getYMax() const { return yaxis(1).max.getValue( 1); }
  double getZMin() const { return zaxis(1).min.getValue(-1); }
  double getZMax() const { return zaxis(1).max.getValue( 1); }

  void setXMin(double x) { xaxis(1).min.setValue(x); updateGroupRange(); }
  void setXMax(double x) { xaxis(1).max.setValue(x); updateGroupRange(); }
  void setYMin(double y) { yaxis(1).min.setValue(y); updateGroupRange(); }
  void setYMax(double y) { yaxis(1).max.setValue(y); updateGroupRange(); }
  void setZMin(double z) { zaxis(1).min.setValue(z); updateGroupRange(); }
  void setZMax(double z) { zaxis(1).max.setValue(z); updateGroupRange(); }

  void setRange(const CBBox2D &b) {
    setXMin(b.getXMin()); setYMin(b.getYMin());
    setXMax(b.getXMax()); setYMax(b.getYMax());
  }

  void updateGroupRange();

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  void getXRange(double *xmin, double *xmax) const { *xmin = getXMin(); *xmax = getXMax(); }
  void getYRange(double *ymin, double *ymax) const { *ymin = getYMin(); *ymax = getYMax(); }
  void getZRange(double *zmin, double *zmax) const { *zmin = getZMin(); *zmax = getZMax(); }

  //---

  int xind() const { return xind_; }
  int yind() const { return yind_; }

  //---

  const Palette &palette() const { return palette_; }
  void setPalette(const Palette &p) { palette_ = p; }

  const FilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const FilledCurve &c) { filledCurve_ = c; }

  int trianglePattern3D() const { return trianglePattern3D_; }
  void setTrianglePattern3D(int n) { trianglePattern3D_ = n; }

  double whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }

  //---

  virtual void draw();

  void draw2D();
  void draw3D();

  void drawBoxErrorBars  (const CBBox2D &bbox);
  void drawBoxes         (const CBBox2D &bbox);
  void drawBoxPlot       (const CBBox2D &bbox);
  void drawBoxXYErrorBars(const CBBox2D &bbox);
  void drawCandleSticks  (const CBBox2D &bbox);
  void drawCircles       (const CBBox2D &bbox);
  void drawEllipses      (const CBBox2D &bbox);
  void drawDots          (const CBBox2D &bbox);
  void drawFilledCurves  (const CBBox2D &bbox);
  void drawFinanceBars   (const CBBox2D &bbox);
  void drawSteps         (const CBBox2D &bbox);
  void drawBinaryImage   (const CBBox2D &bbox);
  void drawImage         (const CBBox2D &bbox);
  void drawImpulses      (const CBBox2D &bbox);
  void drawLabels        (const CBBox2D &bbox);
  void drawLines         ();
  void drawParallelAxes  (const CBBox2D &bbox);
  void drawPieChart      (const CBBox2D &bbox);
  void drawPoints        ();
  void drawVectors       (const CBBox2D &bbox);
  void drawXYErrorBars   (const CBBox2D &bbox);

  double decodeImageUsingColor(int col, const CRGBA &c) const;
  double indexImageColor(int i, const CRGBA &c) const;

  virtual void drawBar(double x, double y, const CBBox2D &bbox, FillType fillType,
                       FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor);

  void drawTerminal();
  void drawPalette();

  double getXSpacing() const;

  void drawPath(const std::vector<CPoint2D> &points, double w=1.0, const CRGBA &c=CRGBA(0,0,0),
                const CLineDash &dash=CLineDash());
  void drawLine(const CPoint2D &p1, const CPoint2D &p2, double w=1.0, const CRGBA &c=CRGBA(0,0,0),
                const CLineDash &dash=CLineDash());

  void fillPolygon(const std::vector<CPoint2D> &polygons, const CRGBA &c);

 protected:
  typedef std::vector<CPoint2D>         Points;
  typedef std::pair<double,Points>      ZPoints;
  typedef std::vector<ZPoints>          ZPointsArray;
  typedef std::map<double,ZPointsArray> ZPolygons;

  static int nextId_;

  CGnuPlotGroup*     group_;                            // parent group
  int                id_;                               // unique id
  int                ind_       { 0 };                  // index in group
  bool               displayed_ { true };               // is displayed
  bool               is3D_      { false };              // is 3D
  COptValT<CBBox2D>  clearRect_;
  Points2D           points2D_;                         // 2D points
  Points3D           points3D_;                         // 3D points
  ImageData          imageData_;                        // image data
  bool               binary_ { false };
  bool               matrix_ { false };
  ImageStyle         imageStyle_;
  BoxWidth           boxWidth_;                         // box widths
  Palette            palette_;
  FilledCurve        filledCurve_;                      // filled curve data
  PlotStyle          style_       { PlotStyle::POINTS}; // plot style
  CGnuPlotFillStyle  fillStyle_;                        // fill style
  CGnuPlotLineStyle  lineStyle_;                        // line style
  PointStyle         pointStyle_;                       // point style
  CGnuPlotArrowStyle arrowStyle_;                       // arrow style
  AxesData           axesData_;
  std::string        keyTitle_;                         // title on key
  int                xind_ { 1 };                       // xaxis index
  int                yind_ { 1 };                       // yaxis index
  Smooth             smooth_      { Smooth::NONE };     // smooth data
  CGnuPlotContour    contour_;                          // contour data
  bool               contourSet_ { false };
  ZPolygons          surface_;                          // surface data
  bool               surfaceSet_ { false };
  COptReal           surfaceZMin_, surfaceZMax_;
  int                trianglePattern3D_ { 3 };          // 3d surface pattern
  double             whiskerBars_ { 0 };                // whisker bar data
};

#endif
