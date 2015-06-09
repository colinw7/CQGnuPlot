#ifndef CGnuPlotGroup_H
#define CGnuPlotGroup_H

#include <CGnuPlot.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotColorBox.h>
#include <CGnuPlotPalette.h>
#include <CGnuPlotClip.h>

class CGnuPlotGroup {
 public:
  typedef CGnuPlotTypes::HistogramStyle          HistogramStyle;
  typedef CGnuPlotTypes::DrawLayer               DrawLayer;
  typedef CGnuPlotTypes::PlotStyle               PlotStyle;
  typedef CGnuPlot::Annotations                  Annotations;
  typedef CGnuPlot::AxesData                     AxesData;
  typedef CGnuPlot::PlotSize                     PlotSize;
  typedef CGnuPlot::Margin                       Margin;
  typedef std::vector<CGnuPlotPlot *>            Plots;
  typedef std::map<std::string, CGnuPlotAxis *>  Axes;

 public:
  CGnuPlotGroup(CGnuPlotWindow *window);

  CGnuPlotWindow *window() const { return window_; }

  CGnuPlot *app() const;

  int id() const { return id_; }

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

  void set3D(bool b);
  bool is3D() const { return is3D_; }

  CGnuPlotTitle *title() const { return title_; }

  void cameraChanged();

  void setTitleData(const CGnuPlotTitle &t);

  const Plots &plots() const { return plots_; }
  void addSubPlots(const Plots &plots);

  //---

  void init();

  void addObjects();

  const Annotations &annotations() const { return annotations_; }
  void setAnnotations(const Annotations &annotations) { annotations_ = annotations; }

