#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotStyleBase.h>

int CGnuPlotGroup::nextId_ = 1;

CGnuPlotGroup::
CGnuPlotGroup(CGnuPlotWindow *window) :
 window_(window), id_(nextId_++)
{
  key_      = CGnuPlotKeyP     (app()->createKey     (this));
  colorBox_ = CGnuPlotColorBoxP(app()->createColorBox(this));
  palette_  = CGnuPlotPaletteP (app()->createPalette (this));
  title_    = CGnuPlotTitleP   (app()->createTitle   (this));

  camera_ = app()->createCamera(this);

  timeStamp_ = app()->createTimeStamp(this);
}

CGnuPlot *
CGnuPlotGroup::
app() const
{
  if (window_)
    return window_->app();
  else
    return 0;
}

void
CGnuPlotGroup::
init()
{
  CGnuPlot *plot = this->app();

  setBackgroundColor(plot->backgroundColor());

  setRegion(plot->region());
  setMargin(plot->margin());

  setHistogramData(plot->histogramData());

  key_->setKeyData(plot->keyData());

  colorBox_->init(plot->colorBox());
  palette_ ->init(plot->palette ());
  camera_  ->init(plot->camera  ());

  timeStamp_->setData(plot->timeStamp());

  setClip(plot->clip());

  for (int ind = 1; ind <= 2; ++ind) {
    CGnuPlotAxisData &axisData = this->xaxis(ind);

    if (axisData.min().isValid() || axisData.max().isValid()) {
      double xmin = axisData.min().getValue(0.0);
      double xmax = axisData.max().getValue(1.0);

      normalizeXRange(ind, xmin, xmax);

      if (axisData.min().isValid()) axisData.setMin(xmin);
      if (axisData.max().isValid()) axisData.setMax(xmax);
    }
  }

  for (int ind = 1; ind <= 2; ++ind) {
    CGnuPlotAxisData &axisData = this->yaxis(ind);

    if (axisData.min().isValid() || axisData.max().isValid()) {
      double ymin = axisData.min().getValue(0.0);
      double ymax = axisData.max().getValue(1.0);

      normalizeYRange(ind, ymin, ymax);

      if (axisData.min().isValid()) axisData.setMin(ymin);
      if (axisData.max().isValid()) axisData.setMax(ymax);
    }
  }

  if (is3D()) {
    CGnuPlotAxisData &axisData = this->zaxis(1);

    if (axisData.min().isValid() || axisData.max().isValid()) {
      double zmin = axisData.min().getValue(0.0);
      double zmax = axisData.max().getValue(1.0);

      normalizeZRange(1, zmin, zmax);

      if (axisData.min().isValid()) axisData.setMin(zmin);
      if (axisData.max().isValid()) axisData.setMax(zmax);
    }
  }

  setAxesData(plot->axesData());

  setPolar(plot->isPolar());

  hidden3D_ = plot->hidden3D().enabled;

  pm3D_ = CGnuPlotPm3DP(plot->createPm3D(this));

  pm3D_->setData(plot->pm3D());
}

void
CGnuPlotGroup::
setHistogramData(const CGnuPlotHistogramData &data)
{
  histogramData_ = data;

  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    xaxis(1).resetMin(); xaxis(1).resetMax();
    yaxis(1).resetMin(); yaxis(1).resetMax();

    fit();

    updatePlotAxisRange(AxisType::X, 1);
    updatePlotAxisRange(AxisType::Y, 1);
  }
}

void
CGnuPlotGroup::
set3D(bool b)
{
  is3D_ = b;
}

void
CGnuPlotGroup::
cameraChanged()
{
  for (auto plot : plots_)
    plot->resetSurface();
}

void
CGnuPlotGroup::
setTitleData(const CGnuPlotTitleData &t)
{
  title_->setData(t);

  title_->setGroup(this);
}

void
CGnuPlotGroup::
clearSubPlots()
{
  plots_.clear();
}

void
CGnuPlotGroup::
addSubPlot(CGnuPlotPlot *plot)
{
  plot->setInd(plots_.size() + 1);

  plots_.push_back(plot);

  plot->setGroup(this);
}

void
CGnuPlotGroup::
addSubPlots(const Plots &plots)
{
  clearSubPlots();

  for (auto plot : plots)
    addSubPlot(plot);
}

void
CGnuPlotGroup::
addObjects()
{
  CGnuPlot *plot = this->app();

  CGnuPlotDevice *device = plot->device();

  for (auto ann : plot->annotations()) {
    CGnuPlotArrow     *arrow   = 0;
    CGnuPlotCircle    *circle  = 0;
    CGnuPlotEllipse   *ellipse = 0;
    CGnuPlotLabel     *label   = 0;
    CGnuPlotPolygon   *poly    = 0;
    CGnuPlotRectangle *rect    = 0;

    if      ((arrow = dynamic_cast<CGnuPlotArrow *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createArrow(this)->setData(arrow)));
    }
    else if ((circle = dynamic_cast<CGnuPlotCircle *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createCircle(this)->setData(circle)));
    }
    else if ((ellipse = dynamic_cast<CGnuPlotEllipse *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createEllipse(this)->setData(ellipse)));
    }
    else if ((label = dynamic_cast<CGnuPlotLabel *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createLabel(this)->setData(label)));
    }
    else if ((poly = dynamic_cast<CGnuPlotPolygon *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createPolygon(this)->setData(poly)));
    }
    else if ((rect = dynamic_cast<CGnuPlotRectangle *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createRectangle(this)->setData(rect)));
    }
  }
}

