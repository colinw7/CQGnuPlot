#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotStyleBase.h>

namespace {
  std::string encodeAxisId(char c, int ind) {
    char id[3] = { c, char('0' + ind), 0 };

    return std::string(&id[0]);
  }

  void decodeAxisId(const std::string &id, char &c, int &ind) {
    c   = id[0];
    ind = id[1] - '0';
  }
}

int CGnuPlotGroup::nextId_ = 1;

CGnuPlotGroup::
CGnuPlotGroup(CGnuPlotWindow *window) :
 window_(window), id_(nextId_++)
{
  key_      = CGnuPlotKeyP     (app()->createKey     (this));
  colorBox_ = CGnuPlotColorBoxP(app()->createColorBox(this));
  palette_  = CGnuPlotPaletteP (app()->createPalette (this));

  title_  = app()->createTitle (this);
  camera_ = app()->createCamera(this);
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

  setClip(plot->clip());

  setAxesData(plot->axesData());

  for (int ind = 1; ind <= 2; ++ind) {
    double xmin = xaxis(ind).min().getValue(0.0);
    double xmax = xaxis(ind).max().getValue(1.0);

    normalizeXRange(xmin, xmax);

    if (xaxis(ind).min().isValid()) xaxis(ind).setMin(xmin);
    if (xaxis(ind).max().isValid()) xaxis(ind).setMax(xmax);
  }

  for (int ind = 1; ind <= 2; ++ind) {
    double ymin = yaxis(ind).min().getValue(0.0);
    double ymax = yaxis(ind).max().getValue(1.0);

    normalizeYRange(ymin, ymax);

    if (yaxis(ind).min().isValid()) yaxis(ind).setMin(ymin);
    if (yaxis(ind).max().isValid()) yaxis(ind).setMax(ymax);
  }

  if (is3D()) {
    double zmin = zaxis(1).min().getValue(0.0);
    double zmax = zaxis(1).max().getValue(1.0);

    normalizeZRange(zmin, zmax);

    if (zaxis(1).min().isValid()) zaxis(1).setMin(zmin);
    if (zaxis(1).max().isValid()) zaxis(1).setMax(zmax);
  }

  setPolar(plot->isPolar());
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
setTitleData(const CGnuPlotTitle &t)
{
  *title_ = t;

  title_->setGroup(this);
}

void
CGnuPlotGroup::
addSubPlots(const Plots &plots)
{
  plots_.clear();

  for (auto plot : plots) {
    plot->setInd(plots_.size() + 1);

    plots_.push_back(plot);

    plot->setGroup(this);
  }
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

  COptReal xmin1, xmax1, xmin2, xmax2;
  COptReal ymin1, ymax1, ymin2, ymax2;
  COptReal zmin1, zmax1;

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
  }
  else if (singlePlot) {
    CGnuPlotStyleBase *singleStyle = app()->getPlotStyle(singlePlot->getStyle());

    CBBox2D rect = singleStyle->fit(singlePlot);

    xmin1 = rect.getXMin(); ymin1 = rect.getYMin();
    xmax1 = rect.getXMax(); ymax1 = rect.getYMax();
  }
  else if (hasPlotStyle(PlotStyle::PARALLELAXES)) {
    int nc = 0;

    for (auto plot : plots_) {
      int nc1 = 0;

      for (const auto &p : plot->getPoints2D())
        nc1 = std::max(nc, p.getNumValues());

      if (plot->lineStyle().color().isCalc())
        --nc1;

      nc = std::max(nc, nc1);
    }

    xmin1 = 1; xmax1 = nc;
    ymin1 = 0; ymax1 = 1;
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
    COptReal xamin1 = xaxis(1).min(); xmin1 = xamin1;
    COptReal xamax1 = xaxis(1).max(); xmin1 = xamax1;
    COptReal xamin2 = xaxis(2).min(); xmin2 = xamin2;
    COptReal xamax2 = xaxis(2).max(); xmin2 = xamax2;

    for (auto plot : plots_) {
      double xmin, xmax;

      plot->calcXRange(&xmin, &xmax);

      if (plot->xind() == 1) {
        if (! xamin1.isValid())
          xmin1.updateMin(xmin);
        if (! xamax1.isValid())
          xmax1.updateMax(xmax);
      }
      else {
        if (! xamin2.isValid())
          xmin2.updateMin(xmin);
        if (! xamax2.isValid())
          xmax2.updateMax(xmax);
      }
    }

    COptReal yamin1 = yaxis(1).min(); ymin1 = yamin1;
    COptReal yamax1 = yaxis(1).max(); ymax1 = yamax1;
    COptReal yamin2 = yaxis(2).min(); ymin2 = yamin2;
    COptReal yamax2 = yaxis(2).max(); ymax2 = yamax2;

    for (auto plot : plots_) {
      double ymin, ymax;

      plot->calcBoundedYRange(&ymin, &ymax);

      if (plot->yind() == 1) {
        if (! yamin1.isValid())
          ymin1.updateMin(ymin);
        if (! yamax1.isValid())
          ymax1.updateMax(ymax);
      }
      else {
        if (! yamin2.isValid())
          ymin2.updateMin(ymin);
        if (! yamax2.isValid())
          ymax2.updateMax(ymax);
      }
    }

    if (is3D()) {
      COptReal zamin1 = zaxis(1).min(); zmin1 = zamin1;
      COptReal zamax1 = zaxis(1).max(); zmax1 = zamax1;

      for (auto plot : plots_) {
        double zmin, zmax;

        plot->calcZRange(&zmin, &zmax);

        if (! zamin1.isValid())
          zmin1.updateMin(zmin);
        if (! zamax1.isValid())
          zmax1.updateMax(zmax);
      }
    }
  }

  //---

  if (! xmin1.isValid()) xmin1 = -10; if (! xmax1.isValid()) xmax1 = 10;
  if (! xmin2.isValid()) xmin2 = -10; if (! xmax2.isValid()) xmax2 = 10;
  if (! ymin1.isValid()) ymin1 = -10; if (! ymax1.isValid()) ymax1 = 10;
  if (! ymin2.isValid()) ymin2 = -10; if (! ymax2.isValid()) ymax2 = 10;

  if (is3D()) {
    if (! zmin1.isValid()) zmin1 = -10; if (! zmax1.isValid()) zmax1 = 10;
  }

  //---

  if (! xaxis(1).min().isValid() || ! xaxis(1).max().isValid()) {
    double xmin = xmin1.getValue();
    double xmax = xmax1.getValue();

    if (! xaxis(1).isAutoScaleFixMin())
      normalizeXRange(xmin, xmax, dx);

    xaxis(1).setMin(xmin);
    xaxis(1).setMax(xmax);
  }

  if (! yaxis(1).min().isValid() || ! yaxis(1).max().isValid()) {
    double ymin = ymin1.getValue();
    double ymax = ymax1.getValue();

    if (! yaxis(1).isAutoScaleFixMin())
      normalizeYRange(ymin, ymax, dy);

    yaxis(1).setMin(ymin);
    yaxis(1).setMax(ymax);
  }

  if (is3D()) {
    if (! zaxis(1).min().isValid() || ! zaxis(1).max().isValid()) {
      double zmin = zmin1.getValue();
      double zmax = zmax1.getValue();

      if (! zaxis(1).isAutoScaleFixMin())
        normalizeZRange(zmin, zmax, dz);

      zaxis(1).setMin(zmin);
      zaxis(1).setMax(zmax);
    }
  }

  //---

  if (! xaxis(2).min().isValid() || ! xaxis(2).max().isValid()) {
    double xmin = xmin2.getValue();
    double xmax = xmax2.getValue();

    if (! xaxis(2).isAutoScaleFixMin())
      normalizeXRange(xmin, xmax, dx);

    xaxis(2).setMin(xmin);
    xaxis(2).setMax(xmax);
  }

  if (! yaxis(2).min().isValid() || ! yaxis(2).max().isValid()) {
    double ymin = ymin2.getValue();
    double ymax = ymax2.getValue();

    if (! yaxis(2).isAutoScaleFixMin())
      normalizeYRange(ymin, ymax, dy);

    yaxis(2).setMin(ymin);
    yaxis(2).setMax(ymax);
  }
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
  setAxisRange("x1", saveRange_.getXMin(), saveRange_.getXMax());
  setAxisRange("y1", saveRange_.getYMin(), saveRange_.getYMax());
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
normalizeXRange(double &xmin, double &xmax, double xi) const
{
  double xmin1, xmax1;
  int    numTicks1, numTicks2;

  (void) CGnuPlotAxis::calcTics(xmin, xmax, xi, xmin1, xmax1, numTicks1, numTicks2);

  xmin = xmin1;
  xmax = xmax1;
}

void
CGnuPlotGroup::
normalizeYRange(double &ymin, double &ymax, double yi) const
{
  double ymin1, ymax1;
  int    numTicks1, numTicks2;

  (void) CGnuPlotAxis::calcTics(ymin, ymax, yi, ymin1, ymax1, numTicks1, numTicks2);

  ymin = ymin1;
  ymax = ymax1;
}

void
CGnuPlotGroup::
normalizeZRange(double &zmin, double &zmax, double zi) const
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
  double xmin = xaxis(xind).min().getValue(0.0);
  double ymin = yaxis(yind).min().getValue(0.0);
  double xmax = xaxis(xind).max().getValue(1.0);
  double ymax = yaxis(yind).max().getValue(1.0);

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
setHistogramData(const CGnuPlotHistogramData &data)
{
  histogramData_ = data;

  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    xaxis(1).resetMin();
    xaxis(1).resetMax();
    yaxis(1).resetMin();
    yaxis(1).resetMax();

    fit();

    updatePlotAxisRange('x', 1);
    updatePlotAxisRange('y', 1);
  }
}

