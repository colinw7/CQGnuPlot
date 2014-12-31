#ifndef CGnuPlotPlot_H
#define CGnuPlotPlot_H

#include <CGnuPlot.h>
#include <CGnuPlotContour.h>

#include <CExpr.h>
#include <CRefPtr.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <vector>
#include <map>

class CGnuPlotWindow;
class CGnuPlotRenderer;

typedef CRefPtr<CExprValue> CExprValuePtr;

class CGnuPlotPlot {
 public:
  typedef std::vector<CGnuPlot::Arrow>     Arrows;
  typedef std::vector<CGnuPlot::Label>     Labels;
  typedef std::vector<CGnuPlot::Rectangle> Rectangles;
  typedef std::vector<CGnuPlot::Ellipse>   Ellipses;
  typedef std::vector<CGnuPlot::Polygon>   Polygons;
  typedef std::vector<CExprValuePtr>       Values;
  typedef std::vector<CGnuPlotPlot *>      Plots;
  typedef CGnuPlot::AxesData               AxesData;
  typedef CGnuPlot::AxisData               AxisData;
  typedef CGnuPlot::KeyData                KeyData;
  typedef CGnuPlot::PlotSize               PlotSize;
  typedef CGnuPlot::FillType               FillType;
  typedef CGnuPlot::FillPattern            FillPattern;
  typedef CGnuPlot::SymbolType             SymbolType;

  struct Point {
    Values values;
    bool   discontinuity;

    Point();
    Point(const Values &values, bool discontinuity);

   ~Point();

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
      p.print(os); return os;
    }

    bool getX(double &x) const;
    bool getY(double &y) const;
    bool getZ(double &z) const;

    double getX() const;
    double getY() const;
    double getZ() const;

    bool getValue(int n, double &r) const;
    bool getValue(int n, std::string &str) const;

    int cmp(const Point &p) const;

    void print(std::ostream &os) const;

    bool operator< (const Point &p) const { return (cmp(p) < 0); }
    bool operator> (const Point &p) const { return (cmp(p) > 0); }
    bool operator==(const Point &p) const { return (cmp(p) == 0); }

    bool operator>=(const Point &p) const { return ! (*this < p); }
    bool operator<=(const Point &p) const { return ! (*this > p); }

