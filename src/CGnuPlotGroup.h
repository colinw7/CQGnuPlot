#ifndef CGnuPlotGroup_H
#define CGnuPlotGroup_H

#include <CGnuPlot.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotKey.h>

class CGnuPlotGroup {
 public:
  typedef CGnuPlot::HistogramStyle               HistogramStyle;
  typedef std::vector<CGnuPlotPlot *>            Plots;
  typedef std::vector<CGnuPlotArrowP>            Arrows;
  typedef std::vector<CGnuPlotLabelP>            Labels;
  typedef std::vector<CGnuPlotEllipseP>          Ellipses;
  typedef std::vector<CGnuPlotPolygonP>          Polygons;
  typedef std::vector<CGnuPlotRectangleP>        Rectangles;
  typedef std::vector<CGnuPlotGroupAnnotation *> Annotations;
  typedef CGnuPlot::AxesData                     AxesData;
  typedef CGnuPlot::AxisData                     AxisData;
  typedef CGnuPlot::PlotSize                     PlotSize;
  typedef CGnuPlot::PlotStyle                    PlotStyle;
  typedef CGnuPlot::LogScaleMap                  LogScaleMap;
  typedef CGnuPlot::LogScale                     LogScale;
  typedef std::map<std::string, CGnuPlotAxis *>  Axes;

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

  void getAnnotations(Annotations &annotations) const;

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

  const AxisData &xaxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->axesData_.xaxis[ind];
  }
  const AxisData &yaxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->axesData_.yaxis[ind];
  }
  const AxisData &zaxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->axesData_.zaxis[ind];
  }
  const AxisData &paxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->axesData_.paxis[ind];
  }

  AxisData &xaxis(int ind) { return const_cast<CGnuPlotGroup *>(this)->axesData_.xaxis[ind]; }
  AxisData &yaxis(int ind) { return const_cast<CGnuPlotGroup *>(this)->axesData_.yaxis[ind]; }
  AxisData &zaxis(int ind) { return const_cast<CGnuPlotGroup *>(this)->axesData_.zaxis[ind]; }
  AxisData &paxis(int ind) { return const_cast<CGnuPlotGroup *>(this)->axesData_.paxis[ind]; }

  //---

  double getXMin() const { return xaxis(1).min.getValue(-10); }
  double getXMax() const { return xaxis(1).max.getValue( 10); }
  double getYMin() const { return yaxis(1).min.getValue(-1); }
  double getYMax() const { return yaxis(1).max.getValue( 1); }
  double getZMin() const { return zaxis(1).min.getValue(-1); }
  double getZMax() const { return zaxis(1).max.getValue( 1); }

  void setXMin(double x) { xaxis(1).min.setValue(x); }
  void setXMax(double x) { xaxis(1).max.setValue(x); }
  void setYMin(double y) { yaxis(1).min.setValue(y); }
  void setYMax(double y) { yaxis(1).max.setValue(y); }
  void setZMin(double z) { zaxis(1).min.setValue(z); }
  void setZMax(double z) { zaxis(1).max.setValue(z); }

  const std::string &getXLabel() const { return xaxis(1).str; }
  void setXLabel(const std::string &s) { xaxis(1).str = s; }

  const std::string &getYLabel() const { return yaxis(1).str; }
  void setYLabel(const std::string &s) { yaxis(1).str = s; }

  bool getXTics() const { return xaxis(1).showTics; }
  void setXTics(bool b) { xaxis(1).showTics = b; }

  bool getXTicsMirror() const { return xaxis(1).mirror; }
  void setXTicsMirror(bool b) { xaxis(1).mirror = b; }

  bool getYTics() const { return yaxis(1).showTics; }
  void setYTics(bool b) { yaxis(1).showTics = b; }

  bool getYTicsMirror() const { return yaxis(1).mirror; }
  void setYTicsMirror(bool b) { yaxis(1).mirror = b; }

  bool getXGrid() const { return xaxis(1).grid; }
  void setXGrid(bool b) { xaxis(1).grid = b; }

  bool getYGrid() const { return yaxis(1).grid; }
  void setYGrid(bool b) { yaxis(1).grid = b; }

  int getBorders() const { return axesData_.borders; }
  void setBorders(int b) { axesData_.borders = b; }

  int getBorderWidth() const { return axesData_.borderWidth; }
  void setBorderWidth(double w) { axesData_.borderWidth = w; }

  //---

  const CGnuPlotKeyP &key() const { return key_; }

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

  const Axes &axes() const { return axes_; }

  //-----

  void showXAxis(bool show);
  void showYAxis(bool show);

  //-----

  void reset3D();

  virtual void draw();

  void drawTitle();

  void drawHistogram(const Plots &plots);

  void drawAxes();
  void drawBorder();
  void drawXAxes(int xind, bool other);
  void drawYAxes(int yind, bool other);

  void drawKey();

  void drawAnnotations(CGnuPlotLayer layer);

  CGnuPlotAxis *getPlotAxis(char c, int ind);

  //-----

  std::string getAxisValueStr(const std::string &id, int i, double r) const;

  std::string formatAxisValue(const AxisData &axis, double r) const;

  //-----

  void calcRange(int xind, int yind, double &xmin, double &ymin, double &xmax, double &ymax) const;

  CBBox2D getDisplayRange(int xind, int yind) const;

  //-----

  bool hasPlotStyle(PlotStyle plotStyle) const;

  void getPlotsOfStyle(Plots &plots, PlotStyle plotStyle) const;

  //-----

  CBBox2D getRegionBBox() const;

  const CBBox2D &getBBox() const { return bbox_; }

  //-----

  void mapLogPoint  (CPoint2D &p) const;
  void mapLogPoint  (double *x, double *y) const;
  void unmapLogPoint(double *x, double *y) const;

  //-----

  //void windowToPixel(double wx, double wy, double *px, double *py) const;
  //void pixelToWindow(double px, double py, double *wx, double *wy) const;

  //CPoint2D windowToPixel(const CPoint2D &p) const;
  //CPoint2D pixelToWindow(const CPoint2D &p) const;

  //double pixelWidthToWindowWidth  (double w) const;
  //double pixelHeightToWindowHeight(double h) const;

 private:
  static int nextId_;

  CGnuPlotWindow* window_;                                 // parent window
  int             id_ { 0 };                               // unique id
  int             ind_  {0};                               // group index in window
  CGnuPlot::Title title_;
  Plots           plots_;                                  // plots
  CBBox2D         region_ {0,0,1,1};                       // region of window
  CRange2D        margin_ {10,10,10,10};                   // margin around plots
  CBBox2D         bbox_ { 0, 0, 1, 1 };                    // bounding box
  PlotSize        plotSize_;
  HistogramStyle  histogramStyle_ { HistogramStyle::NONE}; // histogram style
  CGnuPlotKeyP    key_;                                    // key
  AxesData        axesData_;                               // axes data
  LogScaleMap     logScale_;                               // log axis data
  Arrows          arrows_;                                 // arrow annotations
  Labels          labels_;                                 // label annotations
  Rectangles      rects_;                                  // rectangle annotations
  Ellipses        ellipses_;                               // ellipse annotations
  Polygons        polygons_;                               // polygon annotations
  Axes            axes_;                                   // axes
};

#endif