void
CGnuPlotGroup::
fit()
{
  CGnuPlotPlot *singlePlot = getSingleStylePlot();

  // data range (ind == 1)
  COptReal xmin1, xmax1, ymin1, ymax1, zmin1, zmax1;
  // data range (ind == 2)
  COptReal xmin2, xmax2, ymin2, ymax2;

  double dx = 0.0, dy = 0.0, dz = 0.0;

  if      (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    Plots plots;

    getPlotsOfStyle(plots, PlotStyle::HISTOGRAMS);

    CGnuPlotBBoxRenderer brenderer(app()->renderer());

    HistogramStyle hstyle = getHistogramData().style();

    CGnuPlotPlot::DrawHistogramData drawData;

    drawData.i  = 0;
    drawData.np = plots.size();
    drawData.xb = 0;

    drawData.w = 1.0/std::max(drawData.np, 1);
    drawData.d = 1.0;

    drawData.x2 = -drawData.np*drawData.w/2.0;

    if      (hstyle == HistogramStyle::CLUSTERED) {
      for (auto plot : plots) {
        plot->drawClusteredHistogram(&brenderer, drawData);

        drawData.x2 += drawData.w;

        ++drawData.i;
      }
    }
    else if (hstyle == HistogramStyle::ERRORBARS) {
      for (auto plot : plots) {
        plot->drawErrorBarsHistogram(&brenderer, drawData);

        drawData.x2 += drawData.w;

        ++drawData.i;
      }
    }
    else if (hstyle == HistogramStyle::ROWSTACKED) {
      drawRowStackedHistograms(&brenderer, plots);
    }
    else if (hstyle == HistogramStyle::COLUMNSTACKED) {
      drawColumnStackedHistograms(&brenderer, plots);
    }

    xmin1.updateMin(brenderer.bbox().getLeft  ());
    ymin1.updateMin(brenderer.bbox().getBottom());
    xmax1.updateMax(brenderer.bbox().getRight ());
    ymax1.updateMax(brenderer.bbox().getTop   ());

    dx = 1.0;

    CGnuPlotAxis *plotXAxis1 = getPlotAxis(AxisType::X, 1, true);
    CGnuPlotAxis *plotYAxis1 = getPlotAxis(AxisType::Y, 1, true);

    plotXAxis1->setDataRange(brenderer.bbox().getLeft  (), brenderer.bbox().getRight());
    plotYAxis1->setDataRange(brenderer.bbox().getBottom(), brenderer.bbox().getTop  ());
  }
  else if (singlePlot) {
    CGnuPlotStyleBase *singleStyle = app()->getPlotStyle(singlePlot->getStyle());

    CBBox2D rect = singleStyle->fit(singlePlot);

    xmin1 = rect.getXMin(); ymin1 = rect.getYMin();
    xmax1 = rect.getXMax(); ymax1 = rect.getYMax();

    CGnuPlotAxis *plotXAxis1 = getPlotAxis(AxisType::X, 1, true);
    CGnuPlotAxis *plotYAxis1 = getPlotAxis(AxisType::Y, 1, true);

    plotXAxis1->setDataRange(xmin1.getValue(), xmax1.getValue());
    plotYAxis1->setDataRange(ymin1.getValue(), ymax1.getValue());
  }
  else if (hasPlotStyle(PlotStyle::PARALLELAXES)) {
    int nc = 0;

    for (auto plot : plots_) {
      const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

      int nc1 = 0;

      for (const auto &p : plot->getPoints2D())
        nc1 = std::max(nc, p.getNumValues());

      if (lineStyle.lineColor().isValid() && lineStyle.lineColor().getValue().isCalc())
        --nc1;

      nc = std::max(nc, nc1);
    }

    xmin1 = 1; xmax1 = nc;
    ymin1 = 0; ymax1 = 1;

    CGnuPlotAxis *plotXAxis1 = getPlotAxis(AxisType::X, 1, true);
    CGnuPlotAxis *plotYAxis1 = getPlotAxis(AxisType::Y, 1, true);

    plotXAxis1->setDataRange(xmin1.getValue(), xmax1.getValue());
    plotYAxis1->setDataRange(ymin1.getValue(), ymax1.getValue());
  }
#if 0
  else if (hasImageStyle()) {
    for (auto plot : plots_) {
      const CGnuPlotImageStyle imageStyle = plot->imageStyle();

      double xmin = 0, xmax = 1;
      double ymin = 0, ymax = 1;

      if (! imageStyle.w.isValid() || ! imageStyle.h.isValid()) {
        int nx = 1, ny = 1;

        if (! is3D()) {
          if (! plot->getPoints2D().empty()) {
            ny = int(plot->getPoints2D().size());
            nx = int(plot->getPoints2D()[0].getNumValues());
          }
        }

        xmin = -1; xmax = nx;
        ymin = -1; ymax = ny;
      }
      else {
        xmax = imageStyle.w.getValue() - 1;
        ymax = imageStyle.h.getValue() - 1;
      }

      xmin1.updateMin(xmin); xmax1.updateMax(xmax);
      ymin1.updateMin(ymin); ymax1.updateMax(ymax);
    }
  }
#endif
  else {
    CGnuPlotAxis *plotXAxis1 = getPlotAxis(AxisType::X, 1, true);
    CGnuPlotAxis *plotXAxis2 = getPlotAxis(AxisType::X, 2, true);

    COptReal xamin1 = xaxis(1).min(), xamax1 = xaxis(1).max();
    COptReal xamin2 = xaxis(2).min(), xamax2 = xaxis(2).max();

    if (xamin1.isValid() || xamax1.isValid()) {
      if (xamin1.isValid()) xmin1 = plotXAxis1->mapLogValue(xamin1.getValue());
      if (xamax1.isValid()) xmax1 = plotXAxis1->mapLogValue(xamax1.getValue());
    }

    if (xamin2.isValid() || xamax2.isValid()) {
      if (xamin2.isValid()) xmin2 = plotXAxis2->mapLogValue(xamin2.getValue());
      if (xamax2.isValid()) xmax2 = plotXAxis2->mapLogValue(xamax2.getValue());
    }

    //---

    CGnuPlotAxis *plotRAxis = getPlotAxis(AxisType::R, 1, true);

    if (isPolar()) {
      if (! xamin1.isValid() && raxis().max().isValid()) {
        xamin1 = -plotRAxis->mapLogValue(raxis().max().getValue());
      }
      if (! xamax1.isValid() && raxis().max().isValid())
        xamax1 =  plotRAxis->mapLogValue(raxis().max().getValue());
    }

    //---

    COptReal xpmin1, xpmax1, xpmin2, xpmax2;
    COptReal cbmin, cbmax;

    for (auto plot : plots_) {
      double xmin, xmax;

      plot->calcXRange(&xmin, &xmax);

      if (plot->xind() == 1) {
        xpmin1.updateMin(xmin);
        xpmax1.updateMax(xmax);
      }
      else {
        xpmin2.updateMin(xmin);
        xpmax2.updateMax(xmax);
      }

      cbmin.updateMin(plot->cbmin());
      cbmax.updateMax(plot->cbmax());
    }

    if (! xamin1.isValid()) xmin1.updateMin(xpmin1);
    if (! xamax1.isValid()) xmax1.updateMax(xpmax1);
    if (! xamin2.isValid()) xmin2.updateMin(xpmin2);
    if (! xamax2.isValid()) xmax2.updateMax(xpmax2);

    //---

    CGnuPlotAxis *plotYAxis1 = getPlotAxis(AxisType::Y, 1, true);
    CGnuPlotAxis *plotYAxis2 = getPlotAxis(AxisType::Y, 2, true);

    COptReal yamin1 = yaxis(1).min(), yamax1 = yaxis(1).max();
    COptReal yamin2 = yaxis(2).min(), yamax2 = yaxis(2).max();

    if (yamin1.isValid() || yamax1.isValid()) {
      if (yamin1.isValid()) ymin1 = plotYAxis1->mapLogValue(yamin1.getValue());
      if (yamax1.isValid()) ymax1 = plotYAxis1->mapLogValue(yamax1.getValue());
    }

    if (yamin2.isValid() || yamax2.isValid()) {
      if (yamin2.isValid()) ymin2 = plotYAxis2->mapLogValue(yamin2.getValue());
      if (yamax2.isValid()) ymax2 = plotYAxis2->mapLogValue(yamax2.getValue());
    }

    if (isPolar()) {
      if (! yamin1.isValid() && raxis().max().isValid())
        yamin1 = -plotRAxis->mapLogValue(raxis().max().getValue());
      if (! yamax1.isValid() && raxis().max().isValid())
        yamax1 =  plotRAxis->mapLogValue(raxis().max().getValue());
    }

    if (! colorBox_->axis().min().isValid() && cbmin.isValid())
      colorBox_->axis().setMin(cbmin.getValue());
    if (! colorBox_->axis().max().isValid() && cbmax.isValid())
      colorBox_->axis().setMax(cbmax.getValue());

    //---

    COptReal ypmin1, ypmax1, ypmin2, ypmax2;

    for (auto plot : plots_) {
      double ymin, ymax;

      if (! plot->calcBoundedYRange(&ymin, &ymax))
        continue;

      if (plot->yind() == 1) {
        ypmin1.updateMin(ymin);
        ypmax1.updateMax(ymax);
      }
      else {
        ypmin2.updateMin(ymin);
        ypmax2.updateMax(ymax);
      }
    }

    if (! yamin1.isValid()) ymin1.updateMin(ypmin1);
    if (! yamax1.isValid()) ymax1.updateMax(ypmax1);
    if (! yamin2.isValid()) ymin2.updateMin(ypmin2);
    if (! yamax2.isValid()) ymax2.updateMax(ypmax2);

    //---

    COptReal zpmin1, zpmax1;

    if (is3D()) {
      COptReal zamin1 = zaxis(1).min(); zmin1 = zamin1;
      COptReal zamax1 = zaxis(1).max(); zmax1 = zamax1;

      for (auto plot : plots_) {
        double zmin, zmax;

        plot->calcZRange(&zmin, &zmax);

        zpmin1.updateMin(zmin);
        zpmax1.updateMax(zmax);
      }

      if (! zamin1.isValid()) zmin1.updateMin(zpmin1);
      if (! zamax1.isValid()) zmax1.updateMax(zpmax1);
    }

    //---

    if (isPolar()) {
      double xmax2 = (xamax1.isValid() ? xamax1.getValue(10) : xmax1.getValue(10));
      double ymax2 = (yamax1.isValid() ? yamax1.getValue(10) : ymax1.getValue(10));

      double r = plotRAxis->mapLogValue(std::max(xmax2, ymax2));

      if (! xmin1.isValid()) xmin1 = -r;
      if (! xmax1.isValid()) xmax1 =  r;
      if (! ymin1.isValid()) ymin1 = -r;
      if (! ymax1.isValid()) ymax1 =  r;
    }

    //---

    if (xpmin1.isValid()) { plotXAxis1->setDataRange(xpmin1.getValue(), xpmax1.getValue()); }
    if (ypmin1.isValid()) { plotYAxis1->setDataRange(ypmin1.getValue(), ypmax1.getValue()); }
    if (xpmin2.isValid()) { plotXAxis2->setDataRange(xpmin2.getValue(), xpmax2.getValue()); }
    if (xpmin2.isValid()) { plotYAxis2->setDataRange(ypmin2.getValue(), ypmax2.getValue()); }
  }

  if (! xmin1.isValid()) xmin1 = -10; if (! xmax1.isValid()) xmax1 = 10;
  if (! ymin1.isValid()) ymin1 = -10; if (! ymax1.isValid()) ymax1 = 10;

  if (! xmin2.isValid()) xmin2 = xmin1; if (! xmax2.isValid()) xmax2 = xmax1;
  if (! ymin2.isValid()) ymin2 = ymin1; if (! ymax2.isValid()) ymax2 = ymax1;

  if (is3D()) {
    if (! zmin1.isValid()) zmin1 = -10; if (! zmax1.isValid()) zmax1 = 10;
  }

  //---

  if (isPolar()) {
    if (! raxis().min().isValid() || ! raxis().max().isValid()) {
      CGnuPlotAxis *plotRAxis = getPlotAxis(AxisType::R, 1, true);

      double xmax = xmax1.getValue();
      double ymax = ymax1.getValue();

      double r = plotRAxis->mapLogValue(std::max(xmax, ymax));

      raxis().setMin(0);
      raxis().setMax(r);
    }

    updatePlotAxisRange(AxisType::R, 1);

    double s = raxis().max().getValue(100) - raxis().min().getValue(0);

    if (raxis().logBase().isValid())
      s = raxis().mapLogValue(s);

    xmin1 = -s; ymin1 = -s;
    xmax1 =  s; ymax1 =  s;
  }

  if (! xaxis(1).min().isValid() || ! xaxis(1).max().isValid()) {
    double xmin = xmin1.getValue();
    double xmax = xmax1.getValue();

    if (! xaxis(1).isAutoScaleFixMin())
      normalizeXRange(1, xmin, xmax, dx);

    xaxis(1).setMin(xmin);
    xaxis(1).setMax(xmax);
  }

  if (! yaxis(1).min().isValid() || ! yaxis(1).max().isValid()) {
    double ymin = ymin1.getValue();
    double ymax = ymax1.getValue();

    if (! yaxis(1).isAutoScaleFixMin())
      normalizeYRange(1, ymin, ymax, dy);

    yaxis(1).setMin(ymin);
    yaxis(1).setMax(ymax);
  }

  if (is3D()) {
    if (! zaxis(1).min().isValid() || ! zaxis(1).max().isValid()) {
      double zmin = zmin1.getValue();
      double zmax = zmax1.getValue();

      if (! zaxis(1).isAutoScaleFixMin())
        normalizeZRange(1, zmin, zmax, dz);

      zaxis(1).setMin(zmin);
      zaxis(1).setMax(zmax);
    }
  }

  updatePlotAxisRange(AxisType::X, 1);
  updatePlotAxisRange(AxisType::Y, 1);

  if (is3D()) {
    updatePlotAxisRange(AxisType::Z, 1);
  }

  //---

  if (! xaxis(2).min().isValid() || ! xaxis(2).max().isValid()) {
    double xmin = xmin2.getValue();
    double xmax = xmax2.getValue();

    if (! xaxis(2).isAutoScaleFixMin())
      normalizeXRange(2, xmin, xmax, dx);

    xaxis(2).setMin(xmin);
    xaxis(2).setMax(xmax);
  }

  if (! yaxis(2).min().isValid() || ! yaxis(2).max().isValid()) {
    double ymin = ymin2.getValue();
    double ymax = ymax2.getValue();

    if (! yaxis(2).isAutoScaleFixMin())
      normalizeYRange(2, ymin, ymax, dy);

    yaxis(2).setMin(ymin);
    yaxis(2).setMax(ymax);
  }

  updatePlotAxisRange(AxisType::X, 2);
  updatePlotAxisRange(AxisType::Y, 2);

  //---

  {
  // fit axes
  CBBox2D bbox = getMappedDisplayRange(1, 1);

  CGnuPlotBBoxRenderer brenderer(app()->renderer());

  axisBBox_ = CBBox2D();

  drawAxes(&brenderer, true);

  marginBBox_ = axisBBox_;

  drawColorBox(&brenderer);

  drawTitle(&brenderer);

  if (marginBBox_.isSet()) {
    double lm = fabs(bbox.getLeft  () - marginBBox_.getLeft  ());
    double bm = fabs(bbox.getBottom() - marginBBox_.getBottom());
    double rm = fabs(bbox.getRight () - marginBBox_.getRight ());
    double tm = fabs(bbox.getTop   () - marginBBox_.getTop   ());

    margin_.updateDefaultValues(&brenderer, lm, bm, rm, tm);
  }
  }
}

