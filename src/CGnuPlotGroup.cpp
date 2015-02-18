#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotRenderer.h>

namespace {
  std::string encodeAxisId(char c, int ind) {
    char id[3] = { c, char('0' + ind), 0 };

    return std::string(id, 3);
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
  key_   = CGnuPlotKeyP(app()->createKey(this));
  title_ = app()->createTitle(this);
}

CGnuPlot *
CGnuPlotGroup::
app() const
{
  return window()->app();
}

void
CGnuPlotGroup::
init()
{
  CGnuPlot *plot = this->app();

  setRegion(plot->region());
  setMargin(plot->margin());

  setHistogramStyle(plot->histogramStyle());

  setLogScaleMap(plot->logScaleMap());

  key_->setKeyData(plot->keyData());

  setAxesData(plot->axesData());
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
    CGnuPlotEllipse   *ellipse = 0;
    CGnuPlotLabel     *label   = 0;
    CGnuPlotPolygon   *poly    = 0;
    CGnuPlotRectangle *rect    = 0;

    if      ((arrow = dynamic_cast<CGnuPlotArrow *>(ann.get()))) {
      annotations_.push_back(
        CGnuPlotGroupAnnotationP(device->createArrow(this)->setData(arrow)));
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
  COptReal xmin1, ymin1, xmax1, ymax1, xmin2, ymin2, xmax2, ymax2;

  for (auto plot : plots_) {
    xmin1.updateMin(plot->xaxis(1).min); xmax1.updateMax(plot->xaxis(1).max);
    ymin1.updateMin(plot->yaxis(1).min); ymax1.updateMax(plot->yaxis(1).max);

    xmin2.updateMin(plot->xaxis(2).min); xmax2.updateMax(plot->xaxis(2).max);
    ymin2.updateMin(plot->yaxis(2).min); ymax2.updateMax(plot->yaxis(2).max);
  }

  xaxis(1).min = xmin1; xaxis(1).max = xmax1;
  yaxis(1).min = ymin1; yaxis(1).max = ymax1;
  xaxis(2).min = xmin2; xaxis(2).max = xmax2;
  yaxis(2).min = ymin2; yaxis(2).max = ymax2;

  for (auto plot : plots_)
    plot->setAxesData(axesData_);
}

CBBox2D
CGnuPlotGroup::
getClip(int xind, int yind) const
{
  double xmin = xaxis(xind).min.getValue(0.0);
  double ymin = yaxis(yind).min.getValue(0.0);
  double xmax = xaxis(xind).max.getValue(1.0);
  double ymax = yaxis(yind).max.getValue(1.0);

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
  xaxis(1).displayed = show;

  app()->stateChanged(window(), CGnuPlotTypes::ChangeState::AXIS_DISPLAY);
}

void
CGnuPlotGroup::
showYAxis(bool show)
{
  yaxis(1).displayed = show;

  app()->stateChanged(window(), CGnuPlotTypes::ChangeState::AXIS_DISPLAY);
}

void
CGnuPlotGroup::
draw()
{
  bbox_ = getDisplayRange(1, 1);

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());
  renderer->setMargin(margin().range());

  if (plotSize().ratio.isValid())
    renderer->setRatio(plotSize().ratio.getValue());
  else
    renderer->unsetRatio();

  drawAnnotations(CGnuPlotLayer::BEHIND);
  drawAnnotations(CGnuPlotLayer::BACK);

  //---

  // draw plot

  drawTitle();

  //---

  // root plot with children
  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    Plots hplots;

    getPlotsOfStyle(hplots, PlotStyle::HISTOGRAMS);

    drawHistogram(hplots);
  }
  else {
    for (auto plot : plots_) {
      plot->initRenderer();

      plot->draw();
    }
  }

  //---

  drawAxes();

  //---

  drawKey();

  //---

  // draw front
  drawAnnotations(CGnuPlotLayer::FRONT);
}