  void fit();

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  const CGnuPlotAxisData &xaxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->xaxis(ind);
  }
  const CGnuPlotAxisData &yaxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->yaxis(ind);
  }
  const CGnuPlotAxisData &zaxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->zaxis(ind);
  }
  const CGnuPlotAxisData &paxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->paxis(ind);
  }
  const CGnuPlotAxisData &taxis(int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->taxis(ind);
  }
  const CGnuPlotAxisData &raxis() const {
    return const_cast<CGnuPlotGroup *>(this)->raxis();
  }
  const CGnuPlotAxisData &uaxis() const {
    return const_cast<CGnuPlotGroup *>(this)->uaxis();
  }
  const CGnuPlotAxisData &vaxis() const {
    return const_cast<CGnuPlotGroup *>(this)->vaxis();
  }

  CGnuPlotAxisData &xaxis(int ind) {
    CGnuPlot::IAxisMap::iterator p = axesData_.xaxis.find(ind);

    if (p == axesData_.xaxis.end())
      p = axesData_.xaxis.insert(p, CGnuPlot::IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &yaxis(int ind) {
    CGnuPlot::IAxisMap::iterator p = axesData_.yaxis.find(ind);

    if (p == axesData_.yaxis.end())
      p = axesData_.yaxis.insert(p, CGnuPlot::IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &zaxis(int ind) {
    CGnuPlot::IAxisMap::iterator p = axesData_.zaxis.find(ind);

    if (p == axesData_.zaxis.end())
      p = axesData_.zaxis.insert(p, CGnuPlot::IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &paxis(int ind) { return this->axesData_.paxis[ind]; }
  CGnuPlotAxisData &taxis(int ind) { return this->axesData_.taxis[ind]; }

  CGnuPlotAxisData &raxis() { return this->axesData_.raxis ; }
  CGnuPlotAxisData &uaxis() { return this->axesData_.uaxis ; }
  CGnuPlotAxisData &vaxis() { return this->axesData_.vaxis ; }

  //---

  double getXMin() const { return xaxis(1).min().getValue(-10); }
  double getXMax() const { return xaxis(1).max().getValue( 10); }
  double getYMin() const { return yaxis(1).min().getValue(-10); }
  double getYMax() const { return yaxis(1).max().getValue( 10); }
  double getZMin() const { return zaxis(1).min().getValue(-10); }
  double getZMax() const { return zaxis(1).max().getValue( 10); }

  void setXMin(double x) { xaxis(1).setMin(x); }
  void setXMax(double x) { xaxis(1).setMax(x); }
  void setYMin(double y) { yaxis(1).setMin(y); }
  void setYMax(double y) { yaxis(1).setMax(y); }
  void setZMin(double z) { zaxis(1).setMin(z); }
  void setZMax(double z) { zaxis(1).setMax(z); }

  void setRange(const CBBox2D &b) {
    setXMin(b.getXMin()); setYMin(b.getYMin());
    setXMax(b.getXMax()); setYMax(b.getYMax());
  }

  void saveRange();
  void restoreRange();

  //---

  const std::string &getXLabel() const { return xaxis(1).text(); }
  void setXLabel(const std::string &s) { xaxis(1).setText(s); }

  const std::string &getYLabel() const { return yaxis(1).text(); }
  void setYLabel(const std::string &s) { yaxis(1).setText(s); }

  bool getXTics() const { return xaxis(1).showTics(); }
  void setXTics(bool b) { xaxis(1).setShowTics(b); }

  bool getXTicsMirror() const { return xaxis(1).isMirror(); }
  void setXTicsMirror(bool b) { xaxis(1).setMirror(b); }

  bool getYTics() const { return yaxis(1).showTics(); }
  void setYTics(bool b) { yaxis(1).setShowTics(b); }

  bool getYTicsMirror() const { return yaxis(1).isMirror(); }
  void setYTicsMirror(bool b) { yaxis(1).setMirror(b); }

  bool getXGrid() const { return xaxis(1).hasGrid(); }
  void setXGrid(bool b) { xaxis(1).setGrid(b); }

  bool getYGrid() const { return yaxis(1).hasGrid(); }
  void setYGrid(bool b) { yaxis(1).setGrid(b); }

  //---

  bool isPolar() const { return polar_; }
  void setPolar(bool b) { polar_ = b; }

  //---

  CGnuPlotCamera *camera() const { return camera_; }
  void setCamera(CGnuPlotCamera *c);

  void setCameraEnabled(bool b);

  void setCameraRotateX(double a);
  void setCameraRotateY(double a);
  void setCameraRotateZ(double a);

  void setCameraXMin(double x);
  void setCameraXMax(double x);
  void setCameraYMin(double y);
  void setCameraYMax(double y);
  void setCameraNear(double z);
  void setCameraFar (double z);

  //---

  int getBorderSides() const { return axesData_.border.sides; }
  void setBorderSides(int b) { axesData_.border.sides = b; }

  const DrawLayer &getBorderLayer() const { return axesData_.border.layer; }
  void setBorderLayer(const DrawLayer &l) { axesData_.border.layer = l; }

  int getBorderWidth() const { return axesData_.border.lineWidth; }
  void setBorderWidth(double w) { axesData_.border.lineWidth = w; }

  int getBorderStyle() const { return axesData_.border.lineStyle.getValue(-1); }
  void setBorderStyle(int ls) { axesData_.border.lineStyle = ls; }

  int getBorderType() const { return axesData_.border.lineType.getValue(-1); }
  void setBorderType(int lt) { axesData_.border.lineType = lt; }

  //---

  void normalizeXRange(double &xmin, double &xmax, double xi=0) const;
  void normalizeYRange(double &ymin, double &ymax, double yi=0) const;
  void normalizeZRange(double &zmin, double &zmax, double zi=0) const;

  CBBox2D getClip(int xind=1, int yind=1) const;

  //---

  const COptBBox2D &clearRect() const { return clearRect_; }
  void setClearRect(const COptBBox2D &r) { clearRect_ = r; }

  //---

  const CGnuPlotKeyP &key() const { return key_; }

  const CGnuPlotColorBoxP &colorBox() const { return colorBox_; }

  const CGnuPlotPaletteP &palette() const { return palette_; }

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

  double marginLeft  () const { return margin_.left  ().getValue(10); }
  double marginRight () const { return margin_.right ().getValue(10); }
  double marginTop   () const { return margin_.top   ().getValue(10); }
  double marginBottom() const { return margin_.bottom().getValue(10); }

  void setMarginLeft  (double v) { margin_.setLeft  (v); }
  void setMarginRight (double v) { margin_.setRight (v); }
  void setMarginTop   (double v) { margin_.setTop   (v); }
  void setMarginBottom(double v) { margin_.setBottom(v); }

  const Margin &margin() const { return margin_; }
  void setMargin(const Margin &m) { margin_ = m; }

  //-----

  const CGnuPlotClip &clip() const { return clip_; }
  void setClip(const CGnuPlotClip &clip) { clip_ = clip; }

  //-----

  const PlotSize &plotSize() const { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

  //-----

  const CGnuPlotHistogramData &getHistogramData() const { return histogramData_; }
  void setHistogramData(const CGnuPlotHistogramData &data);

  //-----

  const Axes &axes() const { return axes_; }

  //-----

  void showXAxis(bool show);
  void showYAxis(bool show);

  void setAxisRange(const std::string &id, double r1, double r2);

  void setAxisStart(const std::string &id, double r);
  void setAxisEnd  (const std::string &id, double r);

  void setAxisGridDisplayed(const std::string &id, bool b);

  void updatePlotAxisRange(const std::string &id);
  void updatePlotAxisRange(char c, int ind);

  //-----

  void reset3D();

  virtual void draw();

  void drawClearRect(CGnuPlotRenderer *renderer);

  void drawTitle();

  void drawHistogram(const Plots &plots);

  void drawRowStackedHistograms   (CGnuPlotRenderer *renderer, const Plots &plots);
  void drawColumnStackedHistograms(CGnuPlotRenderer *renderer, const Plots &plots);

  void drawAxes(CGnuPlotRenderer *renderer);

  void drawBorder(CGnuPlotRenderer *renderer);

  void drawXAxis(CGnuPlotRenderer *renderer, int xind, bool other);
  void drawYAxis(CGnuPlotRenderer *renderer, int yind, bool other);
  void drawZAxis(CGnuPlotRenderer *renderer, int yind, bool other);

  void drawAxis(CGnuPlotRenderer *renderer, const CGnuPlotAxisData &axis, char c, int ind);

  void drawGrid(CGnuPlotRenderer *renderer, const CGnuPlot::DrawLayer &layer);

  void drawKey();
  void drawColorBox(CGnuPlotRenderer *renderer);

  void drawAnnotations(DrawLayer layer);

  CGnuPlotPlot *getSingleStylePlot() const;

  CGnuPlotAxis *getPlotAxis(char c, int ind);

  //-----

  const CGnuPlotAxisData *getAxisDataFromId(const std::string &id) const;
  CGnuPlotAxisData       *getAxisDataFromId(const std::string &id);

  bool hasTicLabels(const std::string &id) const;

  const CGnuPlotAxisData::RTicLabels &ticLabels(const std::string &id) const;

  std::string getAxisValueStr(const std::string &id, int i, double r) const;
  std::string getAxisValueStr(const CGnuPlotAxisData &axis, int i, double r) const;
  std::string formatAxisValue(const CGnuPlotAxisData &axis, double r) const;


  //-----

  void calcRange(int xind, int yind, double &xmin, double &ymin, double &xmax, double &ymax) const;

  CBBox2D getDisplayRange(int xind, int yind) const;

  //-----

  bool hasPlotStyle(PlotStyle plotStyle) const;
  bool hasImageStyle() const;

  void getPlotsOfStyle(Plots &plots, PlotStyle plotStyle) const;

  //-----

  CBBox2D getRegionBBox() const;

  const CBBox2D &getBBox() const { return bbox_; }

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &r) { backgroundColor_ = r; }

  //-----

  CPoint3D mapLogPoint(const CPoint3D &p) const;
  CPoint2D mapLogPoint(const CPoint2D &p) const;

  void mapLogPoint(CPoint3D &p) const;
  void mapLogPoint(CPoint2D &p) const;
  void mapLogPoint(double *x, double *y, double *z) const;

  void unmapLogPoint(CPoint3D &p) const;
  void unmapLogPoint(double *x, double *y, double *z) const;

  //-----

  //void windowToPixel(double wx, double wy, double *px, double *py) const;
  //void pixelToWindow(double px, double py, double *wx, double *wy) const;

  //CPoint2D windowToPixel(const CPoint2D &p) const;
  //CPoint2D pixelToWindow(const CPoint2D &p) const;

  //double pixelWidthToWindowWidth  (double w) const;
  //double pixelHeightToWindowHeight(double h) const;

 protected:
  static int nextId_;

  CGnuPlotWindow*       window_;               // parent window
  int                   id_   { 0 };           // unique id
  int                   ind_  { 0 };           // group index in window
  bool                  is3D_ { false };       // plots are 3D
  CGnuPlotTitle*        title_;                // plot title
  Plots                 plots_;                // plots
  CBBox2D               region_ {0,0,1,1};     // region of window
  Margin                margin_ {10,10,10,10}; // margin around plots
  CBBox2D               bbox_ { 0, 0, 1, 1 };  // bounding box
  CGnuPlotClip          clip_;                 // clip
  COptBBox2D            clearRect_;            // optional clear rectangle
  PlotSize              plotSize_;
  CGnuPlotHistogramData histogramData_;        // histogram style
  CGnuPlotKeyP          key_;                  // key
  CGnuPlotColorBoxP     colorBox_;             // color box
  CGnuPlotPaletteP      palette_;              // palette
  AxesData              axesData_;             // axes data
  Annotations           annotations_;          // annotations
  Axes                  axes_;                 // axes
  bool                  polar_ { false };      // polar
  CGnuPlotCamera*       camera_;               // view camera
  CRGBA                 backgroundColor_;      // background color
  mutable CBBox2D       saveRange_;
};

#endif