void
CGnuPlotGroup::
showXAxis(bool show)
{
  auto p = axes_.find("x1");
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  axis->setDisplayed(show);
}

void
CGnuPlotGroup::
showYAxis(bool show)
{
  auto p = axes_.find("y1");
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  axis->setDisplayed(show);
}

void
CGnuPlotGroup::
draw()
{
  bbox_ = getDisplayRange(1, 1);

  clip_.setRect(bbox_);

  CGnuPlotRenderer *renderer = app()->renderer();

  if (is3D())
    renderer->setCamera(camera_);

  drawClearRect(renderer);

  //renderer->clear(backgroundColor());

  renderer->setRegion(region());
  renderer->setMargin(margin().range());

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

  // draw title (TODO: order)
  drawTitle();

  //---

  // draw axes (underneath plot)
  drawAxes(renderer);

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
getPlotAxis(char c, int ind)
{
  std::string id = encodeAxisId(c, ind);

  auto p = axes_.find(id);

  if (p == axes_.end()) {
    CGnuPlotAxis::Direction dir;

    if      (c == 'x') dir = CGnuPlotAxis::Direction::X;
    else if (c == 'y') dir = CGnuPlotAxis::Direction::Y;
    else if (c == 'z') dir = CGnuPlotAxis::Direction::Z;
    else if (c == 'p') dir = CGnuPlotAxis::Direction::Y;
    else if (c == 'r') dir = CGnuPlotAxis::Direction::X;
    else if (c == 't') dir = CGnuPlotAxis::Direction::Y;
    else               assert(false);

    CGnuPlotAxis *axis = app()->createAxis(this, id, dir);

    p = axes_.insert(p, Axes::value_type(id, axis));

    updatePlotAxisRange(c, ind);
  }

  return (*p).second;
}

void
CGnuPlotGroup::
setAxisRange(const std::string &id, double r1, double r2)
{
  auto p = axes_.find(id);
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  axis->setRange(r1, r2);

  char c;
  int  ind;

  decodeAxisId(id, c, ind);

  if      (c == 'x') {
    xaxis(ind).setMin(axis->getStart());
    xaxis(ind).setMax(axis->getEnd  ());
  }
  else if (c == 'y') {
    yaxis(ind).setMin(axis->getStart());
    yaxis(ind).setMax(axis->getEnd  ());
  }
}


void
CGnuPlotGroup::
setAxisStart(const std::string &id, double r)
{
  auto p = axes_.find(id);
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  double start = r;
  double end   = axis->getEnd();

  axis->setRange(start, end);

  char c;
  int  ind;

  decodeAxisId(id, c, ind);

  if      (c == 'x') {
    xaxis(ind).setMin(axis->getStart());
    xaxis(ind).setMax(axis->getEnd  ());
  }
  else if (c == 'y') {
    yaxis(ind).setMin(axis->getStart());
    yaxis(ind).setMax(axis->getEnd  ());
  }
}

void
CGnuPlotGroup::
setAxisEnd(const std::string &id, double r)
{
  auto p = axes_.find(id);
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  double start = axis->getStart();
  double end   = r;

  axis->setRange(start, end);

  char c;
  int  ind;

  decodeAxisId(id, c, ind);

  if      (c == 'x') {
    xaxis(ind).setMin(axis->getStart());
    xaxis(ind).setMax(axis->getEnd  ());
  }
  else if (c == 'y') {
    yaxis(ind).setMin(axis->getStart());
    yaxis(ind).setMax(axis->getEnd  ());
  }
}

void
CGnuPlotGroup::
setAxisGridDisplayed(const std::string &id, bool b)
{
  auto p = axes_.find(id);
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  axis->setGrid(b);
}

void
CGnuPlotGroup::
updatePlotAxisRange(const std::string &id)
{
  char c;
  int  ind;

  decodeAxisId(id, c, ind);

  updatePlotAxisRange(c, ind);
}

void
CGnuPlotGroup::
updatePlotAxisRange(char c, int ind)
{
  std::string id = encodeAxisId(c, ind);

  auto p = axes_.find(id);
  if (p == axes_.end()) return;

  CGnuPlotAxis *axis = (*p).second;

  double start = 0, end = 1;

  if      (c == 'x' || c == 'y' || c == 'z') {
    if      (c == 'x') {
      start = xaxis(ind).min().getValue(0.0);
      end   = xaxis(ind).max().getValue(1.0);
    }
    else if (c == 'y') {
      start = yaxis(ind).min().getValue(0.0);
      end   = yaxis(ind).max().getValue(1.0);
    }
    else if (c == 'z') {
      start = zaxis(ind).min().getValue(0.0);
      end   = zaxis(ind).max().getValue(1.0);
    }
    else
      assert(false);
  }
  else if (c == 'p') {
    start = paxis(ind).min().getValue(0.0);
    end   = paxis(ind).max().getValue(1.0);
  }
  else if (c == 'r') {
    start = raxis().min().getValue(0.0);
    end   = raxis().max().getValue(1.0);
  }
  else
    assert(false);

  axis->setRange(start, end);
}

void
CGnuPlotGroup::
drawTitle()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRange(region());
  renderer->setReverse(false, false);

  title_->draw(renderer);
}

