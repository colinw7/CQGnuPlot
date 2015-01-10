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
  typedef CGnuPlot::KeyData               KeyData;
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

  const AxesData &axesData() const;
  void setAxesData(const AxesData &a);

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

  const KeyData &keyData() const { return keyData_; }
  void setKeyData(const KeyData &k) { keyData_ = k; }

  bool getKeyDisplayed() const { return keyData().displayed; }
  void setKeyDisplayed(bool b) { KeyData k = keyData(); k.displayed = b; setKeyData(k); }

  bool getKeyReverse() const { return keyData().reverse; }
  void setKeyReverse(bool b) { KeyData k = keyData(); k.reverse = b; setKeyData(k); }

  bool getKeyBox() const { return keyData().box; }
  void setKeyBox(bool b) { KeyData k = keyData(); k.box = b; setKeyData(k); }

  CHAlignType getKeyHAlign() const { return keyData().halign; }
  void setKeyHAlign(CHAlignType a) { KeyData k = keyData(); k.halign = a; setKeyData(k); }

  CVAlignType getKeyVAlign() const { return keyData().valign; }
  void setKeyVAlign(CVAlignType a) { KeyData k = keyData(); k.valign = a; setKeyData(k); }

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

  const CBBox2D &region() const;
  void setRegion(const CBBox2D &r);

  //-----

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

  //-----

  const PlotSize &plotSize() const;
  void setPlotSize(const PlotSize &s);

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

  bool hasHistograms() const;
  void getHistograms(Plots &plots) const;

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
  Plots           plots_;                                  // plots
  CBBox2D         region_         {0,0,1,1};               // region of window
  CRange2D        margin_         {10,10,10,10};           // margin around plots
  PlotSize        plotSize_;
  HistogramStyle  histogramStyle_ { HistogramStyle::NONE}; // histogram style
  AxesData        axesData_;                               // axes data
  LogScaleMap     logScale_;                               // log axis data
  KeyData         keyData_;                                // key data
  Arrows          arrows_;                                 // arrow annotations
  Labels          labels_;                                 // label annotations
  Rectangles      rects_;                                  // rectangle annotations
  Ellipses        ellipses_;                               // ellipse annotations
  Polygons        polygons_;                               // ploygon annotations
  CGnuPlotAxis    plotXAxis1_, plotXAxis2_;
  CGnuPlotAxis    plotYAxis1_, plotYAxis2_;
};

#endif
