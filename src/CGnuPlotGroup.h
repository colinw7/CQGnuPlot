#ifndef CGnuPlotGroup_H
#define CGnuPlotGroup_H

#include <CGnuPlot.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotColorBox.h>
#include <CGnuPlotPalette.h>
#include <CGnuPlotClip.h>
#include <CGnuPlotTimeStamp.h>
#include <CGnuPlotPm3D.h>
#include <CGnuPlotProbeEvent.h>
#include <CRange.h>

typedef std::shared_ptr<CGnuPlotPlot> CGnuPlotPlotP;

class CGnuPlotGroup {
 public:
  typedef CGnuPlotTypes::HistogramStyle      HistogramStyle;
  typedef CGnuPlotTypes::DrawLayer           DrawLayer;
  typedef CGnuPlotTypes::PlotStyle           PlotStyle;
  typedef CGnuPlotTypes::AxisType            AxisType;
  typedef CGnuPlotTypes::VariableName        VariableName;
  typedef CGnuPlot::Annotations              Annotations;
  typedef std::map<VariableName,Annotations> VarAnnotations;
  typedef std::vector<CGnuPlotPlotP>         Plots;
  typedef std::map<int, CGnuPlotAxis *>      IAxes;
  typedef std::map<AxisType, IAxes>          Axes;
  typedef std::map<int, CRange>              IndRange;

 public:
  CGnuPlotGroup(CGnuPlotWindow *window);

  virtual ~CGnuPlotGroup();

  void initObjects();

  CGnuPlotWindow *window() const { return window_; }

  CGnuPlot *app() const;

  int id() const { return id_; }

  int ind() const { return ind_; }
  void setInd(int ind) { ind_ = ind; }

  bool is2D() const { return ! is3D(); }
  void set2D(bool b);

  void set3D(bool b);
  bool is3D() const { return is3D_; }

  CGnuPlotTitleP title() const { return title_; }

  void cameraChanged();

  void setTitleData(const CGnuPlotTitleData &t);

  const Plots &plots() const { return plots_; }

  void clearSubPlots();

  void addSubPlot(CGnuPlotPlotP &plot);

  void addSubPlots(const Plots &plots);

  CGnuPlotPlotP createNewPlot(CGnuPlotTypes::PlotStyle style=CGnuPlotTypes::PlotStyle::LINES);

  //---

  void init();

  void addObjects();

  void addAnnotation(CGnuPlotGroupAnnotation *ann);

  const VarAnnotations &annotations() const { return varAnnotations_; }
  void setAnnotations(const VarAnnotations &annotations);

  const Annotations &varAnnotations(const VariableName &type) const {
    return const_cast<CGnuPlotGroup *>(this)->varAnnotations_[type];
  }

  void fit();

  void fitHistograms(COptReal &xmin1, COptReal &xmax1, COptReal &ymin1, COptReal &ymax1);

  void fitSinglePlot(CGnuPlotPlot *singlePlot, COptReal &xmin1, COptReal &xmax1,
                     COptReal &ymin1, COptReal &ymax1);

  void fitParallelAxes(COptReal &xmin1, COptReal &xmax1, COptReal &ymin1, COptReal &ymax1);

  //---

  const CGnuPlotAxesData &axesData() const { return axesData_; }
  void setAxesData(const CGnuPlotAxesData &a) { axesData_ = a; }

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
  const CGnuPlotAxisData &axis(AxisType type, int ind) const {
    return const_cast<CGnuPlotGroup *>(this)->axis(type, ind);
  }

  CGnuPlotAxisData &xaxis(int ind) { return axesData_.xaxis(ind); }
  CGnuPlotAxisData &yaxis(int ind) { return axesData_.yaxis(ind); }
  CGnuPlotAxisData &zaxis(int ind) { return axesData_.zaxis(ind); }

  CGnuPlotAxisData &paxis(int ind) { return this->axesData_.paxis(ind); }
  CGnuPlotAxisData &taxis(int ind) { return this->axesData_.taxis(ind); }

  CGnuPlotAxisData &raxis() { return this->axesData_.raxis(); }
  CGnuPlotAxisData &uaxis() { return this->axesData_.uaxis(); }
  CGnuPlotAxisData &vaxis() { return this->axesData_.vaxis(); }

  CGnuPlotAxisData &axis(AxisType type, int ind) { return this->axesData_.axis(type, ind); }

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

#if 0
  void setRange(const CBBox2D &b) {
    setXMin(b.getXMin()); setYMin(b.getYMin());
    setXMax(b.getXMax()); setYMax(b.getYMax());
  }
#endif