void
CGnuPlotGroup::
drawHistogram(const Plots &plots)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CBBox2D bbox = getDisplayRange(1, 1);

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
    renderer->setReverse(xaxis(1).isReverse(), yaxis(1).isReverse());

    for (auto plot : plots) {
      plot->drawClusteredHistogram(renderer, drawData);

      ++drawData.i;
    }
  }
  else if (hstyle == HistogramStyle::ERRORBARS) {
    renderer->setReverse(xaxis(1).isReverse(), yaxis(1).isReverse());

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
drawAxes(CGnuPlotRenderer *renderer)
{
  if (is3D()) {
    drawBorder(renderer);

    drawXAxis(renderer, 1, false);
    drawYAxis(renderer, 1, false);
    drawZAxis(renderer, 1, false);
  }
  else {
    CGnuPlotPlot *singlePlot = getSingleStylePlot();

    if (singlePlot) {
      CGnuPlotStyleBase *singleStyle = app()->getPlotStyle(singlePlot->getStyle());

      singleStyle->drawAxes(singlePlot, renderer);

      return;
    }

    //---

    drawBorder(renderer);

    if (! hasPlotStyle(PlotStyle::PARALLELAXES)) {
      std::set<int> xSet, ySet;

      for (auto plot : plots_) {
        xSet.insert(plot->xind());
        ySet.insert(plot->yind());
      }

      for (int i = 1; i <= 2; ++i) {
        if (xSet.find(i) == xSet.end() && xaxis(i).text() != "")
          xSet.insert(i);
        if (ySet.find(i) == ySet.end() && yaxis(i).text() != "")
          ySet.insert(i);
      }

      for (const auto &xi : xSet)
        drawXAxis(renderer, xi, xi == 1 && xSet.find(2) == xSet.end());

      for (const auto &yi : ySet)
        drawYAxis(renderer, yi, yi == 1 && ySet.find(2) == ySet.end());
    }
    else {
      drawXAxis(renderer, 1, false);
    }

    if (isPolar()) {
      drawAxis(renderer, raxis(), 'r', 1);
    }
  }
}

void
CGnuPlotGroup::
drawBorder(CGnuPlotRenderer *renderer)
{
  if (! axesData().border.sides)
    return;

  CRGBA c(0,0,0);

  CBBox2D bbox = getDisplayRange(1, 1);

  renderer->setRange(bbox);
  renderer->setReverse(xaxis(1).isReverse(), yaxis(1).isReverse());

  double xmin1 = bbox.getLeft  (), ymin1 = bbox.getBottom();
  double xmax1 = bbox.getRight (), ymax1 = bbox.getTop   ();

  double bw = axesData().border.lineWidth;

  if (is3D()) {
    double zmin1 = zaxis(1).min().getValue(-10);
    double zmax1 = zaxis(1).max().getValue( 10);

    // 1 : x front, bottom
    if (axesData().border.sides & (1<<0))
      renderer->drawLine(CPoint3D(xmin1, ymin1, zmin1), CPoint3D(xmax1, ymin1, zmin1), bw, c);

    // 2 : y left, bottom
    if (axesData().border.sides & (1<<1))
      renderer->drawLine(CPoint3D(xmin1, ymin1, zmin1), CPoint3D(xmin1, ymax1, zmin1), bw, c);

    // 4 : y right, bottom
    if (axesData().border.sides & (1<<2))
      renderer->drawLine(CPoint3D(xmax1, ymin1, zmin1), CPoint3D(xmax1, ymax1, zmin1), bw, c);

    // 8 : x back, bottom
    if (axesData().border.sides & (1<<3))
      renderer->drawLine(CPoint3D(xmin1, ymax1, zmin1), CPoint3D(xmax1, ymax1, zmin1), bw, c);

    //---

    // 16 : vertical, left, front
    if (axesData().border.sides & (1<<4))
      renderer->drawLine(CPoint3D(xmin1, ymin1, zmin1), CPoint3D(xmin1, ymin1, zmax1), bw, c);

    // 32 : vertical, left, back
    if (axesData().border.sides & (1<<5))
      renderer->drawLine(CPoint3D(xmin1, ymax1, zmin1), CPoint3D(xmin1, ymax1, zmax1), bw, c);

    // 64 : vertical, right, back
    if (axesData().border.sides & (1<<6))
      renderer->drawLine(CPoint3D(xmax1, ymax1, zmin1), CPoint3D(xmax1, ymax1, zmax1), bw, c);

    // 128 : vertical, right, front
    if (axesData().border.sides & (1<<7))
      renderer->drawLine(CPoint3D(xmax1, ymin1, zmin1), CPoint3D(xmax1, ymin1, zmax1), bw, c);

    //---

    // 256: y left, top
    if (axesData().border.sides & (1<<8))
      renderer->drawLine(CPoint3D(xmin1, ymin1, zmax1), CPoint3D(xmin1, ymax1, zmax1), bw, c);

    // 512: x back, top
    if (axesData().border.sides & (1<<9))
      renderer->drawLine(CPoint3D(xmin1, ymax1, zmax1), CPoint3D(xmax1, ymax1, zmax1), bw, c);

    // 1024: x front, top
    if (axesData().border.sides & (1<<10))
      renderer->drawLine(CPoint3D(xmin1, ymin1, zmax1), CPoint3D(xmax1, ymin1, zmax1), bw, c);

    // 2048: y right, top
    if (axesData().border.sides & (1<<11))
      renderer->drawLine(CPoint3D(xmax1, ymin1, zmax1), CPoint3D(xmax1, ymax1, zmax1), bw, c);
  }
  else {
    // 1 : x axis (bottom)
    if (axesData().border.sides & (1<<0))
      renderer->drawLine(CPoint2D(xmin1, ymin1), CPoint2D(xmax1, ymin1), bw, c);

    // 2 : y axis (left)
    if (axesData().border.sides & (1<<1))
      renderer->drawLine(CPoint2D(xmin1, ymin1), CPoint2D(xmin1, ymax1), bw, c);

    // 4 : x axis (top)
    if (axesData().border.sides & (1<<2))
      renderer->drawLine(CPoint2D(xmin1, ymax1), CPoint2D(xmax1, ymax1), bw, c);

    // 8 : y axis (right)
    if (axesData().border.sides & (1<<3))
      renderer->drawLine(CPoint2D(xmax1, ymin1), CPoint2D(xmax1, ymax1), bw, c);
  }
}

void
CGnuPlotGroup::
drawXAxis(CGnuPlotRenderer *renderer, int xind, bool drawOther)
{
  CGnuPlotAxisData &xaxis = this->xaxis(xind);

  CGnuPlotAxis *plotXAxis = getPlotAxis('x', xind);

  if (! plotXAxis->isDisplayed())
    return;

  CGnuPlotAxis *plotYAxis = getPlotAxis('y', 1);

  double ymin1 = plotYAxis->getStart();
  double ymax1 = plotYAxis->getEnd  ();

  double zmin1 = 0.0;

  if (is3D()) {
    CGnuPlotAxis *plotZAxis = getPlotAxis('z', 1);

    zmin1 = plotZAxis->getStart();
  }

  //---

  if (! plotXAxis->isInitialized()) {
    // defaults
    plotXAxis->setLabel(xaxis.text());

    if (xaxis.logScale().isValid()) {
      plotXAxis->setLogarithmic(true);
      plotXAxis->setLogarithmicBase(xaxis.logScale().getValue());
    }
    else
      plotXAxis->setLogarithmic(false);

    plotXAxis->setDrawLine(false);
    plotXAxis->setDrawTickMark(xaxis.showTics());

    plotXAxis->setTickInside(xaxis.isOutside() ? xind == 1 : xind == 2);
    plotXAxis->setDrawTickLabel(xaxis.showTics());
    plotXAxis->setLabelInside(xind == 2);
    plotXAxis->setDrawLabel(true);

    if (xaxis.isTime())
      plotXAxis->setTimeFormat(xaxis.format());

    //---

    plotXAxis->setEnhanced(xaxis.isEnhanced());
    plotXAxis->setTickInside1(false);
    plotXAxis->setDrawTickLabel1(false);
    plotXAxis->setLabelInside1(false);
    plotXAxis->setDrawLabel1(false);
    plotXAxis->setDrawTickMark1(xaxis.showTics() && xaxis.isMirror());

    //---

    plotXAxis->setGrid(xaxis.hasGrid());
    plotXAxis->setGridMajor(xaxis.hasGridTics());
    plotXAxis->setGridMinor(xaxis.hasGridMinorTics());
    plotXAxis->setGridLayer(axesData_.grid.layer);

    //---

    // style customization
    if      (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
      plotXAxis->setMajorIncrement(1);
      plotXAxis->setDrawMinorTickMark(false);
    }
    else if (hasPlotStyle(PlotStyle::PARALLELAXES)) {
      plotXAxis->setMajorIncrement(1);
    }
    else if (hasPlotStyle(PlotStyle::BOXPLOT)) {
      plotXAxis->setMajorIncrement(1);
      plotXAxis->setDrawTickMark(false);
      plotXAxis->setDrawTickLabel(false);
      plotXAxis->setDrawMinorTickMark(false);
    }

    //---

    plotXAxis->setPosition (CPoint3D(0, xind == 2 ? ymax1 : ymin1, zmin1));
    plotXAxis->setPosition1(CPoint3D(0, ymax1                    , zmin1));

    //---

    plotXAxis->setInitialized(true);
  }

  //---

  renderer->setRange(getDisplayRange(xind, 1));
  renderer->setReverse(xaxis.isReverse(), false);

  // draw major (bottom)
  plotXAxis->drawAxis(renderer, true);

  // draw minor (top)
  if (drawOther)
    plotXAxis->drawAxis(renderer, false);
}

void
CGnuPlotGroup::
drawYAxis(CGnuPlotRenderer *renderer, int yind, bool drawOther)
{
  CGnuPlotAxisData &yaxis = this->yaxis(yind);

  CGnuPlotAxis *plotYAxis = getPlotAxis('y', yind);

  if (! plotYAxis->isDisplayed())
    return;

  //---

  CGnuPlotAxis *plotXAxis = getPlotAxis('x', 1);

  double xmin1 = plotXAxis->getStart();
  double xmax1 = plotXAxis->getEnd  ();

  double zmin1 = 0.0;

  if (is3D()) {
    CGnuPlotAxis *plotZAxis = getPlotAxis('z', 1);

    zmin1 = plotZAxis->getStart();
  }

  //---

  if (! plotYAxis->isInitialized()) {
    if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
      plotYAxis->setDrawMinorTickMark(false);
    }

    //---

    plotYAxis->setLabel(yaxis.text());

    if (yaxis.logScale().isValid()) {
      plotYAxis->setLogarithmic(true);
      plotYAxis->setLogarithmicBase(yaxis.logScale().getValue());
    }
    else
      plotYAxis->setLogarithmic(false);

    plotYAxis->setDrawLine(false);
    plotYAxis->setDrawTickMark(yaxis.showTics());

    plotYAxis->setTickInside(yaxis.isOutside() ? yind == 1 : yind == 2);
    plotYAxis->setDrawTickLabel(yaxis.showTics());
    plotYAxis->setLabelInside(yind == 2);
    plotYAxis->setDrawLabel(true);

    if (yaxis.isTime())
      plotYAxis->setTimeFormat(yaxis.format());

    //---

    plotYAxis->setEnhanced(yaxis.isEnhanced());
    plotYAxis->setTickInside1(false);
    plotYAxis->setDrawTickLabel1(false);
    plotYAxis->setLabelInside1(false);
    plotYAxis->setDrawLabel1(false);
    plotYAxis->setDrawTickMark1(yaxis.showTics() && yaxis.isMirror());

    //---

    plotYAxis->setGrid(yaxis.hasGrid());
    plotYAxis->setGridMajor(yaxis.hasGridTics());
    plotYAxis->setGridMinor(yaxis.hasGridMinorTics());
    plotYAxis->setGridLayer(axesData_.grid.layer);

    //---

    plotYAxis->setPosition (CPoint3D(yind == 2 ? xmax1 : xmin1, 0, zmin1));
    plotYAxis->setPosition1(CPoint3D(xmax1                    , 0, zmin1));

    //---

    plotYAxis->setInitialized(true);
  }

  //---

  renderer->setRange(getDisplayRange(1, yind));
  renderer->setReverse(false, yaxis.isReverse());

  // draw major (left)
  plotYAxis->drawAxis(renderer, true);

  // draw minor (right)
  if (drawOther)
    plotYAxis->drawAxis(renderer, false);
}