CGnuPlotAxis *
CGnuPlotGroup::
getPlotAxis(char c, int ind)
{
  std::string id = encodeAxisId(c, ind);

  Axes::iterator p = axes_.find(id);

  if (p == axes_.end()) {
    CGnuPlotAxis *axis;

    if      (c == 'x' || c == 'y') {
      COrientation dir = (c == 'x' ? CORIENTATION_HORIZONTAL : CORIENTATION_VERTICAL);

      axis = app()->createAxis(this, id, dir);

      double start = 0, end = 1;

      if      (ind == 1 && dir == CORIENTATION_HORIZONTAL) {
        start = xaxis(1).min.getValue(0.0);
        end   = xaxis(1).max.getValue(1.0);
      }
      else if (ind == 1 && dir == CORIENTATION_VERTICAL) {
        start = yaxis(1).min.getValue(0.0);
        end   = yaxis(1).max.getValue(1.0);
      }
      else if (ind == 2 && dir == CORIENTATION_HORIZONTAL) {
        start = xaxis(2).min.getValue(0.0);
        end   = xaxis(2).max.getValue(1.0);
      }
      else if (ind == 2 && dir == CORIENTATION_VERTICAL) {
        start = yaxis(2).min.getValue(0.0);
        end   = yaxis(2).max.getValue(1.0);
      }
      else
        assert(false);

      axis->setRange(start, end);
    }
    else if (c == 'p') {
      COrientation dir = CORIENTATION_VERTICAL;

      axis = app()->createAxis(this, id, dir);

      double start = axesData_.paxis[ind].min.getValue(0.0);
      double end   = axesData_.paxis[ind].max.getValue(1.0);

      axis->setRange(start, end);
    }
    else
      assert(false);

    p = axes_.insert(p, Axes::value_type(id, axis));
  }

  return (*p).second;
}

void
CGnuPlotGroup::
drawTitle()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRange(region());
  renderer->setReverse(false, false);

  title_->draw();
}

void
CGnuPlotGroup::
drawHistogram(const Plots &plots)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  COptReal xmin1, ymin1, xmax1, ymax1;

  for (auto plot : plots) {
    double xmin2, ymin2, xmax2, ymax2;

    plot->calcXRange(&xmin2, &xmax2);
    plot->calcYRange(&ymin2, &ymax2);

    xmin1.updateMin(xmin2); xmax1.updateMax(xmax2);
    ymin1.updateMin(ymin2); ymax1.updateMax(ymax2);
  }

  double xmin = xmin1.getValue(0);
  double ymin = ymin1.getValue(0);
  double xmax = xmax1.getValue(1);
  double ymax = ymax1.getValue(1);

  //---

  uint numPlots = plots.size();

  uint numPoints = 0;

  for (auto plot : plots)
    numPoints = std::max(numPoints, plot->numPoints2D());

  if (numPoints == 0) return;

  //---

  double xf = 1.0;

  if      (getHistogramStyle() == HistogramStyle::CLUSTERED) {
    CBBox2D bbox = getDisplayRange(1, 1);

    renderer->setRange(bbox);
    renderer->setReverse(xaxis(1).reverse, yaxis(1).reverse);

    double y2 = std::max(0.0, bbox.getBottom());

    double w = xf*(xmax - xmin + 1)/(numPoints*numPlots);

    for (auto plot : plots) {
      double d = (plot->ind() - 1 - numPlots/2.0)*w;

      plot->drawClusteredHistogram(y2, d, w);
    }
  }
  else if (getHistogramStyle() == HistogramStyle::ROWSTACKED) {
    double xb = renderer->pixelWidthToWindowWidth(2);

    ymin = 0;
    ymax = 0;

    for (auto plot : plots) {
      double ymin1, ymax1;

      plot->getYRange(&ymin1, &ymax1);

      ymax += ymax1;
    }

    yaxis(1).max.setValue(ymin);
    yaxis(1).max.setValue(ymax);

    //---

    for (auto plot : plots)
      plot->setNumBars(numPoints);

    //---

    double w = (xmax - xmin)/(numPoints - 1);
    double x = xmin - w/2.0;

    for (uint i = 0; i < numPoints; ++i) {
      double h = 0;

      for (auto plot : plots) {
        const CGnuPlotPoint &point = plot->getPoint2D(i);

        double y;

        if (! point.getY(y))
          y = 0.0;

        CBBox2D bbox(x + xb, h, x + w - xb, h + y);

        plot->drawStackedHistogram(i, bbox);

        h += y;
      }

      x += w;
    }
  }
}