  void saveRange();
  void restoreRange();

  //---

  const std::string &getXLabel() const { return xaxis(1).text(); }
  void setXLabel(const std::string &s) { xaxis(1).setText(s); }

  const std::string &getYLabel() const { return yaxis(1).text(); }
  void setYLabel(const std::string &s) { yaxis(1).setText(s); }

  //---

  bool isPolar() const { return polar_; }
  void setPolar(bool b) { polar_ = b; }

  //---

  CGnuPlotCameraP camera() const { return camera_; }

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

  double cameraPlaneZMin() const;

  //---

  bool isHidden3D() const { return hidden3D_; }
  void setHidden3D(bool b) { hidden3D_ = b; }

  bool isHiddenGrayScale() const { return hiddenGrayScale_; }
  void setHiddenGrayScale(bool b) { hiddenGrayScale_ = b; }

  CGnuPlotPm3DP pm3D() const { return pm3D_; }
  void setPm3D(CGnuPlotPm3DP p) { pm3D_ = p; }

  //---

  CGnuPlotTimeStampP timeStamp() const { return timeStamp_; }

  //---

  int getBorderSides() const { return axesData_.getBorderSides(); }
  void setBorderSides(int b) { axesData_.setBorderSides(b); }

  const DrawLayer &getBorderLayer() const { return axesData_.getBorderLayer(); }
  void setBorderLayer(const DrawLayer &l) { axesData_.setBorderLayer(l); }

  double getBorderWidth() const { return axesData_.getBorderWidth(); }
  void setBorderWidth(double w) { axesData_.setBorderWidth(w); }

  int getBorderStyle() const { return axesData_.getBorderStyle(); }
  void setBorderStyle(int ls) { axesData_.setBorderStyle(ls); }

  int getBorderType() const { return axesData_.getBorderType(); }
  void setBorderType(int lt) { axesData_.setBorderType(lt); }

  //---

  void normalizeXRange(int ind, double &xmin, double &xmax, double xi=0) const;
  void normalizeYRange(int ind, double &ymin, double &ymax, double yi=0) const;
  void normalizeZRange(int ind, double &zmin, double &zmax, double zi=0) const;

  CBBox2D getClip(int xind=1, int yind=1) const;

  //---

  const COptBBox2D &clearRect() const { return clearRect_; }
  void setClearRect(const COptBBox2D &r) { clearRect_ = r; }

  //---

  const CGnuPlotKeyP &key() const { return key_; }

  const CGnuPlotColorBoxP &colorBox() const { return colorBox_; }
  CGnuPlotColorBoxP colorBox() { return colorBox_; }

  const CGnuPlotPaletteP &palette() const { return palette_; }
  void setPalette(const CGradientPalette &pal);

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

  double marginLeft  () const { return margin_.left  ().realValue(); }
  double marginRight () const { return margin_.right ().realValue(); }
  double marginTop   () const { return margin_.top   ().realValue(); }
  double marginBottom() const { return margin_.bottom().realValue(); }

  void setMarginLeft  (double r) { margin_.setLeft  (r); }
  void setMarginRight (double r) { margin_.setRight (r); }
  void setMarginTop   (double r) { margin_.setTop   (r); }
  void setMarginBottom(double r) { margin_.setBottom(r); }

  const CGnuPlotMargin &margin() const { return margin_; }
  void setMargin(const CGnuPlotMargin &m) { margin_ = m; }

  //-----

  double dataRangeLeft  () const { return xrange(1).low (0); }
  double dataRangeRight () const { return xrange(1).high(1); }
  double dataRangeBottom() const { return yrange(1).low (0); }
  double dataRangeTop   () const { return yrange(1).high(1); }

  void setDataRangeLeft  (double r) { xrange_[1].setLow (r); }
  void setDataRangeRight (double r) { xrange_[1].setHigh(r); }
  void setDataRangeBottom(double r) { yrange_[1].setLow (r); }
  void setDataRangeTop   (double r) { yrange_[1].setHigh(r); }

  const CRange &xrange(int ind) const { return const_cast<CGnuPlotGroup *>(this)->xrange_[ind]; }
  void setXRange(int ind, const CRange &r) { xrange_[ind] = r; }

  const CRange &yrange(int ind) const { return const_cast<CGnuPlotGroup *>(this)->yrange_[ind]; }
  void setYRange(int ind, const CRange &r) { yrange_[ind] = r; }