void
CGnuPlotGroup::
drawZAxis(CGnuPlotRenderer *renderer, int zind, bool drawOther)
{
  CGnuPlotAxisData &zaxis = this->zaxis(zind);

  CGnuPlotAxis *plotZAxis = getPlotAxis('z', zind);

  if (! plotZAxis->isDisplayed())
    return;

  //---

  CGnuPlotAxis *plotXAxis = getPlotAxis('x', 1);

  double xmin1 = plotXAxis->getStart();
  double xmax1 = plotXAxis->getEnd  ();

  CGnuPlotAxis *plotYAxis = getPlotAxis('y', 1);

  double ymin1 = plotYAxis->getStart();
//double ymax1 = plotYAxis->getEnd  ();

  //---

  if (! plotZAxis->isInitialized()) {
    plotZAxis->setLabel(zaxis.text());

    if (zaxis.logScale().isValid()) {
      plotZAxis->setLogarithmic(true);
      plotZAxis->setLogarithmicBase(zaxis.logScale().getValue());
    }
    else
      plotZAxis->setLogarithmic(false);

    plotZAxis->setDrawLine(false);
    plotZAxis->setDrawTickMark(zaxis.showTics());

    plotZAxis->setTickInside(zaxis.isOutside() ? zind == 1 : zind == 2);
    plotZAxis->setDrawTickLabel(zaxis.showTics());
    plotZAxis->setLabelInside(zind == 2);
    plotZAxis->setDrawLabel(true);

    if (zaxis.isTime())
      plotZAxis->setTimeFormat(zaxis.format());

    //---

    plotZAxis->setEnhanced(zaxis.isEnhanced());
    plotZAxis->setTickInside1(false);
    plotZAxis->setDrawTickLabel1(false);
    plotZAxis->setLabelInside1(false);
    plotZAxis->setDrawLabel1(false);
    plotZAxis->setDrawTickMark1(zaxis.showTics() && zaxis.isMirror());

    //---

    plotZAxis->setGrid(zaxis.hasGrid());
    plotZAxis->setGridMajor(zaxis.hasGridTics());
    plotZAxis->setGridMinor(zaxis.hasGridMinorTics());
    plotZAxis->setGridLayer(axesData_.grid.layer);

    //---

    plotZAxis->setPosition (CPoint3D(xmin1, ymin1, 0));
    plotZAxis->setPosition1(CPoint3D(xmax1, ymin1, 0));

    //---

    plotZAxis->setInitialized(true);
  }

  //---

  renderer->setRange(getDisplayRange(1, 1));
  renderer->setReverse(zaxis.isReverse(), false);

  // draw major (bottom)
  plotZAxis->drawAxis(renderer, true);

  // draw minor (top)
  if (drawOther)
    plotZAxis->drawAxis(renderer, false);
}