void
CGnuPlotGroup::
drawAxes()
{
  if (hasPlotStyle(PlotStyle::BUBBLECHART))
    return;

  drawBorder();

  if (! hasPlotStyle(PlotStyle::PARALLELAXES)) {
    std::set<int> xSet, ySet;

    for (auto plot : plots_) {
      xSet.insert(plot->xind());
      ySet.insert(plot->yind());
    }

    for (const auto &xi : xSet)
      drawXAxes(xi, xi == 1 && xSet.find(2) == xSet.end());

    for (const auto &yi : ySet)
      drawYAxes(yi, yi == 1 && ySet.find(2) == ySet.end());
  }
  else {
    drawXAxes(1, false);
  }
}

void
CGnuPlotGroup::
drawBorder()
{
  if (! axesData().borders)
    return;

  CGnuPlotRenderer *renderer = app()->renderer();

  CBBox2D bbox = getDisplayRange(1, 1);

  renderer->setRange(bbox);
  renderer->setReverse(xaxis(1).reverse, yaxis(1).reverse);

  double xmin1 = bbox.getLeft  (), ymin1 = bbox.getBottom();
  double xmax1 = bbox.getRight (), ymax1 = bbox.getTop   ();

  double bw = axesData().borderWidth;

  if (axesData().borders & (1<<0))
    renderer->drawLine(CPoint2D(xmin1, ymin1), CPoint2D(xmax1, ymin1), bw);

  if (axesData().borders & (1<<1))
    renderer->drawLine(CPoint2D(xmin1, ymax1), CPoint2D(xmax1, ymax1), bw);

  if (axesData().borders & (1<<2))
    renderer->drawLine(CPoint2D(xmin1, ymin1), CPoint2D(xmin1, ymax1), bw);

  if (axesData().borders & (1<<3))
    renderer->drawLine(CPoint2D(xmax1, ymin1), CPoint2D(xmax1, ymax1), bw);
}

void
CGnuPlotGroup::
drawXAxes(int xind, bool drawOther)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  AxisData &xaxis = this->xaxis(xind);

  CGnuPlotAxis *plotXAxis = getPlotAxis('x', xind);
  CGnuPlotAxis *plotYAxis = getPlotAxis('y', 1);

  if (! plotXAxis->isDisplayed())
    return;

  double xmin1 = plotXAxis->getStart();
  double xmax1 = plotXAxis->getEnd  ();
  double ymin1 = plotYAxis->getStart();
  double ymax1 = plotYAxis->getEnd  ();

  if      (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    plotXAxis->setMajorIncrement(1);

    if (getHistogramStyle() == HistogramStyle::CLUSTERED ||
        getHistogramStyle() == HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    if (getHistogramStyle() == HistogramStyle::ROWSTACKED)
      ymin1 = 0;

    plotXAxis->setDrawMinorTickMark(false);
  }
  else if (hasPlotStyle(PlotStyle::PARALLELAXES)) {
    plotXAxis->setMajorIncrement(1);
  }

  renderer->setRange(getDisplayRange(xind, 1));
  renderer->setReverse(xaxis.reverse, false);

  if (xaxis.displayed) {
    plotXAxis->setLabel(xaxis.str);

    if (getLogScale(LogScale::X))
      plotXAxis->setLogarithmic(getLogScale(LogScale::X));
    else
      plotXAxis->resetLogarithmic();

    plotXAxis->setDrawLine(false);
    plotXAxis->setDrawTickMark(xaxis.showTics);

    // draw major (bottom)
    plotXAxis->setTickInside(xind == 1);
    plotXAxis->setDrawTickLabel(true);
    plotXAxis->setLabelInside(xind == 2);
    plotXAxis->setDrawLabel(true);

    if (xaxis.isTime)
      plotXAxis->setTimeFormat(xaxis.format);

    plotXAxis->drawAxis(xind == 2 ? ymax1 : ymin1);

    // draw minor (top)
    if (drawOther) {
      plotXAxis->setTickInside(false);
      plotXAxis->setDrawTickLabel(false);
      plotXAxis->setDrawLabel(false);
      plotXAxis->setDrawTickMark(xaxis.mirror);

      plotXAxis->drawAxis(ymax1);
    }
  }

  if (xaxis.grid)
    plotXAxis->drawGrid(ymin1, ymax1);
}