void
CGnuPlotGroup::
updatePlotAxisRange(AxisType type, int ind)
{
  CGnuPlotAxis *axis = getPlotAxis(type, ind, false);
  if (! axis) return;

  double start = 0, end = 1;

  if      (type == AxisType::X || type == AxisType::Y || type == AxisType::Z) {
    start = this->axis(type, ind).min().getValue(0.0);
    end   = this->axis(type, ind).max().getValue(1.0);
  }
  else if (type == AxisType::P) {
    start = this->axis(type, ind).min().getValue(0.0);
    end   = this->axis(type, ind).max().getValue(1.0);
  }
  else if (type == AxisType::R) {
    assert(ind  == 1);

    start = this->axis(type, ind).min().getValue(0.0);
    end   = this->axis(type, ind).max().getValue(1.0);
  }
  else if (type == AxisType::T) {
    assert(ind  == 1);

    start = this->axis(type, ind).min().getValue(0.0);
    end   = this->axis(type, ind).max().getValue(1.0);
  }
  else
    assert(false);

  axis->setRange(start, end);
}

void
CGnuPlotGroup::
saveRange()
{
  saveRange_ = CBBox2D(getXMin(), getYMin(), getXMax(), getYMax());
}

void
CGnuPlotGroup::
restoreRange()
{
  setAxisRange(AxisType::X, 1, saveRange_.getXMin(), saveRange_.getXMax());
  setAxisRange(AxisType::Y, 1, saveRange_.getYMin(), saveRange_.getYMax());
}

void
CGnuPlotGroup::
setCameraEnabled(bool b)
{
  camera_->setEnabled(b);

  reset3D();
}

void
CGnuPlotGroup::
setCameraRotateX(double a)
{
  camera_->setRotateX(a);

  reset3D();
}

void
CGnuPlotGroup::
setCameraRotateY(double a)
{
  camera_->setRotateY(a);

  reset3D();
}

void
CGnuPlotGroup::
setCameraRotateZ(double a)
{
  camera_->setRotateZ(a);

  reset3D();
}

void
CGnuPlotGroup::
setCameraXMin(double x)
{
  camera_->setXMin(x);

  reset3D();
}

void
CGnuPlotGroup::
setCameraXMax(double x)
{
  camera_->setXMax(x);

  reset3D();
}

void
CGnuPlotGroup::
setCameraYMin(double y)
{
  camera_->setYMin(y);

  reset3D();
}

void
CGnuPlotGroup::
setCameraYMax(double y)
{
  camera_->setYMax(y);

  reset3D();
}

void
CGnuPlotGroup::
setCameraNear(double z)
{
  camera_->setNear(z);

  reset3D();
}

void
CGnuPlotGroup::
setCameraFar(double z)
{
  camera_->setFar(z);

  reset3D();
}

void
CGnuPlotGroup::
normalizeXRange(int ind, double &xmin, double &xmax, double xi) const
{
  CGnuPlotAxis *plotAxis = getPlotAxis(AxisType::X, ind, true);

  if (plotAxis->logBase().isValid())
    return;

  double xmin1, xmax1;
  int    numTicks1, numTicks2;

  (void) CGnuPlotAxis::calcTics(xmin, xmax, xi, xmin1, xmax1, numTicks1, numTicks2);

  xmin = xmin1;
  xmax = xmax1;
}