// polar (raxis) ?
void
CGnuPlotGroup::
drawAxis(CGnuPlotRenderer *renderer, const CGnuPlotAxisData &axis, char c, int ind)
{
  CGnuPlotAxis *plotAxis = getPlotAxis(c, ind);

  if (! plotAxis->isDisplayed())
    return;

  //---

  double zmin1 = 0.0;

  if (is3D()) {
    CGnuPlotAxis *plotZAxis = getPlotAxis('z', 1);

    zmin1 = plotZAxis->getStart();
  }

  //---

  if (! plotAxis->isInitialized()) {
    plotAxis->setLabel(axis.text());

    plotAxis->setLogarithmic(false);
    plotAxis->setDrawLine(c == 'r');
    plotAxis->setDrawTickMark(axis.showTics());

    plotAxis->setTickInside(! axis.isOutside());
    plotAxis->setDrawTickLabel(axis.showTics());
    plotAxis->setLabelInside(false);
    plotAxis->setDrawLabel(true);

    if (axis.isTime())
      plotAxis->setTimeFormat(axis.format());

    //---

    plotAxis->setEnhanced(axis.isEnhanced());
    plotAxis->setTickInside1(false);
    plotAxis->setDrawTickLabel1(false);
    plotAxis->setLabelInside1(false);
    plotAxis->setDrawLabel1(false);
    plotAxis->setDrawTickMark1(false);

    //---

    plotAxis->setGrid(axis.hasGrid());
    plotAxis->setGridMajor(axis.hasGridTics());
    plotAxis->setGridMinor(axis.hasGridMinorTics());
    plotAxis->setGridLayer(axesData_.grid.layer);

    //---

    CGnuPlotAxis *plotXAxis = getPlotAxis('x', 1);

  //double xmin = plotXAxis->getStart();
    double xmax = plotXAxis->getEnd  ();

    plotAxis->setRange(0, xmax);

    //CGnuPlotAxis *plotYAxis = getPlotAxis('y', 1);

    //double ymin = plotYAxis->getStart();
    //double ymax = plotYAxis->getEnd  ();

    plotAxis->setPosition(CPoint3D(0, 0, zmin1));

    //---

    plotAxis->setInitialized(true);
  }

  //---

  renderer->setRange(getDisplayRange(1, 1));
  renderer->setReverse(axis.isReverse(), false);

  plotAxis->drawAxis(renderer, true);
}

