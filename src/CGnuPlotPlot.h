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

class CGnuPlotWindow;
class CGnuPlotGroup;

typedef CRefPtr<CExprValue> CExprValueP;

class CGnuPlotPlot {
 public:
  typedef std::vector<CExprValueP>   Values;
  typedef CGnuPlot::BoxWidth         BoxWidth;
  typedef CGnuPlot::BoxWidthType     BoxWidthType;
  typedef CGnuPlot::AxesData         AxesData;
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

  std::pair<int,int> numPoints3D() const {
    assert(is3D_);

    if (points3D_.empty()) return std::pair<int,int>(0,0);

    return std::pair<int,int>(points3D_.begin()->second.size(), points3D_.size());
  }

  const Points3D &getPoints3D() const { assert(! is3D_); return points3D_; }

  const CGnuPlotPoint &getPoint3D(int ix, int iy) const {
    assert(is3D_);

    auto p = points3D_.find(iy);

    if (p != points3D_.end())
      return (*p).second[ix];
    else
      assert(false);
  }

  void clearPoints();

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

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &f) { fillStyle_ = f; }

  const FillType &fillType() const { return fillStyle_.style(); }
  void setFillType(const FillType &f) { fillStyle_.setStyle(f); }

  const FillPattern &fillPattern() const { return fillStyle_.pattern(); }
  void setFillPattern(const FillPattern &f) { fillStyle_.setPattern(f); }

  CGnuPlotLineStyleP lineStyle() const;
  void setLineStyle(CGnuPlotLineStyleP ls) { lineStyle_ = ls; }

  CGnuPlotLineStyleP calcLineStyle() const;

  int lineStyleId() const { return lineStyle()->ind(); }
  void setLineStyleId(int ind);

  CRGBA lineColor() const { return lineStyle()->color(CRGBA(0,0,0)); }
  void setLineColor(const CRGBA &c) { lineStyle()->setColor(c); }

  double lineWidth() const { return lineStyle()->width(); }
  void setLineWidth(double w) { lineStyle()->setWidth(w); }

  SymbolType pointType() const { return lineStyle()->pointType(); }
  void setPointType(SymbolType type) { lineStyle()->setPointType(type); }

  double pointSize() const { return lineStyle()->pointSize(); }
  void setPointSize(double s) { lineStyle()->setPointSize(s); }

  const PointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const PointStyle &s) { pointStyle_ = s; }

  const std::string &keyTitle() const { return keyTitle_; }
  void setKeyTitle(const std::string &s) { keyTitle_ = s; }

  //---

  double getXMin() const { return axesData().xaxis.min.getValue(-10); }
  double getXMax() const { return axesData().xaxis.max.getValue( 10); }
  double getYMin() const { return axesData().yaxis.min.getValue(-1); }
  double getYMax() const { return axesData().yaxis.max.getValue( 1); }
  double getZMin() const { return axesData().zaxis.min.getValue(-1); }
  double getZMax() const { return axesData().zaxis.max.getValue( 1); }

  void setXMin(double x) { AxesData a = axesData(); a.xaxis.min.setValue(x); setAxesData(a); }
  void setXMax(double x) { AxesData a = axesData(); a.xaxis.max.setValue(x); setAxesData(a); }
  void setYMin(double y) { AxesData a = axesData(); a.yaxis.min.setValue(y); setAxesData(a); }
  void setYMax(double y) { AxesData a = axesData(); a.yaxis.max.setValue(y); setAxesData(a); }
  void setZMin(double z) { AxesData a = axesData(); a.zaxis.min.setValue(z); setAxesData(a); }
  void setZMax(double z) { AxesData a = axesData(); a.zaxis.max.setValue(z); setAxesData(a); }

  void setRange(const CBBox2D &b) {
    setXMin(b.getXMin()); setYMin(b.getYMin());
    setXMax(b.getXMax()); setYMax(b.getYMax());
  }

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

  void setFitX(bool b) { fitX_ = b; }
  bool getFitX() const { return fitX_; }

  void setFitY(bool b) { fitY_ = b; }
  bool getFitY() const { return fitY_; }

  void setFitZ(bool b) { fitZ_ = b; }
  bool getFitZ() const { return fitZ_; }

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

  void drawLines();

  virtual void drawBar(double x, double y, const CBBox2D &bbox, FillType fillType,
                       FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor);

  void drawTerminal();
  void drawPalette();

  double getXSpacing() const;

  void drawLine(const CPoint2D &p1, const CPoint2D &p2, double w, const CRGBA &c);

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
  BoxWidth           boxWidth_;                         // box widths
  Palette            palette_;
  FilledCurve        filledCurve_;                      // filled curve data
  PlotStyle          style_       { PlotStyle::POINTS}; // plot style
  CGnuPlotFillStyle  fillStyle_;                        // fill style
  CGnuPlotLineStyleP lineStyle_;                        // line style
  PointStyle         pointStyle_;                       // point style
  AxesData           axesData_;
  std::string        keyTitle_;                         // title on key
  int                xind_ { 1 };                       // xaxis index
  int                yind_ { 1 };                       // yaxis index
  Smooth             smooth_      { Smooth::NONE };     // smooth data
  bool               fitX_ { false };                   // auto fit x
  bool               fitY_ { false };                   // auto fit y
  bool               fitZ_ { false };                   // auto fit z
  CGnuPlotContour    contour_;                          // contour data
  bool               contourSet_ { false };
  ZPolygons          surface_;                          // surface data
  bool               surfaceSet_ { false };
  COptReal           surfaceZMin_, surfaceZMax_;
  int                trianglePattern3D_ { 3 };          // 3d surface pattern
  double             whiskerBars_ { 0 };                // whisker bar data
  CBBox2D            clip_;
};

#endif