void
CGnuPlotGroup::
normalizeYRange(int ind, double &ymin, double &ymax, double yi) const
{
  CGnuPlotAxis *plotAxis = getPlotAxis(AxisType::Y, ind, true);

  if (plotAxis->logBase().isValid())
    return;

  double ymin1, ymax1;
  int    numTicks1, numTicks2;

  (void) CGnuPlotAxis::calcTics(ymin, ymax, yi, ymin1, ymax1, numTicks1, numTicks2);

  ymin = ymin1;
  ymax = ymax1;
}

void
CGnuPlotGroup::
normalizeZRange(int /*ind*/, double &zmin, double &zmax, double zi) const
{
  double zmin1, zmax1;
  int    numTicks1, numTicks2;

  (void) CGnuPlotAxis::calcTics(zmin, zmax, zi, zmin1, zmax1, numTicks1, numTicks2);

  zmin = zmin1;
  zmax = zmax1;
}

CBBox2D
CGnuPlotGroup::
getClip(int xind, int yind) const
{
  CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, xind, true);
  CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, yind, true);

  double xmin = plotXAxis->getStart1();
  double ymin = plotYAxis->getStart1();
  double xmax = plotXAxis->getEnd1  ();
  double ymax = plotYAxis->getEnd1  ();

  return CBBox2D(xmin, ymin, xmax, ymax);
}

void
CGnuPlotGroup::
reset3D()
{
  for (auto plot : plots_)
    plot->reset3D();
}

void
CGnuPlotGroup::
showXAxis(bool show)
{
  CGnuPlotAxis *axis = getPlotAxis(AxisType::X, 1, false);
  if (! axis) return;

  axis->setDisplayed(show);
}

void
CGnuPlotGroup::
showYAxis(bool show)
{
  CGnuPlotAxis *axis = getPlotAxis(AxisType::Y, 1, false);
  if (! axis) return;

  axis->setDisplayed(show);
}

void
CGnuPlotGroup::
draw()
{
  bbox_ = getMappedDisplayRange(1, 1);

  clip_.setRect(bbox_);

  CGnuPlotRenderer *renderer = app()->renderer();

  if (is3D())
    renderer->setCamera(camera_);
  else
    renderer->unsetCamera();

  drawClearRect(renderer);

  //renderer->clear(backgroundColor());

  renderer->setRegion(region());
  renderer->setMargin(margin());

  if (plotSize().xratio.isValid())
    renderer->setRatio(COptReal(plotSize().xratio.getValue()));
  else
    renderer->unsetRatio();

  drawAnnotations(DrawLayer::BEHIND);
  drawAnnotations(DrawLayer::BACK);

  //---

  if (! colorBox_->isFront())
    drawColorBox(renderer);

  //---

  // draw axes (underneath plot)
  axisBBox_ = CBBox2D();

  drawAxes(renderer, getBorderLayer() == CGnuPlotTypes::DrawLayer::FRONT);
//renderer->drawRect(axisBBox_, CRGBA(1,0,0), 1);

//CBBox2D bbox = getMappedDisplayRange(1, 1);
//renderer->setRange(bbox);

  marginBBox_ = axisBBox_;

  //---

  // draw title (TODO: order)
  drawTitle(renderer);

  timeStamp_->draw(renderer);
//renderer->drawRect(marginBBox_, CRGBA(1,0,0), 1);

  //---

#if 0
  double lm = fabs(bbox_.getLeft  () - marginBBox_.getLeft  ());
  double bm = fabs(bbox_.getBottom() - marginBBox_.getBottom());
  double rm = fabs(bbox_.getRight () - marginBBox_.getRight ());
  double tm = fabs(bbox_.getTop   () - marginBBox_.getTop   ());

  margin_.updateDefaultValues(renderer, lm, bm, rm, tm);
#endif

  //---

  // draw grid
  drawGrid(renderer, CGnuPlotTypes::DrawLayer::BACK);

  //---

  // draw plot

  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    Plots hplots;

    getPlotsOfStyle(hplots, PlotStyle::HISTOGRAMS);

    drawHistogram(hplots);
  }
  else {
    for (auto plot : plots_) {
      plot->initRenderer(renderer);

      plot->draw();
    }
  }

  //---

  if (getBorderLayer() == CGnuPlotTypes::DrawLayer::FRONT)
    drawBorder(renderer);

  // draw grid
  drawGrid(renderer, CGnuPlotTypes::DrawLayer::FRONT);

  //---

  drawKey();

  if (colorBox_->isFront())
    drawColorBox(renderer);

  //---

  // draw front
  drawAnnotations(DrawLayer::FRONT);
}

void
CGnuPlotGroup::
drawClearRect(CGnuPlotRenderer *renderer)
{
  if (! clearRect_.isValid()) return;

  const CBBox2D &clearRect = clearRect_.getValue();

  CPoint2D p1, p2;

  renderer->regionToPixel(clearRect.getLL(), p1);
  renderer->regionToPixel(clearRect.getUR(), p2);

  CPoint2D p3, p4;

  renderer->pixelToWindow(p1, p3);
  renderer->pixelToWindow(p2, p4);

  renderer->fillRect(CBBox2D(p3, p4), window()->backgroundColor());
}

CGnuPlotAxis *
CGnuPlotGroup::
getPlotAxis(AxisType type, int ind, bool create) const
{
  CGnuPlotGroup *th = const_cast<CGnuPlotGroup *>(this);

  auto p = th->axes_.find(type);

  if (p == th->axes_.end()) {
    if (! create) return 0;

    p = th->axes_.insert(p, Axes::value_type(type, IAxes()));
  }

  IAxes &iaxis = (*p).second;

  auto p1 = iaxis.find(ind);

  if (p1 == iaxis.end()) {
    if (! create) return 0;

    const CGnuPlotAxisData &axisData = this->axis(type, ind);

    CGnuPlotAxis *axis = app()->createAxis(th, axisData);

    axis->setGridLayer(axesData_.getGridLayer());

    axis->setGridPolarAngle    (axesData_.gridPolarAngle());
    axis->setGridMajorLineStyle(axesData_.gridMajorLineStyle());
    axis->setGridMinorLineStyle(axesData_.gridMinorLineStyle());
    axis->setGridMajorLineType (axesData_.gridMajorLineType());
    axis->setGridMinorLineType (axesData_.gridMinorLineType());
    axis->setGridMajorLineWidth(axesData_.gridMajorLineWidth());
    axis->setGridMinorLineWidth(axesData_.gridMinorLineWidth());

    p1 = iaxis.insert(p1, IAxes::value_type(ind, axis));

    axis->setDrawTickLabel(axisData.isShowTics());

    // style customization
    if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
      axis->setDrawMinorTickMark(false);
    }

    if      (type == AxisType::X) {
      if      (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
        axis->setMajorIncrement(1);
      }
      else if (hasPlotStyle(PlotStyle::PARALLELAXES)) {
        axis->setMajorIncrement(1);
      }
      else if (hasPlotStyle(PlotStyle::BOXPLOT)) {
        axis->setMajorIncrement(1);

        axis->setDrawTickMark     (false);
        axis->setDrawTickLabel    (false);
        axis->setDrawMinorTickMark(false);
      }
    }
    else if (type == AxisType::Y) {
    }
    else if (type == AxisType::Z) {
    }
    else if (type == AxisType::R) {
      CPoint3D center(0, 0, 0);

      if (isPolar())
        center = CPoint3D(1, 1, 1);

      //---

      CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, 1, true);

    //double xmin = plotXAxis->getStart1();
      double xmax = plotXAxis->getEnd1  ();

      axis->setRange(center.x, xmax);
    }

    if (type == AxisType::P) {
      axis->setParallel(true);
    }

    if (axisData.ticIncr().isValid())
      axis->setMajorIncrement(axisData.ticIncr().getValue());

    th->updatePlotAxisRange(type, ind);
  }

  return (*p1).second;
}