void
CGnuPlotGroup::
drawGrid(CGnuPlotRenderer *renderer, const CGnuPlotTypes::DrawLayer &layer)
{
  bool isBack = (layer == CGnuPlotTypes::DrawLayer::BACK);

  if (! is3D()) {
    CGnuPlotAxis *plotXAxis = getPlotAxis('x', 1);
    CGnuPlotAxis *plotYAxis = getPlotAxis('y', 1);
    CGnuPlotAxis *plotRAxis = getPlotAxis('r', 1);

    if (plotXAxis->hasGrid() && plotXAxis->isGridBackLayer() == isBack) {
      //double ymin1 = plotYAxis->getStart();
      //double ymax1 = plotYAxis->getEnd  ();

      plotXAxis->drawGrid(renderer);
    }

    if (plotYAxis->hasGrid() && plotYAxis->isGridBackLayer() == isBack) {
      //double xmin1 = plotXAxis->getStart();
      //double xmax1 = plotXAxis->getEnd  ();

      plotYAxis->drawGrid(renderer);
    }

    if (plotRAxis->hasGrid() && plotRAxis->isGridBackLayer() == isBack) {
      plotRAxis->drawRadialGrid(renderer);
    }
  }
  else {
    CGnuPlotAxis *plotXAxis = getPlotAxis('x', 1);
    CGnuPlotAxis *plotYAxis = getPlotAxis('y', 1);
    CGnuPlotAxis *plotZAxis = getPlotAxis('z', 1);

    // x/y grid at zmin
    if (plotXAxis->hasGrid() && plotXAxis->isGridBackLayer() == isBack) {
      //double ymin1 = plotYAxis->getStart();
      //double ymax1 = plotYAxis->getEnd  ();
      //double zmin1 = plotZAxis->getStart();

      plotXAxis->drawGrid(renderer);
    }
    if (plotYAxis->hasGrid() && plotYAxis->isGridBackLayer() == isBack) {
      //double xmin1 = plotXAxis->getStart();
      //double xmax1 = plotXAxis->getEnd  ();
      //double zmin1 = plotZAxis->getStart();

      plotYAxis->drawGrid(renderer);
    }

    // z/x grid at ymin
    if (plotZAxis->hasGrid() && plotZAxis->isGridBackLayer() == isBack) {
      //double zmin1 = plotZAxis->getStart();
      //double zmax1 = plotZAxis->getEnd  ();
      //double ymin1 = plotYAxis->getStart();

      plotZAxis->drawGrid(renderer);
    }
    if (plotXAxis->hasGrid() && plotXAxis->isGridBackLayer() == isBack) {
      //double zmin1 = plotZAxis->getStart();
      //double zmax1 = plotZAxis->getEnd  ();
      //double ymin1 = plotYAxis->getStart();

      plotXAxis->drawGrid(renderer);
    }
  }
}

