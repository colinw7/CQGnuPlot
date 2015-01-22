#ifndef CGnuPlotGroup_H
#define CGnuPlotGroup_H

#include <CGnuPlot.h>
#include <CGnuPlotAxis.h>

class CGnuPlotGroup {
 public:
  typedef CGnuPlot::HistogramStyle        HistogramStyle;
  typedef std::vector<CGnuPlotPlot *>     Plots;
  typedef std::vector<CGnuPlotArrowP>     Arrows;
  typedef std::vector<CGnuPlotLabelP>     Labels;
  typedef std::vector<CGnuPlotEllipseP>   Ellipses;
  typedef std::vector<CGnuPlotPolygonP>   Polygons;
  typedef std::vector<CGnuPlotRectangleP> Rectangles;
  typedef CGnuPlot::AxesData              AxesData;
  typedef CGnuPlot::AxisData              AxisData;
  typedef CGnuPlot::PlotSize              PlotSize;
  typedef CGnuPlot::PlotStyle             PlotStyle;
  typedef CGnuPlot::LogScaleMap           LogScaleMap;
  typedef CGnuPlot::LogScale              LogScale;

 public:
  CGnuPlotGroup(CGnuPlotWindow *window);

  CGnuPlotWindow *window() const { return window_; }

  CGnuPlot *app() const;

  int id() const { return id_; }

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

  const CGnuPlot::Title &title() const { return title_; }
  void setTitle(const CGnuPlot::Title &t) { title_ = t; }

  const Plots &plots() const { return plots_; }
  void addSubPlots(const Plots &plots);

  //---

  void init();

  void addObjects();

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

  void fit();

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  double getXMin() const { return axesData().xaxis.min.getValue(-10); }
  double getXMax() const { return axesData().xaxis.max.getValue( 10); }
  double getYMin() const { return axesData().yaxis.min.getValue(-1); }
  double getYMax() const { return axesData().yaxis.max.getValue( 1); }
  double getZMin() const { return axesData().zaxis.min.getValue(-1); }
  double getZMax() const { return axesData().zaxis.max.getValue( 1); }

  void setXMin(double x) { axesData_.xaxis.min.setValue(x); }
  void setXMax(double x) { axesData_.xaxis.max.setValue(x); }
  void setYMin(double y) { axesData_.yaxis.min.setValue(y); }
  void setYMax(double y) { axesData_.yaxis.max.setValue(y); }
  void setZMin(double z) { axesData_.zaxis.min.setValue(z); }
  void setZMax(double z) { axesData_.zaxis.max.setValue(z); }

  const std::string &getXLabel() const { return axesData().xaxis.str; }
  void setXLabel(const std::string &s) { axesData_.xaxis.str = s; }

  const std::string &getYLabel() const { return axesData().yaxis.str; }
  void setYLabel(const std::string &s) { axesData_.yaxis.str = s; }

  bool getXTics() const { return axesData().xaxis.showTics; }
  void setXTics(bool b) { axesData_.xaxis.showTics = b; }

  bool getXTicsMirror() const { return axesData().xaxis.mirror; }
  void setXTicsMirror(bool b) { axesData_.xaxis.mirror = b; }

  bool getYTics() const { return axesData().yaxis.showTics; }
  void setYTics(bool b) { axesData_.yaxis.showTics = b; }

  bool getYTicsMirror() const { return axesData().yaxis.mirror; }
  void setYTicsMirror(bool b) { axesData_.yaxis.mirror = b; }

  bool getXGrid() const { return axesData().xaxis.grid; }
  void setXGrid(bool b) { axesData_.xaxis.grid = b; }

  bool getYGrid() const { return axesData().yaxis.grid; }
  void setYGrid(bool b) { axesData_.yaxis.grid = b; }

  int getBorders() const { return axesData().borders; }
  void setBorders(int b) { axesData_.borders = b; }

  int getBorderWidth() const { return axesData().borderWidth; }
  void setBorderWidth(double w) { axesData_.borderWidth = w; }

  //---

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  bool getKeyDisplayed() const { return keyData().displayed(); }
  void setKeyDisplayed(bool b) { keyData_.setDisplayed(b); }

  bool getKeyReverse() const { return keyData().reverse(); }
  void setKeyReverse(bool b) { keyData_.setReverse(b); }

  bool getKeyBox() const { return keyData().box(); }
  void setKeyBox(bool b) { keyData_.setBox(b); }