void
CGnuPlotGroup::
setAxisRange(AxisType type, int ind, double r1, double r2)
{
  CGnuPlotAxis *axis = getPlotAxis(type, ind, false);
  if (! axis) return;

  axis->setRange(r1, r2);

  this->axis(type, ind).setMin(axis->getStart1());
  this->axis(type, ind).setMax(axis->getEnd1  ());
}


void
CGnuPlotGroup::
setAxisStart(AxisType type, int ind, double r)
{
  CGnuPlotAxis *axis = getPlotAxis(type, ind, false);
  if (! axis) return;

  double start = r;
  double end   = axis->getEnd();

  axis->setRange(start, end);

  this->axis(type, ind).setMin(axis->getStart1());
  this->axis(type, ind).setMax(axis->getEnd1  ());
}

void
CGnuPlotGroup::
setAxisEnd(AxisType type, int ind, double r)
{
  CGnuPlotAxis *axis = getPlotAxis(type, ind, false);
  if (! axis) return;

  double start = axis->getStart();
  double end   = r;

  axis->setRange(start, end);

  this->axis(type, ind).setMin(axis->getStart1());
  this->axis(type, ind).setMax(axis->getEnd1  ());
}

void
CGnuPlotGroup::
setAxisGridDisplayed(AxisType type, int ind, bool b)
{
  CGnuPlotAxis *axis = getPlotAxis(type, ind, false);
  if (! axis) return;

  axis->setGridMajor(b);
}

void
CGnuPlotGroup::
drawTitle(CGnuPlotRenderer *renderer)
{
  if (! is3D() || pm3D()->isEnabled()) {
    CBBox2D bbox = getMappedDisplayRange(1, 1);

    renderer->setRange(bbox);
  }
  else
    renderer->setRange(region());

  renderer->setReverse(false, false);

  title_->draw(renderer);
}

void
CGnuPlotGroup::
drawHistogram(const Plots &plots)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CBBox2D bbox = getMappedDisplayRange(1, 1);

  renderer->setRange(bbox);
  renderer->setClip(bbox);

  //---

  HistogramStyle hstyle = getHistogramData().style();

  CGnuPlotPlot::DrawHistogramData drawData;

  drawData.i  = 0;
  drawData.np = plots.size();
  drawData.xb = renderer->pixelWidthToWindowWidth(2);

  drawData.w = 1.0/std::max(drawData.np, 1); // TODO: bbox width
  drawData.d = 1.0;

  drawData.x2 = -drawData.np*drawData.w/2.0;
  drawData.y2 = std::max(0.0, bbox.getBottom());

  if      (hstyle == HistogramStyle::CLUSTERED) {
    CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, 1, true);
    CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, 1, true);

    renderer->setReverse(plotXAxis->isReverse(), plotYAxis->isReverse());

    for (auto plot : plots) {
      plot->drawClusteredHistogram(renderer, drawData);

      ++drawData.i;
    }
  }
  else if (hstyle == HistogramStyle::ERRORBARS) {
    CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, 1, true);
    CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, 1, true);

    renderer->setReverse(plotXAxis->isReverse(), plotYAxis->isReverse());

    for (auto plot : plots) {
      plot->drawErrorBarsHistogram(renderer, drawData);

      ++drawData.i;
    }
  }
  else if (hstyle == HistogramStyle::ROWSTACKED) {
    drawRowStackedHistograms(renderer, plots);
  }
  else if (hstyle == HistogramStyle::COLUMNSTACKED) {
    drawColumnStackedHistograms(renderer, plots);
  }
  else {
    app()->errorMsg("Unsupported histogram style");
  }
}

void
CGnuPlotGroup::
drawRowStackedHistograms(CGnuPlotRenderer *renderer, const Plots &plots)
{
  uint numPoints = 0;

  for (auto plot : plots)
    numPoints = std::max(numPoints, plot->numPoints2D());

  if (numPoints == 0)
    return;

  if (! renderer->isPseudo()) {
    for (auto plot : plots)
      plot->updateBarCacheSize(numPoints);
  }

  //---

  double xb = 0;

  if (! renderer->isPseudo())
    xb = renderer->pixelWidthToWindowWidth(2);

  double w = 1.0 - 2*xb;
  double x = 0.5 + xb;

  for (uint i = 0; i < numPoints; ++i) {
    double h = 0;

    for (auto plot : plots) {
      const CGnuPlotPoint &point = plot->getPoint2D(i);

      double y;

      if (! point.getY(y))
        y = 0.0;

      CBBox2D bbox1(x, h, x + w, h + y);

      plot->drawStackedHistogram(renderer, i, bbox1);

      h += y;
    }

    x += 1.0;
  }

  for (auto plot : plots)
    plot->drawBars(renderer);
}

void
CGnuPlotGroup::
drawColumnStackedHistograms(CGnuPlotRenderer *renderer, const Plots &plots)
{
  if (! renderer->isPseudo()) {
    for (auto plot : plots)
      plot->updateBarCacheSize(plot->numPoints2D());
  }

  //---

  double xb = 0;

  if (! renderer->isPseudo())
    xb = renderer->pixelWidthToWindowWidth(2);

  double w = 1.0 - 2*xb;
  double x = 0.5 + xb;

  //---

  for (auto plot : plots) {
    double h1 = 0, h2 = 0;

    for (uint i = 0; i < plot->numPoints2D(); ++i) {
      const CGnuPlotPoint &point = plot->getPoint2D(i);

      double y;

      if (! point.getY(y))
        y = 0.0;

      if (y >= 0) {
        CBBox2D bbox1(x, h1, x + w, h1 + y);

        plot->drawStackedHistogram(renderer, i, bbox1);

        h1 += y;
      }
      else {
        CBBox2D bbox1(x, h2, x + w, h2 + y);

        plot->drawStackedHistogram(renderer, i, bbox1);

        h2 += y;
      }
    }

    x += 1.0;
  }

  for (auto plot : plots)
    plot->drawBars(renderer);
}

#if 0
void
CGnuPlotGroup::
calcHistogramRange(const Plots &plots, CBBox2D &bbox) const
{
  uint numPoints = 0;

  for (auto plot : plots)
    numPoints = std::max(numPoints, plot->numPoints2D());

  //---

  double xmin = 0, xmax = 1;
  double ymin = 0, ymax = 1;

  HistogramStyle hstyle = getHistogramData().style();

  if      (hstyle == HistogramStyle::CLUSTERED) {
    xmin = 0;
    xmax = plots.size()*numPoints + 1;

    COptReal ymin1, ymax1;

    for (auto plot : plots) {
      CBBox2D bbox;

      plot->getPointsRange(bbox);

      ymin1.updateMin(bbox.getYMin());
      ymax1.updateMax(bbox.getYMax());
    }

    ymin = ymin1.getValue(0);
    ymax = ymax1.getValue(1);
  }
  else if (hstyle == HistogramStyle::ERRORBARS) {
    xmin = 0;
    xmax = plots.size()*numPoints + 1;

    COptReal ymin1, ymax1;

    for (auto plot : plots) {
      CBBox2D bbox;

      plot->getPointsRange(bbox);

      ymin1.updateMin(bbox.getYMin());
      ymax1.updateMax(bbox.getYMax());
    }

    ymin = ymin1.getValue(0);
    ymax = ymax1.getValue(1);
  }
  else if (hstyle == HistogramStyle::ROWSTACKED) {
    xmin = 0;
    xmax = numPoints + 1;

    ymin = 0;
    ymax = 0;

    for (auto plot : plots) {
      CBBox2D bbox;

      plot->getPointsRange(bbox);

      if (bbox.getYMin() < 0)
        ymin += bbox.getYMin();

      if (bbox.getYMax() > 0)
        ymax += bbox.getYMax();
    }
  }
  else if (hstyle == HistogramStyle::COLUMNSTACKED) {
    xmin = 0;
    xmax = plots.size() + 1;

    ymin = 0;
    ymax = 0;

    for (auto plot : plots) {
      double ymin1 = 0, ymax1 = 0;

      for (uint i = 0; i < plot->numPoints2D(); ++i) {
        const CGnuPlotPoint &point = plot->getPoint2D(i);

        double y;

        if (! point.getY(y))
          y = 0.0;

        if (y >= 0)
          ymax1 += y;
        else
          ymin1 += y;
      }

      ymin = std::min(ymin, ymin1);
      ymax = std::max(ymax, ymax1);
    }
  }

  bbox = CBBox2D(xmin, ymin, xmax, ymax);
}
#endif