void
CGnuPlotGroup::
drawKey()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CGnuPlotPlot *singlePlot = getSingleStylePlot();

  if (singlePlot) {
    CGnuPlotStyleBase *singleStyle = app()->getPlotStyle(singlePlot->getStyle());

    singleStyle->drawKey(singlePlot, renderer);

    return;
  }

  // TODO: key drawn in own coord system
  CBBox2D bbox = getDisplayRange(1, 1);

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
  colorBox_->draw(renderer);
}

void
CGnuPlotGroup::
drawAnnotations(DrawLayer layer)
{
  // draw labels last
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRange(getDisplayRange(1, 1));
  renderer->setReverse(xaxis(1).isReverse(), yaxis(1).isReverse());

  for (const auto &ann : annotations_)
    if (ann->getLayer() == layer)
      ann->draw(renderer);
}

CGnuPlotAxisData *
CGnuPlotGroup::
getAxisDataFromId(const std::string &id)
{
  const CGnuPlotGroup *group = (const CGnuPlotGroup *) this;

  const CGnuPlotAxisData *data = group->getAxisDataFromId(id);

  return const_cast<CGnuPlotAxisData *>(data);
}

const CGnuPlotAxisData *
CGnuPlotGroup::
getAxisDataFromId(const std::string &id) const
{
  char c;
  int  ind;

  decodeAxisId(id, c, ind);

  const CGnuPlotAxisData *axis = 0;

  if      (c == 'x') axis = &xaxis(ind);
  else if (c == 'y') axis = &yaxis(ind);
  else if (c == 'z') axis = &zaxis(ind);
  else if (c == 'p') axis = &paxis(ind);
  else if (c == 'c') axis = &colorBox()->axis();
  else if (c == 't') axis = &taxis(ind);
  else if (c == 'r') axis = &raxis();
  else if (c == 'u') axis = &uaxis();
  else if (c == 'v') axis = &vaxis();

  return axis;
}