  //-----

  const CGnuPlotClip &clip() const { return clip_; }
  void setClip(const CGnuPlotClip &clip) { clip_ = clip; }

  //-----

  const CGnuPlotPlotSize &plotSize() const { return plotSize_; }
  void setPlotSize(const CGnuPlotPlotSize &s) { plotSize_ = s; }

  //-----

  const CGnuPlotHistogramData &getHistogramData() const { return histogramData_; }
  void setHistogramData(const CGnuPlotHistogramData &data) { histogramData_ = data; }

  const CGnuPlotNewHistogramDatas &newHistogramDatas() const { return newHistogramDatas_; }
  void setNewHistogramDatas(const CGnuPlotNewHistogramDatas &v) { newHistogramDatas_ = v; }

  const CGnuPlotNewHistogramData &newHistogramData(int i) const {
    assert(i >= 0 && i < int(newHistogramDatas_.size()));
    return newHistogramDatas_[i];
  }

  void setHistogramDatas(const CGnuPlotHistogramData &data,
                         const CGnuPlotNewHistogramDatas &newDatas);

  double histogramGap() const { return getHistogramData().gap().getValue(0); }
  void setHistogramGap(double g) { histogramData_.setGap(g); }

  //-----

  const Axes &axes() const { return axes_; }

  //-----

  void showXAxis(bool show);
  void showYAxis(bool show);

  void setAxisRange(AxisType type, int ind, double r1, double r2);

  void setAxisStart(AxisType type, int ind, double r);
  void setAxisEnd  (AxisType type, int ind, double r);

  void setAxisGridDisplayed(AxisType type, int ind, bool b);

  void updatePlotAxisRange(AxisType type, int ind);

  //-----

  void reset3D();

  //-----

  virtual void mousePress  (const CGnuPlotMouseEvent &mouseEvent);
  virtual void mouseMove   (const CGnuPlotMouseEvent &mouseEvent, bool pressed);
  virtual void mouseRelease(const CGnuPlotMouseEvent &mouseEvent);

  virtual void keyPress(const CGnuPlotKeyEvent   &keyEvent  );

  virtual bool mouseProbe(CGnuPlotProbeEvent &probeEvent);

  //-----

  void updatePosition(const CPoint2D &pixel, CGnuPlotPosition &pos) const;

  void displayPixelCoordinates(const CPoint2D &pixel) const;

  virtual void draw();

  void drawClearRect(CGnuPlotRenderer *renderer);

  void drawTitle(CGnuPlotRenderer *renderer);

  void drawHistogram(CGnuPlotRenderer *renderer, const Plots &plots);

  void drawRowStackedHistograms(CGnuPlotRenderer *renderer, double xp, const Plots &plots);

  void drawColumnStackedHistograms(CGnuPlotRenderer *renderer, const Plots &plots);

  void drawAxes(CGnuPlotRenderer *renderer, bool border);

  void drawBorder(CGnuPlotRenderer *renderer);

  void drawBorderLine(CGnuPlotRenderer *renderer, const CPoint3D &p1, const CPoint3D &p2);
  void drawBorderLine(CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2);

  void drawXAxis(CGnuPlotRenderer *renderer, int xind);
  void drawYAxis(CGnuPlotRenderer *renderer, int yind);
  void drawZAxis(CGnuPlotRenderer *renderer, int yind);

  void drawRAxis(CGnuPlotRenderer *renderer);

  void drawGrid(CGnuPlotRenderer *renderer, const CGnuPlot::DrawLayer &layer);

  void drawKey(CGnuPlotRenderer *renderer);

  void drawColorBox(CGnuPlotRenderer *renderer);

  void drawAnnotations(CGnuPlotRenderer *renderer, DrawLayer layer);

  CGnuPlotPlotP getSingleStylePlot() const;

  CGnuPlotAxis *getPlotAxis(AxisType type, int ind, bool create=false) const;

  //-----

  void calcRange(int xind, int yind, double &xmin, double &ymin, double &xmax, double &ymax) const;

  CBBox3D getPlotBorderBox(int xind, int yind, int zind) const;

  CBBox2D getMappedDisplayRange(int xind, int yind) const;
  CBBox3D getMappedDisplayRange3D() const;

  CBBox2D getDisplayRange(int xind, int yind) const;

  //-----

  bool hasPlotStyle(PlotStyle plotStyle) const;
  bool hasImageStyle() const;