void
CGnuPlotGroup::
drawAxes(CGnuPlotRenderer *renderer, bool border)
{
  if (hasPlotStyle(PlotStyle::TEST_TERMINAL))
    return;

  if (is3D()) {
    if (border)
      drawBorder(renderer);

    drawXAxis(renderer, 1);
    drawYAxis(renderer, 1);

    if (! pm3D()->isEnabled())
      drawZAxis(renderer, 1);
  }
  else {
    CGnuPlotPlot *singlePlot = getSingleStylePlot();

    if (singlePlot) {
      CGnuPlotStyleBase *singleStyle = app()->getPlotStyle(singlePlot->getStyle());

      singleStyle->drawAxes(singlePlot, renderer);

      return;
    }

    //---

    if (border)
      drawBorder(renderer);

    if (! hasPlotStyle(PlotStyle::PARALLELAXES)) {
      for (int i = 1; i <= 2; ++i) {
        drawXAxis(renderer, i);
        drawYAxis(renderer, i);
      }
    }
    else {
      drawXAxis(renderer, 1);
    }

    if (isPolar()) {
      drawRAxis(renderer);
    }
  }
}

void
CGnuPlotGroup::
drawBorder(CGnuPlotRenderer *renderer)
{
  if (hasPlotStyle(PlotStyle::TEST_TERMINAL))
    return;

  uint sides = axesData().getBorderSides();

  CBBox2D bbox = getMappedDisplayRange(1, 1);

  renderer->setRange(bbox);

  CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, 1, true);
  CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, 1, true);

  renderer->setReverse(plotXAxis->isReverse(), plotYAxis->isReverse());

  double xmin1 = bbox.getLeft  (), ymin1 = bbox.getBottom();
  double xmax1 = bbox.getRight (), ymax1 = bbox.getTop   ();

  if (is3D()) {
    double zmin1 = zaxis(1).min().getValue(-10);
    double zmax1 = zaxis(1).max().getValue( 10);

    if (! axesData().getBorderSides())
      return;

    updateAxisBBox(CPoint3D(xmin1, ymin1, zmin1));
    updateAxisBBox(CPoint3D(xmin1, ymin1, zmax1));
    updateAxisBBox(CPoint3D(xmin1, ymax1, zmin1));
    updateAxisBBox(CPoint3D(xmin1, ymax1, zmax1));
    updateAxisBBox(CPoint3D(xmax1, ymin1, zmin1));
    updateAxisBBox(CPoint3D(xmax1, ymin1, zmax1));
    updateAxisBBox(CPoint3D(xmax1, ymax1, zmin1));
    updateAxisBBox(CPoint3D(xmax1, ymax1, zmax1));

    // 1 : x front, bottom
    if (sides & (1<<0))
      drawBorderLine(renderer, CPoint3D(xmin1, ymin1, zmin1), CPoint3D(xmax1, ymin1, zmin1));

    // 2 : y left, bottom
    if (sides & (1<<1))
      drawBorderLine(renderer, CPoint3D(xmin1, ymin1, zmin1), CPoint3D(xmin1, ymax1, zmin1));

    // 4 : y right, bottom
    if (sides & (1<<2))
      drawBorderLine(renderer, CPoint3D(xmax1, ymin1, zmin1), CPoint3D(xmax1, ymax1, zmin1));

    // 8 : x back, bottom
    if (sides & (1<<3))
      drawBorderLine(renderer, CPoint3D(xmin1, ymax1, zmin1), CPoint3D(xmax1, ymax1, zmin1));

    //---

    // 16 : vertical, left, front
    if (sides & (1<<4))
      drawBorderLine(renderer, CPoint3D(xmin1, ymin1, zmin1), CPoint3D(xmin1, ymin1, zmax1));

    // 32 : vertical, left, back
    if (sides & (1<<5))
      drawBorderLine(renderer, CPoint3D(xmin1, ymax1, zmin1), CPoint3D(xmin1, ymax1, zmax1));

    // 64 : vertical, right, back
    if (sides & (1<<6))
      drawBorderLine(renderer, CPoint3D(xmax1, ymax1, zmin1), CPoint3D(xmax1, ymax1, zmax1));

    // 128 : vertical, right, front
    if (sides & (1<<7))
      drawBorderLine(renderer, CPoint3D(xmax1, ymin1, zmin1), CPoint3D(xmax1, ymin1, zmax1));

    //---

    // 256: y left, top
    if (sides & (1<<8))
      drawBorderLine(renderer, CPoint3D(xmin1, ymin1, zmax1), CPoint3D(xmin1, ymax1, zmax1));

    // 512: x back, top
    if (sides & (1<<9))
      drawBorderLine(renderer, CPoint3D(xmin1, ymax1, zmax1), CPoint3D(xmax1, ymax1, zmax1));

    // 1024: x front, top
    if (sides & (1<<10))
      drawBorderLine(renderer, CPoint3D(xmin1, ymin1, zmax1), CPoint3D(xmax1, ymin1, zmax1));

    // 2048: y right, top
    if (sides & (1<<11))
      drawBorderLine(renderer, CPoint3D(xmax1, ymin1, zmax1), CPoint3D(xmax1, ymax1, zmax1));
  }
  else {
    if (! axesData().getBorderSides())
      return;

    updateAxisBBox(CPoint2D(xmin1, ymin1));
    updateAxisBBox(CPoint2D(xmin1, ymax1));
    updateAxisBBox(CPoint2D(xmax1, ymin1));
    updateAxisBBox(CPoint2D(xmax1, ymax1));

    // 1 : x axis (bottom)
    if (sides & (1<<0))
      drawBorderLine(renderer, CPoint2D(xmin1, ymin1), CPoint2D(xmax1, ymin1));

    // 2 : y axis (left)
    if (sides & (1<<1))
      drawBorderLine(renderer, CPoint2D(xmin1, ymin1), CPoint2D(xmin1, ymax1));

    // 4 : x axis (top)
    if (sides & (1<<2))
      drawBorderLine(renderer, CPoint2D(xmin1, ymax1), CPoint2D(xmax1, ymax1));

    // 8 : y axis (right)
    if (sides & (1<<3))
      drawBorderLine(renderer, CPoint2D(xmax1, ymin1), CPoint2D(xmax1, ymax1));
  }
}

void
CGnuPlotGroup::
drawBorderLine(CGnuPlotRenderer *renderer, const CPoint3D &p1, const CPoint3D &p2)
{
  CRGBA c(0,0,0);

  double bw = axesData().getBorderWidth();

  renderer->drawLine(p1, p2, bw, c);
}

void
CGnuPlotGroup::
drawBorderLine(CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CRGBA c(0,0,0);

  double bw = axesData().getBorderWidth();

  renderer->drawLine(p1, p2, bw, c);
}

void
CGnuPlotGroup::
drawXAxis(CGnuPlotRenderer *renderer, int xind)
{
  CGnuPlotAxisData &xaxis = this->xaxis(xind);

  CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, xind, true);

  if (! plotXAxis->isDisplayed())
    return;

  //---

  renderer->setRange(getMappedDisplayRange(xind, 1));
  renderer->setReverse(xaxis.isReverse(), false);

  // draw major/minor (bottom/top)
  plotXAxis->drawAxes(renderer);
}

void
CGnuPlotGroup::
drawYAxis(CGnuPlotRenderer *renderer, int yind)
{
  CGnuPlotAxisData &yaxis = this->yaxis(yind);

  CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, yind, true);

  if (! plotYAxis->isDisplayed())
    return;

  //---

  renderer->setRange(getMappedDisplayRange(1, yind));
  renderer->setReverse(false, yaxis.isReverse());

  // draw major/minor (left/right)
  plotYAxis->drawAxes(renderer);
}