bool
CGnuPlotGroup::
hasTicLabels(const std::string &id) const
{
  const CGnuPlotAxisData *axis = getAxisDataFromId(id);
  if (! axis) return false;

  return axis->hasTicLabels();
}

const CGnuPlotAxisData::RTicLabels &
CGnuPlotGroup::
ticLabels(const std::string &id) const
{
  static CGnuPlotAxisData::RTicLabels noLabels;

  const CGnuPlotAxisData *axis = getAxisDataFromId(id);
  if (! axis) return noLabels;

  return axis->rticLabels();
}

std::string
CGnuPlotGroup::
getAxisValueStr(const std::string &id, int i, double r) const
{
  const CGnuPlotAxisData *axis = getAxisDataFromId(id);
  if (! axis) return CStrUtil::strprintf("%g", r);

  return getAxisValueStr(*axis, i, r);
}

std::string
CGnuPlotGroup::
getAxisValueStr(const CGnuPlotAxisData &axis, int i, double r) const
{
  if (axis.hasTicLabels()) {
    if (axis.hasITicLabel(i))
      return axis.iticLabel(i);
    else
      return "";
  }
  else
    return formatAxisValue(axis, r);
}

std::string
CGnuPlotGroup::
formatAxisValue(const CGnuPlotAxisData &axis, double r) const
{
  if (axis.isTime()) {
    static char buffer[512];

    time_t t(r);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, axis.format().c_str(), tm1);

    return buffer;
  }
  else if (axis.format() != "")
    return CStrUtil::strprintf(axis.format().c_str(), r);
  else
    return "";
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

    plot->calcBoundedYRange(&ymin2, &ymax2);

    ymin1.updateMin(ymin2);
    ymax1.updateMax(ymax2);
  }

  ymin = ymin1.getValue(0);
  ymax = ymax1.getValue(1);
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

    CGnuPlotAxis plotYAxis(const_cast<CGnuPlotGroup *>(this), "y1", CGnuPlotAxis::Direction::Y);

    plotYAxis.setRange(bbox.getYMin(), bbox.getYMax());

    bbox.setYMin(plotYAxis.getStart());
    bbox.setYMax(plotYAxis.getEnd  ());
  }
  else {
    double xmin, ymin, xmax, ymax;

    calcRange(xind, yind, xmin, ymin, xmax, ymax);

    normalizeXRange(xmin, xmax);
    normalizeYRange(xmin, xmax);

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

CPoint3D
CGnuPlotGroup::
mapLogPoint(const CPoint3D &p) const
{
  CPoint3D p1(p);

  mapLogPoint(p1);

  return p1;
}

void
CGnuPlotGroup::
mapLogPoint(CPoint3D &p) const
{
  mapLogPoint(&p.x, &p.y, &p.z);
}

CPoint2D
CGnuPlotGroup::
mapLogPoint(const CPoint2D &p) const
{
  CPoint2D p1(p);

  mapLogPoint(p1);

  return p1;
}

void
CGnuPlotGroup::
mapLogPoint(CPoint2D &p) const
{
  double z;

  mapLogPoint(&p.x, &p.y, &z);
}

void
CGnuPlotGroup::
mapLogPoint(double *x, double *y, double *z) const
{
  int xbase = xaxis(1).logScale().getValue(-1);
  int ybase = yaxis(1).logScale().getValue(-1);
  int zbase = zaxis(1).logScale().getValue(-1);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);
  double zlogscale = (zbase > 1 ? log(zbase) : 1);

  if (xbase > 1) *x = log(std::max(*x, 0.5))/xlogscale;
  if (ybase > 1) *y = log(std::max(*y, 0.5))/ylogscale;
  if (zbase > 1) *z = log(std::max(*z, 0.5))/zlogscale;
}

void
CGnuPlotGroup::
unmapLogPoint(CPoint3D &p) const
{
  unmapLogPoint(&p.x, &p.y, &p.z);
}

void
CGnuPlotGroup::
unmapLogPoint(double *x, double *y, double *z) const
{
  int xbase = xaxis(1).logScale().getValue(-1);
  int ybase = yaxis(1).logScale().getValue(-1);
  int zbase = zaxis(1).logScale().getValue(-1);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);
  double zlogscale = (zbase > 1 ? log(zbase) : 1);

  if (xbase > 1) *x = exp((*x)*xlogscale);
  if (ybase > 1) *y = exp((*y)*ylogscale);
  if (zbase > 1) *x = exp((*z)*zlogscale);
}