void
CGnuPlotGroup::
drawYAxes(int yind, bool drawOther)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  AxisData &yaxis = this->yaxis(yind);

  CGnuPlotAxis *plotXAxis = getPlotAxis('x', 1);
  CGnuPlotAxis *plotYAxis = getPlotAxis('y', yind);

  if (! plotYAxis->isDisplayed())
    return;

  double xmin1 = plotXAxis->getStart();
  double xmax1 = plotXAxis->getEnd  ();
//double ymin1 = plotYAxis->getStart();
//double ymax1 = plotYAxis->getEnd  ();

  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    if (getHistogramStyle() == HistogramStyle::CLUSTERED ||
        getHistogramStyle() == HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    //if (getHistogramStyle() == HistogramStyle::ROWSTACKED)
    //  ymin1 = 0;

    plotYAxis->setDrawMinorTickMark(false);
  }

  renderer->setRange(getDisplayRange(1, yind));
  renderer->setReverse(false, yaxis.reverse);

  if (yaxis.displayed) {
    plotYAxis->setLabel(yaxis.str);

    if (getLogScale(LogScale::Y))
      plotYAxis->setLogarithmic(getLogScale(LogScale::Y));
    else
      plotYAxis->resetLogarithmic();

    plotYAxis->setDrawLine(false);
    plotYAxis->setDrawTickMark(yaxis.showTics);

    // draw major (left)
    plotYAxis->setTickInside(yind == 1);
    plotYAxis->setDrawTickLabel(true);
    plotYAxis->setLabelInside(yind == 2);
    plotYAxis->setDrawLabel(true);

    if (yaxis.isTime)
      plotYAxis->setTimeFormat(yaxis.format);

    plotYAxis->drawAxis(yind == 2 ? xmax1 : xmin1);

    // draw right
    if (drawOther) {
      plotYAxis->setTickInside(false);
      plotYAxis->setDrawTickLabel(false);
      plotYAxis->setDrawLabel(false);
      plotYAxis->setDrawTickMark(yaxis.mirror);

      plotYAxis->drawAxis(xmax1);
    }
  }

  if (yaxis.grid)
    plotYAxis->drawGrid(xmin1, xmax1);
}

void
CGnuPlotGroup::
drawKey()
{
  if (hasPlotStyle(PlotStyle::BUBBLECHART))
    return;

  CGnuPlotRenderer *renderer = app()->renderer();

  // TODO: key drawn in own coord system
  CBBox2D bbox = getDisplayRange(1, 1);

  renderer->setRange(bbox);
  renderer->setReverse(false, false);

  key_->draw();
}

void
CGnuPlotGroup::
drawAnnotations(CGnuPlotLayer layer)
{
  // draw labels last
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRange(getDisplayRange(1, 1));
  renderer->setReverse(xaxis(1).reverse, yaxis(1).reverse);

  for (const auto &ann : annotations_)
    if (ann->getLayer() == layer)
      ann->draw();
}