void
CGnuPlotGroup::
drawZAxis(CGnuPlotRenderer *renderer, int zind)
{
  CGnuPlotAxisData &zaxis = this->zaxis(zind);

  CGnuPlotAxis *plotZAxis = getPlotAxis(AxisType::Z, zind, true);

  if (! plotZAxis->isDisplayed())
    return;

  //---

  renderer->setRange(getMappedDisplayRange(1, 1));
  renderer->setReverse(zaxis.isReverse(), false);

  // draw major/minor (bottom/top)
  plotZAxis->drawAxes(renderer);
}

// polar (raxis) ?
void
CGnuPlotGroup::
drawRAxis(CGnuPlotRenderer *renderer)
{
  const CGnuPlotAxisData &axis = this->raxis();

  CGnuPlotAxis *plotAxis = getPlotAxis(AxisType::R, 1, true);

  if (! plotAxis->isDisplayed())
    return;

  //---

  renderer->setRange(getMappedDisplayRange(1, 1));
  renderer->setReverse(axis.isReverse(), false);

  // draw single (major)
  plotAxis->drawAxes(renderer);
}

void
CGnuPlotGroup::
drawGrid(CGnuPlotRenderer *renderer, const CGnuPlotTypes::DrawLayer &layer)
{
  if (hasPlotStyle(PlotStyle::TEST_TERMINAL))
    return;

  bool isBack = (layer == CGnuPlotTypes::DrawLayer::BACK);

  if (! is3D()) {
    if (! isPolar()) {
      for (int i = 1; i <= 2; ++i) {
        CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, i, true);

        if (plotXAxis->hasGrid() && plotXAxis->isGridBackLayer(/*back*/true) == isBack) {
          renderer->setRange(getMappedDisplayRange(i, 1));
          renderer->setReverse(plotXAxis->isReverse(), false);

          plotXAxis->drawGrid(renderer);
        }

        CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, i, true);

        if (plotYAxis->hasGrid() && plotYAxis->isGridBackLayer(/*back*/true) == isBack) {
          renderer->setRange(getMappedDisplayRange(1, i));
          renderer->setReverse(false, plotYAxis->isReverse());

          plotYAxis->drawGrid(renderer);
        }
      }
    }
    else {
      CGnuPlotAxis *plotRAxis = getPlotAxis(AxisType::R, 1, true);

      if (plotRAxis->hasGrid() && plotRAxis->isGridBackLayer(/*back*/true) == isBack) {
        plotRAxis->drawRadialGrid(renderer);
      }
    }
  }
  else {
    for (int i = 1; i <= 2; ++i) {
      CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, i, true);

      // x/y grid at zmin
      if (plotXAxis->hasGrid() && plotXAxis->isGridBackLayer(/*back*/false) == isBack) {
        renderer->setRange(getMappedDisplayRange(i, 1));
        renderer->setReverse(plotXAxis->isReverse(), false);

        plotXAxis->drawGrid(renderer);
      }

      // y/z grid at xmin
      CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, i, true);

      if (plotYAxis->hasGrid() && plotYAxis->isGridBackLayer(/*back*/false) == isBack) {
        renderer->setRange(getMappedDisplayRange(1, i));
        renderer->setReverse(false, plotYAxis->isReverse());

        plotYAxis->drawGrid(renderer);
      }

      // z/x grid at ymin
      CGnuPlotAxis *plotZAxis = getPlotAxis(AxisType::Z, i, true);

      if (plotZAxis->hasGrid() && plotZAxis->isGridBackLayer(/*back*/false) == isBack) {
        renderer->setRange(getMappedDisplayRange(i, 1));
        renderer->setReverse(plotZAxis->isReverse(), false);

        plotZAxis->drawGrid(renderer);
      }
    }
  }
}

void
CGnuPlotGroup::
drawKey()
{
  if (hasPlotStyle(PlotStyle::TEST_TERMINAL))
    return;

  CGnuPlotRenderer *renderer = app()->renderer();

  CGnuPlotPlot *singlePlot = getSingleStylePlot();

  if (singlePlot) {
    CGnuPlotStyleBase *singleStyle = app()->getPlotStyle(singlePlot->getStyle());

    singleStyle->drawKey(singlePlot, renderer);

    return;
  }

  // TODO: key drawn in own coord system
  // TODO: always fill background, opaque draws on top of plots
  CBBox2D bbox = getMappedDisplayRange(1, 1);

  renderer->setRange(bbox);
  renderer->setReverse(false, false);

  key_->draw(renderer);
}

CGnuPlotPlot *
CGnuPlotGroup::
getSingleStylePlot() const
{
  for (const auto &p : app()->plotStyles()) {
    CGnuPlotStyleBase *style = p.second;

    if (style->isSingleType() && hasPlotStyle(style->style())) {
      for (auto plot : plots_) {
        if (plot->getStyle() != style->style())
          continue;

        return plot;
      }
    }
  }

  return 0;
}

void
CGnuPlotGroup::
drawColorBox(CGnuPlotRenderer *renderer)
{
  if (hasPlotStyle(PlotStyle::TEST_TERMINAL))
    return;

  colorBox_->draw(renderer);
}

void
CGnuPlotGroup::
drawAnnotations(DrawLayer layer)
{
  if (hasPlotStyle(PlotStyle::TEST_TERMINAL))
    return;

  // draw labels last
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRange(getMappedDisplayRange(1, 1));

  renderer->setReverse(xaxis(1).isReverse(), yaxis(1).isReverse());

  for (const auto &ann : annotations_)
    if (ann->getLayer() == layer)
      ann->draw(renderer);
}

void
CGnuPlotGroup::
calcRange(int xind, int yind, double &xmin, double &ymin, double &xmax, double &ymax) const
{
  COptReal xmin1, xmax1;

  for (auto plot : plots_) {
    if (plot->xind() != xind) continue;

    double xmin2, xmax2;

    plot->calcXRange(&xmin2, &xmax2);

    xmin1.updateMin(xmin2);
    xmax1.updateMax(xmax2);
  }

  xmin = xmin1.getValue(0);
  xmax = xmax1.getValue(1);

  //---

  COptReal ymin1, ymax1;

  for (auto plot : plots_) {
    if (plot->yind() != yind) continue;

    double ymin2, ymax2;

    if (! plot->calcBoundedYRange(&ymin2, &ymax2))
      continue;

    ymin1.updateMin(ymin2);
    ymax1.updateMax(ymax2);
  }

  ymin = ymin1.getValue(0);
  ymax = ymax1.getValue(1);
}