  CHAlignType getKeyHAlign() const { return keyData().halign(); }
  void setKeyHAlign(CHAlignType a) { keyData_.setHAlign(a); }

  CVAlignType getKeyVAlign() const { return keyData().valign(); }
  void setKeyVAlign(CVAlignType a) { keyData_.setVAlign(a); }

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

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &r) { region_ = r; }

  //-----

  double marginLeft  () const { return margin().left  (); }
  double marginRight () const { return margin().right (); }
  double marginTop   () const { return margin().top   (); }
  double marginBottom() const { return margin().bottom(); }

  void setMarginLeft  (double v) { CRange2D m = margin(); m.setLeft  (v); setMargin(m); }
  void setMarginRight (double v) { CRange2D m = margin(); m.setRight (v); setMargin(m); }
  void setMarginTop   (double v) { CRange2D m = margin(); m.setTop   (v); setMargin(m); }
  void setMarginBottom(double v) { CRange2D m = margin(); m.setBottom(v); setMargin(m); }

  const CRange2D &margin() const { return margin_; }
  void setMargin(const CRange2D &m) { margin_ = m; }

  //-----

  const PlotSize &plotSize() const { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

  double getRatio() const { return plotSize().ratio.getValue(1.0); }
  void setRatio(double r) { PlotSize s = plotSize(); s.ratio = r; setPlotSize(s); }

  //-----

  HistogramStyle getHistogramStyle() const { return histogramStyle_; }
  void setHistogramStyle(HistogramStyle style) { histogramStyle_ = style; }

  //-----

  void setLogScaleMap(const LogScaleMap &logScale) { logScale_ = logScale; }

  int getLogScale(LogScale scale) const {
    auto p = logScale_.find(scale);

    return (p != logScale_.end() ? (*p).second : 0);
  }

  //-----

  void showXAxis(bool show);
  void showYAxis(bool show);

  //-----

  void reset3D();

  void draw();

  void initPlotAxis();

  void drawTitle();

  void drawHistogram(const Plots &plots);

  void drawAxes();
  void drawBorder();
  void drawXAxes(int xind, bool other);
  void drawYAxes(int yind, bool other);

  void drawKey();
  void drawAnnotations();

  //-----

  std::string getXAxisValueStr(int ind, int i, double x) const;
  std::string getYAxisValueStr(int ind, int i, double x) const;

  std::string formatX(double x) const;
  std::string formatY(double y) const;

  //-----

  void calcRange(int xind, int yind, double &xmin, double &ymin, double &xmax, double &ymax) const;

  CBBox2D getDisplayRange(int xind, int yind) const;

  //-----

  bool hasPlotStyle(PlotStyle plotStyle) const;

  void getPlotsOfStyle(Plots &plots, PlotStyle plotStyle) const;

  //-----

  CBBox2D getRegionBBox() const;

  //-----

  void mapLogPoint  (CPoint2D &p) const;
  void mapLogPoint  (double *x, double *y) const;
  void unmapLogPoint(double *x, double *y) const;

  //-----

  void windowToPixel(double wx, double wy, double *px, double *py) const;
  void pixelToWindow(double px, double py, double *wx, double *wy) const;

  CPoint2D windowToPixel(const CPoint2D &p) const;
  CPoint2D pixelToWindow(const CPoint2D &p) const;

  double pixelWidthToWindowWidth  (double w) const;
  double pixelHeightToWindowHeight(double h) const;

 private:
  static int nextId_;

  CGnuPlotWindow* window_;                                 // parent window
  int             id_;                                     // unique id
  int             ind_            {0};                     // group index in window
  CGnuPlot::Title title_;
  Plots           plots_;                                  // plots
  CBBox2D         region_         {0,0,1,1};               // region of window
  CRange2D        margin_         {10,10,10,10};           // margin around plots
  PlotSize        plotSize_;
  HistogramStyle  histogramStyle_ { HistogramStyle::NONE}; // histogram style
  AxesData        axesData_;                               // axes data
  LogScaleMap     logScale_;                               // log axis data
  CGnuPlotKeyData keyData_;                                // key data
  Arrows          arrows_;                                 // arrow annotations
  Labels          labels_;                                 // label annotations
  Rectangles      rects_;                                  // rectangle annotations
  Ellipses        ellipses_;                               // ellipse annotations
  Polygons        polygons_;                               // ploygon annotations
  CGnuPlotAxis    plotXAxis1_, plotXAxis2_;
  CGnuPlotAxis    plotYAxis1_, plotYAxis2_;
};

#endif