  void getPlotsOfStyle(Plots &plots, PlotStyle plotStyle) const;

  //-----

  CBBox2D getRegionBBox() const;

  const CBBox2D &bbox2D() const { return bbox2D_; }
  const CBBox3D &bbox3D() const { return bbox3D_; }

  //-----

  const CBBox2D &axisBBox2D() const { return axisBBox2D_; }
  const CBBox3D &axisBBox3D() const { return axisBBox3D_; }

  void updateAxisBBox(int xind, int yind, int zind, const CPoint3D &p);
  void updateAxisBBox(int xind, int yind, const CPoint2D &p);
  void updateAxisBBox(int xind, int yind, const CBBox2D &box);

  //-----

  const CBBox2D &getKeyBBox  () const { return keyBBox_   ; }
  const CBBox2D &getMarginBox() const { return marginBBox_; }

  void updateKeyBBox(const CBBox2D &box) { keyBBox_.add(box); }

  void updateMarginBBox(const CBBox2D &box) { marginBBox_.add(box); }

  //------

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &r) { backgroundColor_ = r; }

  //-----

  CPoint3D mapLogPoint(int xind, int yind, int zind, const CPoint3D &p) const;
  CPoint2D mapLogPoint(int xind, int yind, int zind, const CPoint2D &p) const;
  void     mapLogPoint(int xind, int yind, int zind, double *x, double *y, double *z) const;

  CPoint3D unmapLogPoint(int xind, int yind, int zind, const CPoint3D &p) const;
  CPoint2D unmapLogPoint(int xind, int yind, int zind, const CPoint2D &p) const;
  void     unmapLogPoint(int xind, int yind, int zind, double *x, double *y, double *z) const;

  //-----

  CGnuPlotPlotP getPlotForId(int id) const;

 protected:
  static int nextId_;

  CGnuPlotWindow*           window_;                          // parent window
  int                       id_   { 0 };                      // unique id
  int                       ind_  { 0 };                      // group index in window
  bool                      is3D_ { false };                  // plots are 3D
  bool                      polar_ { false };                 // is polar
  CGnuPlotTitleP            title_;                           // plot title
  Plots                     plots_;                           // plots
  CBBox2D                   region_ {0,0,1,1};                // region of window
  CGnuPlotMargin            margin_;                          // margin around plots
  IndRange                  xrange_;                          // x data ranges
  IndRange                  yrange_;                          // y data ranges
  CBBox2D                   bbox2D_     { 0, 0, 1, 1 };       // bounding box
  CBBox3D                   bbox3D_     { 0, 0, 0, 1, 1, 1 }; // bounding box
  CBBox2D                   axisBBox2D_ { 0, 0, 1, 1 };       // axis 2D bounding box
  CBBox3D                   axisBBox3D_ { 0, 0, 0, 1, 1, 1 }; // axis 3D bounding box
  CBBox2D                   keyBBox_    { 0, 0, 1, 1 };       // key bounding box
  CBBox2D                   marginBBox_ { 0, 0, 1, 1 };       // margin bounding box
  CGnuPlotClip              clip_;                            // clip
  COptBBox2D                clearRect_;                       // optional clear rectangle
  CGnuPlotPlotSize          plotSize_;                        // plot size
  CGnuPlotHistogramData     histogramData_;                   // histogram data
  CGnuPlotNewHistogramDatas newHistogramDatas_;               // new histogram datas
  CGnuPlotKeyP              key_;                             // key
  CGnuPlotColorBoxP         colorBox_;                        // color box
  CGnuPlotPaletteP          palette_;                         // palette
  CGnuPlotAxesData          axesData_;                        // axes data
  CGnuPlotAxesData          initAxesData_;                    // axes data
  bool                      fitted_ { false };
  VarAnnotations            varAnnotations_;                  // annotations
  Axes                      axes_;                            // axes
  CGnuPlotCameraP           camera_;                          // view camera
  bool                      hidden3D_ { false };              // hidden 3d
  bool                      hiddenGrayScale_ { false };       // hidden 3d
  CGnuPlotPm3DP             pm3D_;                            // pm3d data
  CGnuPlotTimeStampP        timeStamp_;                       // time stamp
  CRGBA                     backgroundColor_;                 // background color
  mutable CBBox3D           saveRange_;                       // save range
};

typedef std::shared_ptr<CGnuPlotGroup> CGnuPlotGroupP;

#endif