    bool operator!=(const Point &p) const { return ! (*this == p); }
  };

  typedef std::vector<Point>     Points2D;
  typedef std::map<int,Points2D> Points3D;

 public:
  CGnuPlotPlot(CGnuPlotWindow *window);

  virtual ~CGnuPlotPlot();

  CGnuPlot *plot() const;

  CGnuPlotWindow *window() const { return window_; }

  int id() const { return id_; }

  void set3D(bool b) { is3D_ = b; }
  bool is3D() const { return is3D_; }

  CGnuPlotPlot *parentPlot() const { return parentPlot_; }

  void init();

  const Plots &subPlots() const { return subPlots_; }
  void addSubPlots(const Plots &plots);

  virtual void stateChanged(CGnuPlot::ChangeState) { }

  //---

  // region of window
  double getRegionLeft  () const { return region().getLeft  (); }
  double getRegionRight () const { return region().getRight (); }
  double getRegionTop   () const { return region().getTop   (); }
  double getRegionBottom() const { return region().getBottom(); }

  void setRegionLeft  (double v) { CBBox2D r = region(); r.setLeft  (v); setRegion(r); }
  void setRegionRight (double v) { CBBox2D r = region(); r.setRight (v); setRegion(r); }
  void setRegionTop   (double v) { CBBox2D r = region(); r.setTop   (v); setRegion(r); }
  void setRegionBottom(double v) { CBBox2D r = region(); r.setBottom(v); setRegion(r); }

  void setRegion(const CBBox2D &r);
  const CBBox2D &region() const;

  //---

  double marginLeft  () const { return margin().left  (); }
  double marginRight () const { return margin().right (); }
  double marginTop   () const { return margin().top   (); }
  double marginBottom() const { return margin().bottom(); }

  void setMarginLeft  (double v) { CRange2D m = margin(); m.setLeft  (v); setMargin(m); }
  void setMarginRight (double v) { CRange2D m = margin(); m.setRight (v); setMargin(m); }
  void setMarginTop   (double v) { CRange2D m = margin(); m.setTop   (v); setMargin(m); }
  void setMarginBottom(double v) { CRange2D m = margin(); m.setBottom(v); setMargin(m); }

  const CRange2D &margin() const;
  void setMargin(const CRange2D &m);

  //---

  const PlotSize &plotSize() const;
  void setPlotSize(const PlotSize &s);

  double getRatio() const { return plotSize().ratio.getValue(1.0); }
  void setRatio(double r) { PlotSize s = plotSize(); s.ratio = r; setPlotSize(s); }

  //---

  void setClearRect(const CBBox2D &r) { clearRect_ = r; }

  const Points2D &getPoints2D() const { assert(! is3D_); return points2D_; }

  uint numPoints2D() const { assert(! is3D_); return points2D_.size(); }

  const Point &getPoint2D(int i) const { assert(! is3D_); return points2D_[i]; }

  std::pair<int,int> numPoints3D() const {
    assert(is3D_);

    if (points3D_.empty()) return std::pair<int,int>(0,0);

    return std::pair<int,int>(points3D_.begin()->second.size(), points3D_.size());
  }

  const Point &getPoint3D(int ix, int iy) const {
    assert(is3D_);

    auto p = points3D_.find(iy);

    if (p != points3D_.end())
      return (*p).second[ix];
    else
      assert(false);
  }

  void clearPoints();

  void addPoint2D(double x, double y);
  void addPoint2D(const Values &value, bool discontinuity=false);

  void addPoint3D(int iy, double x, double y, double z);

  void showXAxis(bool show);
  void showYAxis(bool show);

  void reset3D();

  void fit();
  void smooth();

  CBBox2D getDisplayRange() const;

  void calcXRange(double *xmin, double *xmax) const;
  void calcYRange(double *ymin, double *ymax) const;

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

  CGnuPlot::PlotStyle getStyle() const { return style_; }
  void setStyle(CGnuPlot::PlotStyle style) { style_ = style; }

  void setSmooth(CGnuPlot::Smooth s) { smooth_ = s; }
  CGnuPlot::Smooth getSmooth() const { return smooth_; }

  const CGnuPlot::FillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlot::FillStyle &f) { fillStyle_ = f; }

  const FillType &fillType() const { return fillStyle_.style(); }
  void setFillType(const FillType &f) { fillStyle_.setStyle(f); }

  const FillPattern &fillPattern() const { return fillStyle_.pattern(); }
  void setFillPattern(const FillPattern &f) { fillStyle_.setPattern(f); }

  const CGnuPlot::LineStyle &lineStyle() const { return lineStyle_; }
  void setLineStyle(const CGnuPlot::LineStyle &s) { lineStyle_ = s; }

  const COptInt &lineStyleNum() const { return lineStyleNum_; }
  void setLineStyleNum(const COptInt &style) { lineStyleNum_ = style; }
  void resetLineStyleNum() { lineStyleNum_.setInvalid(); }

  CGnuPlot::LineStyle calcLineStyle() const;

  CRGBA lineColor() const { return lineStyle_.color(CRGBA(0,0,0)); }
  void setLineColor(const CRGBA &c) { lineStyle_.setColor(c); }

  double lineWidth() const { return lineStyle_.width(); }
  void setLineWidth(double w) { lineStyle_.setWidth(w); }

  const CGnuPlot::PointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const CGnuPlot::PointStyle &s) { pointStyle_ = s; }

  SymbolType pointType() const { return static_cast<SymbolType>(pointStyle_.type()); }
  void setPointType(SymbolType type) { pointStyle_.setType(type); }

  double pointSize() const { return pointStyle_.size(); }
  void setPointSize(double s) { pointStyle_.setSize(s); }

  CGnuPlot::HistogramStyle getHistogramStyle() const { return histogramStyle_; }
  void setHistogramStyle(CGnuPlot::HistogramStyle style) { histogramStyle_ = style; }

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

  const AxesData &axesData() const;
  void setAxesData(const AxesData &a);

  void getXRange(double *xmin, double *xmax) const { *xmin = getXMin(); *xmax = getXMax(); }
  void getYRange(double *ymin, double *ymax) const { *ymin = getYMin(); *ymax = getYMax(); }
  void getZRange(double *zmin, double *zmax) const { *zmin = getZMin(); *zmax = getZMax(); }

  const std::string &getXLabel() const { return axesData().xaxis.str; }
  void setXLabel(const std::string &s) { AxesData a = axesData(); a.xaxis.str = s; setAxesData(a); }

  const std::string &getYLabel() const { return axesData().yaxis.str; }
  void setYLabel(const std::string &s) { AxesData a = axesData(); a.yaxis.str = s; setAxesData(a); }

  bool getXTics() const { return axesData().xaxis.showTics; }
  void setXTics(bool b) { AxesData a = axesData(); a.xaxis.showTics = b; setAxesData(a); }

  bool getXTicsMirror() const { return axesData().xaxis.mirror; }
  void setXTicsMirror(bool b) { AxesData a = axesData(); a.xaxis.mirror = b; setAxesData(a); }

  bool getYTics() const { return axesData().yaxis.showTics; }
  void setYTics(bool b) { AxesData a = axesData(); a.yaxis.showTics = b; setAxesData(a); }

  bool getYTicsMirror() const { return axesData().yaxis.mirror; }
  void setYTicsMirror(bool b) { AxesData a = axesData(); a.yaxis.mirror = b; setAxesData(a); }

  bool getXGrid() const { return axesData().xaxis.grid; }
  void setXGrid(bool b) { AxesData a = axesData(); a.xaxis.grid = b; setAxesData(a); }

  bool getYGrid() const { return axesData().yaxis.grid; }
  void setYGrid(bool b) { AxesData a = axesData(); a.yaxis.grid = b; setAxesData(a); }

  int getBorders() const { return axesData().borders; }
  void setBorders(int b) { AxesData a = axesData(); a.borders = b; setAxesData(a); }

  int getBorderWidth() const { return axesData().borderWidth; }
  void setBorderWidth(double w) { AxesData a = axesData(); a.borderWidth = w; setAxesData(a); }

  //---

  const KeyData &keyData() const;
  void setKeyData(const KeyData &k);

  const KeyData &rootKeyData() const;
  void setRootKeyData(const KeyData &k);

  bool getKeyDisplayed() const { return rootKeyData().displayed; }
  void setKeyDisplayed(bool b) { KeyData k = rootKeyData(); k.displayed = b; setRootKeyData(k); }

  bool getKeyReverse() const { return rootKeyData().reverse; }
  void setKeyReverse(bool b) { KeyData k = rootKeyData(); k.reverse = b; setRootKeyData(k); }

  bool getKeyBox() const { return rootKeyData().box; }
  void setKeyBox(bool b) { KeyData k = rootKeyData(); k.box = b; setRootKeyData(k); }

  CHAlignType getKeyHAlign() const { return rootKeyData().halign; }
  void setKeyHAlign(CHAlignType a) { KeyData k = rootKeyData(); k.halign = a; setRootKeyData(k); }

  CVAlignType getKeyVAlign() const { return rootKeyData().valign; }
  void setKeyVAlign(CVAlignType a) { KeyData k = rootKeyData(); k.valign = a; setRootKeyData(k); }

  //---

  void setFitX(bool b) { fitX_ = b; }
  bool getFitX() const { return fitX_; }

  void setFitY(bool b) { fitY_ = b; }
  bool getFitY() const { return fitY_; }

  void setFitZ(bool b) { fitZ_ = b; }
  bool getFitZ() const { return fitZ_; }

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

  void setRenderer(CGnuPlotRenderer *renderer);
  CGnuPlotRenderer *renderer() const { return renderer_; }

  const CGnuPlot::Palette &palette() const { return palette_; }
  void setPalette(const CGnuPlot::Palette &p) { palette_ = p; }

  int trianglePattern3D() const { return trianglePattern3D_; }
  void setTrianglePattern3D(int n) { trianglePattern3D_ = n; }

  virtual void draw();

  void draw2D();
  void draw3D();

  virtual void drawBar(double x, double y, const CBBox2D &bbox, FillType fillType,
                       FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor);

  void drawHistogram(const Plots &plots);

  void drawTerminal();
  void drawPalette();

  void drawAxes();
  void drawBorder();
  void drawXAxes(int xind, bool other);
  void drawYAxes(int yind, bool other);

  void drawKey();

  void drawAnnotations();

  void drawArrow(const CGnuPlot::Arrow &arrow);
  void drawLabel(const CGnuPlot::Label &label);
  void drawRectangle(const CGnuPlot::Rectangle &rect);
  void drawEllipse(const CGnuPlot::Ellipse &ellipse);
  void drawPolygon(const CGnuPlot::Polygon &poly);

  void drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c=CRGBA(0,0,0));
  void drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c=CRGBA(0,0,0));

  void drawRotatedText(const CPoint2D &p, const std::string &text,
                       const CRGBA &c=CRGBA(0,0,0), double a=90);

  void drawLine(const CPoint2D &p1, const CPoint2D &p2, double w, const CRGBA &c);

  std::string getXAxisValueStr(int i, double x) const;
  std::string getYAxisValueStr(int i, double x) const;

  std::string formatX(double x) const;
  std::string formatY(double y) const;

  void mapLogPoint  (double *x, double *y) const;
  void unmapLogPoint(double *x, double *y) const;

  void setLogScaleMap(const CGnuPlot::LogScaleMap &logScale) { logScale_ = logScale; }

  int getLogScale(CGnuPlot::LogScale scale) const {
    auto p = logScale_.find(scale);

    return (p != logScale_.end() ? (*p).second : 0);
  }

  void windowToPixel(double wx, double wy, double *px, double *py) const;
  void pixelToWindow(double px, double py, double *wx, double *wy) const;

  CPoint2D windowToPixel(const CPoint2D &p) const;
  CPoint2D pixelToWindow(const CPoint2D &p) const;

  double pixelWidthToWindowWidth  (double w) const;
  double pixelHeightToWindowHeight(double h) const;

 private:
  typedef std::vector<CPoint2D>         Points;
  typedef std::pair<double,Points>      ZPoints;
  typedef std::vector<ZPoints>          ZPointsArray;
  typedef std::map<double,ZPointsArray> ZPolygons;

  static int nextId_;

  CGnuPlotWindow*          window_;
  int                      id_;
  bool                     is3D_;
  CGnuPlotPlot*            parentPlot_;
  CBBox2D                  region_;
  CRange2D                 margin_;
  PlotSize                 plotSize_;
  COptValT<CBBox2D>        clearRect_;
  Plots                    subPlots_;
  Points2D                 points2D_;
  Points3D                 points3D_;
  int                      ind_;
  Arrows                   arrows_;
  Labels                   labels_;
  Rectangles               rects_;
  Ellipses                 ellipses_;
  Polygons                 polygons_;
  CGnuPlot::Palette        palette_;
  CGnuPlot::PlotStyle      style_;
  CGnuPlot::FillStyle      fillStyle_;
  CGnuPlot::LineStyle      lineStyle_;
  CGnuPlot::PointStyle     pointStyle_;
  COptInt                  lineStyleNum_;
  AxesData                 axesData_;
  KeyData                  keyData_;
  int                      xind_ { 1 };
  int                      yind_ { 1 };
  CGnuPlot::LogScaleMap    logScale_;
  CGnuPlot::Smooth         smooth_;
  CGnuPlot::HistogramStyle histogramStyle_;
  bool                     fitX_, fitY_, fitZ_;
  CGnuPlotContour          contour_;
  bool                     contourSet_;
  ZPolygons                surface_;
  bool                     surfaceSet_;
  COptReal                 surfaceZMin_, surfaceZMax_;
  int                      trianglePattern3D_;
  CGnuPlotRenderer*        renderer_;
  CBBox2D                  clip_;
};

#endif