CBBox3D
CGnuPlotGroup::
getPlotBorderBox(int xind, int yind, int zind) const
{
  if (is3D()) {
    if (! xaxis(xind).min().isValid() || ! xaxis(xind).max().isValid() ||
        ! yaxis(yind).min().isValid() || ! yaxis(yind).max().isValid() ||
        ! zaxis(zind).min().isValid() || ! zaxis(yind).max().isValid()) {
      CGnuPlotGroup *th = const_cast<CGnuPlotGroup *>(this);

      th->fit();
    }
  }
  else {
    if (! xaxis(xind).min().isValid() || ! xaxis(xind).max().isValid() ||
        ! yaxis(yind).min().isValid() || ! yaxis(yind).max().isValid()) {
      CGnuPlotGroup *th = const_cast<CGnuPlotGroup *>(this);

      th->fit();
    }
  }

  double xmin = xaxis(xind).min().getValue(-10);
  double ymin = yaxis(yind).min().getValue(-10);
  double zmin = zaxis(zind).min().getValue(-10);
  double xmax = xaxis(xind).max().getValue( 10);
  double ymax = yaxis(yind).max().getValue( 10);
  double zmax = zaxis(zind).max().getValue( 10);

  CPoint3D p1 = mapLogPoint(xind, yind, zind, CPoint3D(xmin, ymin, zmin));
  CPoint3D p2 = mapLogPoint(xind, yind, zind, CPoint3D(xmax, ymax, zmax));

  return CBBox3D(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

CBBox2D
CGnuPlotGroup::
getMappedDisplayRange(int xind, int yind) const
{
  CBBox2D bbox = getDisplayRange(xind, yind);

  CPoint2D p1 = mapLogPoint(xind, yind, 1, bbox.getMin());
  CPoint2D p2 = mapLogPoint(xind, yind, 1, bbox.getMax());

  return CBBox2D(p1, p2);
}

CBBox2D
CGnuPlotGroup::
getDisplayRange(int xind, int yind) const
{
  if (! xaxis(xind).min().isValid() || ! yaxis(yind).min().isValid() ||
      ! xaxis(xind).max().isValid() || ! yaxis(yind).max().isValid()) {
    CGnuPlotGroup *th = const_cast<CGnuPlotGroup *>(this);

    th->fit();
  }

  double xmin = xaxis(xind).min().getValue(-10);
  double xmax = xaxis(xind).max().getValue( 10);
  double ymin = yaxis(yind).min().getValue(-10);
  double ymax = yaxis(yind).max().getValue( 10);

  return CBBox2D(xmin, ymin, xmax, ymax);
}

#if 0
CBBox2D
CGnuPlotGroup::
calcDisplayRange(int xind, int yind) const
{
  CBBox2D bbox;

  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    Plots hplots;

    getPlotsOfStyle(hplots, PlotStyle::HISTOGRAMS);

    calcHistogramRange(hplots, bbox);

    CGnuPlotAxis plotYAxis(const_cast<CGnuPlotGroup *>(this), "y1",
                           CGnuPlotTypes::AxisDirection::Y);

    plotYAxis.setRange(bbox.getYMin(), bbox.getYMax());

    bbox.setYMin(plotYAxis.getStart1());
    bbox.setYMax(plotYAxis.getEnd1  ());
  }
  else {
    double xmin, ymin, xmax, ymax;

    calcRange(xind, yind, xmin, ymin, xmax, ymax);

    normalizeXRange(xind, xmin, xmax);
    normalizeYRange(yind, xmin, xmax);

    bbox = CBBox2D(xmin, ymin, xmax, ymax);
  }

  return bbox;
}
#endif

bool
CGnuPlotGroup::
hasPlotStyle(PlotStyle style) const
{
  for (auto plot : plots_) {
    if (plot->getStyle() == style)
      return true;
  }

  return false;
}

bool
CGnuPlotGroup::
hasImageStyle() const
{
  for (auto plot : plots_) {
    if (plot->isImageStyle())
      return true;
  }

  return false;
}

void
CGnuPlotGroup::
getPlotsOfStyle(Plots &plots, PlotStyle style) const
{
  for (auto plot : plots_) {
    if (plot->getStyle() == style)
      plots.push_back(plot);
  }
}

CBBox2D
CGnuPlotGroup::
getRegionBBox() const
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  const CISize2D &s = window()->size();

  double x1 = region().getXMin()*(s.width  - 1);
  double y1 = region().getYMin()*(s.height - 1);
  double x2 = region().getXMax()*(s.width  - 1);
  double y2 = region().getXMax()*(s.height - 1);

  CPoint2D p1, p2;

  renderer->pixelToWindow(CPoint2D(x1, y1), p1);
  renderer->pixelToWindow(CPoint2D(x2, y2), p2);

  return CBBox2D(p1, p2);
}

void
CGnuPlotGroup::
updateAxisBBox(const CPoint3D &p)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CPoint2D p1 = renderer->transform(p);

  updateAxisBBox(p1);
}

#if 0
CPoint2D
CGnuPlotGroup::
windowToPixel(const CPoint2D &p) const
{
  double px, py;

  windowToPixel(p.x, p.y, &px, &py);

  return CPoint2D(px, py);
}

void
CGnuPlotGroup::
windowToPixel(double wx, double wy, double *px, double *py) const
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  renderer->windowToPixel(wx, wy, px, py);
}

CPoint2D
CGnuPlotGroup::
pixelToWindow(const CPoint2D &p) const
{
  double wx, wy;

  pixelToWindow(p.x, p.y, &wx, &wy);

  return CPoint2D(wx, wy);
}

void
CGnuPlotGroup::
pixelToWindow(double px, double py, double *wx, double *wy) const
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  renderer->pixelToWindow(px, py, wx, wy);
}

double
CGnuPlotGroup::
pixelWidthToWindowWidth(double w) const
{
  CPoint2D w1 = pixelToWindow(CPoint2D(0, 0));
  CPoint2D w2 = pixelToWindow(CPoint2D(w, w));

  return w2.x - w1.x;
}

double
CGnuPlotGroup::
pixelHeightToWindowHeight(double h) const
{
  CPoint2D w1 = pixelToWindow(CPoint2D(0, 0));
  CPoint2D w2 = pixelToWindow(CPoint2D(h, h));

  return w1.y - w2.y;
}
#endif

CPoint2D
CGnuPlotGroup::
convertPolarAxisPoint(const CPoint2D &p, bool &inside) const
{
  CGnuPlotGroup *th = const_cast<CGnuPlotGroup *>(this);

  CGnuPlotAxis *plotTAxis = th->getPlotAxis(AxisType::T, 1, true);
  CGnuPlotAxis *plotRAxis = th->getPlotAxis(AxisType::R, 1, true);

  CPoint2D p1 = p;

  inside = plotTAxis->mappedInside(p1.x) && plotRAxis->mappedInside(p1.y);

  if (! plotRAxis->logBase().isValid()) {
    double rmin = plotRAxis->getStart();
    double rmax = plotRAxis->getEnd  ();
    double s    = rmax - rmin;

    p1.y = CGnuPlotUtil::map(p1.y, rmin, rmax, 0, s);
  }

  p1 = app()->convertPolarPoint(p1);

  return p1;
}

CPoint2D
CGnuPlotGroup::
mapLogPoint(int xind, int yind, int zind, const CPoint2D &p) const
{
  CPoint3D p1(p.x, p.y, 0);

  mapLogPoint(xind, yind, zind, &p1.x, &p1.y, &p1.z);

  return CPoint2D(p1.x, p1.y);
}

CPoint3D
CGnuPlotGroup::
mapLogPoint(int xind, int yind, int zind, const CPoint3D &p) const
{
  CPoint3D p1(p);

  mapLogPoint(xind, yind, zind, &p1.x, &p1.y, &p1.z);

  return p1;
}

CPoint2D
CGnuPlotGroup::
unmapLogPoint(int xind, int yind, int zind, const CPoint2D &p) const
{
  CPoint3D p1(p.x, p.y, 0);

  unmapLogPoint(xind, yind, zind, &p1.x, &p1.y, &p1.z);

  return CPoint2D(p1.x, p1.y);
}

CPoint3D
CGnuPlotGroup::
unmapLogPoint(int xind, int yind, int zind, const CPoint3D &p) const
{
  CPoint3D p1(p);

  unmapLogPoint(xind, yind, zind, &p1.x, &p1.y, &p1.z);

  return p1;
}

void
CGnuPlotGroup::
mapLogPoint(int xind, int yind, int zind, double *x, double *y, double *z) const
{
  CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, xind, true);
  CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, yind, true);

  *x = plotXAxis->mapLogValue(*x);
  *y = plotYAxis->mapLogValue(*y);

  if (is3D()) {
    CGnuPlotAxis *plotZAxis = getPlotAxis(AxisType::Z, zind, true);

    *z = plotZAxis->mapLogValue(*z);
  }
}

void
CGnuPlotGroup::
unmapLogPoint(int xind, int yind, int zind, double *x, double *y, double *z) const
{
  if (! isPolar()) {
    CGnuPlotAxis *plotXAxis = getPlotAxis(AxisType::X, xind, true);
    CGnuPlotAxis *plotYAxis = getPlotAxis(AxisType::Y, yind, true);

    *x = plotXAxis->unmapLogValue(*x);
    *y = plotYAxis->unmapLogValue(*y);

    if (is3D()) {
      CGnuPlotAxis *plotZAxis = getPlotAxis(AxisType::Z, zind, true);

      *z = plotZAxis->unmapLogValue(*z);
    }
  }
  else {
    CGnuPlotAxis *plotRAxis = getPlotAxis(AxisType::R, 1, true);

    *x = CGnuPlotUtil::sign(*x)*plotRAxis->unmapLogValue(fabs(*x));
    *y = CGnuPlotUtil::sign(*y)*plotRAxis->unmapLogValue(fabs(*y));
    *z = CGnuPlotUtil::sign(*z)*plotRAxis->unmapLogValue(fabs(*z));
  }
}