std::string
CGnuPlotGroup::
getAxisValueStr(const std::string &id, int i, double r) const
{
  char c;
  int  ind;

  decodeAxisId(id, c, ind);

  const AxisData *axis = 0;

  if      (c == 'x')
    axis = &xaxis(ind);
  else if (c == 'y')
    axis = &yaxis(ind);
  else if (c == 'p')
    axis = &paxis(ind);

  if (axis->format != "") {
    return CStrUtil::strprintf(axis->format.c_str(), r);
  }
  else {
    auto pl = axis->ticlabel.find(i);

    if (pl != axis->ticlabel.end())
      return (*pl).second;

    return formatAxisValue(*axis, r);
  }
}

std::string
CGnuPlotGroup::
formatAxisValue(const AxisData &axis, double r) const
{
  if (axis.isTime) {
    static char buffer[512];

    time_t t(r);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, axis.format.c_str(), tm1);

    return buffer;
  }
  else
    return CStrUtil::toString(r);
}

void
CGnuPlotGroup::
calcRange(int xind, int yind, double &xmin, double &ymin, double &xmax, double &ymax) const
{
  COptReal xmin1, ymin1, xmax1, ymax1;

  for (auto plot : plots_) {
    double xmin2, ymin2, xmax2, ymax2;

    plot->calcXRange(&xmin2, &xmax2);
    plot->calcYRange(&ymin2, &ymax2);

    if (plot->xind() == xind) { xmin1.updateMin(xmin2); xmax1.updateMax(xmax2); }
    if (plot->yind() == yind) { ymin1.updateMin(ymin2); ymax1.updateMax(ymax2); }
  }

  xmin = xmin1.getValue(0);
  ymin = ymin1.getValue(0);
  xmax = xmax1.getValue(1);
  ymax = ymax1.getValue(1);
}

CBBox2D
CGnuPlotGroup::
getDisplayRange(int xind, int yind) const
{
  double xmin, ymin, xmax, ymax;

  calcRange(xind, yind, xmin, ymin, xmax, ymax);

  CGnuPlotAxis plotXAxis(const_cast<CGnuPlotGroup *>(this), "x1", CORIENTATION_HORIZONTAL);
  CGnuPlotAxis plotYAxis(const_cast<CGnuPlotGroup *>(this), "y1", CORIENTATION_VERTICAL  );

  plotXAxis.setRange(xmin, xmax);
  plotYAxis.setRange(ymin, ymax);

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
  double ymin1 = plotYAxis.getStart();
  double ymax1 = plotYAxis.getEnd  ();

  if (hasPlotStyle(PlotStyle::HISTOGRAMS)) {
    CGnuPlot::HistogramStyle hstyle = getHistogramStyle();

    if (hstyle == CGnuPlot::HistogramStyle::CLUSTERED ||
        hstyle == CGnuPlot::HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    if (hstyle == CGnuPlot::HistogramStyle::ROWSTACKED)
      ymin1 = std::min(0.0, ymin1);
  }

  return CBBox2D(xmin1, ymin1, xmax1, ymax1);
}

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

void
CGnuPlotGroup::
mapLogPoint(CPoint2D &p) const
{
  mapLogPoint(&p.x, &p.y);
}

void
CGnuPlotGroup::
mapLogPoint(double *x, double *y) const
{
  int xbase = getLogScale(LogScale::X);
  int ybase = getLogScale(LogScale::Y);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);

  if (xbase > 1) *x = log(std::max(*x, 0.5))/xlogscale;
  if (ybase > 1) *y = log(std::max(*y, 0.5))/ylogscale;
}

void
CGnuPlotGroup::
unmapLogPoint(double *x, double *y) const
{
  int xbase = getLogScale(LogScale::X);
  int ybase = getLogScale(LogScale::Y);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);

  if (xbase > 1) *x = exp((*x)*xlogscale);
  if (ybase > 1) *y = exp((*y)*ylogscale);
}
