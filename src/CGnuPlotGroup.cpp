#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

int CGnuPlotGroup::nextId_ = 1;

CGnuPlotGroup::
CGnuPlotGroup(CGnuPlotWindow *window) :
 window_(window), id_(nextId_++)
{
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

  setArrows    (plot->arrows    ());
  setLabels    (plot->labels    ());
  setRectangles(plot->rectangles());
  setEllipses  (plot->ellipses  ());
  setPolygons  (plot->polygons  ());
}

void
CGnuPlotGroup::
fit()
{
  COptReal xmin, ymin, xmax, ymax, xmin2, ymin2, xmax2, ymax2;

  for (auto plot : plots_) {
    const AxesData &axesData = plot->axesData();

    xmin .updateMin(axesData.xaxis .min); xmax .updateMax(axesData.xaxis .max);
    ymin .updateMin(axesData.yaxis .min); ymax .updateMax(axesData.yaxis .max);

    xmin2.updateMin(axesData.x2axis.min); xmax2.updateMax(axesData.x2axis.max);
    ymin2.updateMin(axesData.y2axis.min); ymax2.updateMax(axesData.y2axis.max);
  }

  axesData_.xaxis .min = xmin ; axesData_.xaxis .max = xmax ;
  axesData_.yaxis .min = ymin ; axesData_.yaxis .max = ymax ;
  axesData_.x2axis.min = xmin2; axesData_.x2axis.max = xmax2;
  axesData_.y2axis.min = ymin2; axesData_.y2axis.max = ymax2;

  for (auto plot : plots_)
    plot->setAxesData(axesData_);
}

const CGnuPlot::AxesData &
CGnuPlotGroup::
axesData() const
{
  return axesData_;
}

void
CGnuPlotGroup::
setAxesData(const AxesData &a)
{
  axesData_ = a;
}

const CBBox2D &
CGnuPlotGroup::
region() const
{
  return region_;
}

void
CGnuPlotGroup::
setRegion(const CBBox2D &r)
{
  region_ = r;
}

const CRange2D &
CGnuPlotGroup::
margin() const
{
  return margin_;
}

void
CGnuPlotGroup::
setMargin(const CRange2D &m)
{
  margin_ = m;
}

const CGnuPlot::PlotSize &
CGnuPlotGroup::
plotSize() const
{
  return plotSize_;
}

void
CGnuPlotGroup::
setPlotSize(const PlotSize &s)
{
  plotSize_ = s;
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
  axesData_.xaxis.displayed = show;

  app()->stateChanged(window(), CGnuPlotTypes::ChangeState::AXIS_DISPLAY);
}

void
CGnuPlotGroup::
showYAxis(bool show)
{
  axesData_.yaxis.displayed = show;

  app()->stateChanged(window(), CGnuPlotTypes::ChangeState::AXIS_DISPLAY);
}

void
CGnuPlotGroup::
draw()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());
  renderer->setMargin(margin());
  renderer->setRange(getDisplayRange(1, 1));

  if (plotSize().ratio.isValid())
    renderer->setRatio(plotSize().ratio.getValue());
  else
    renderer->unsetRatio();

  //---

  initPlotAxis();

  //---

  // root plot with children
  if (hasHistograms()) {
    Plots hplots;

    getHistograms(hplots);

    drawHistogram(hplots);
  }
  else {
    for (auto plot : plots_) {
      renderer->setRange(getDisplayRange(plot->xind(), plot->yind()));

      plot->draw();
    }
  }

  //---

  drawAxes();

  //---

  drawAnnotations();

  //---

  drawKey();
}

void
CGnuPlotGroup::
initPlotAxis()
{
  double xmin1 = axesData_.xaxis .min.getValue(0.0);
  double xmax1 = axesData_.xaxis .max.getValue(1.0);
  double ymin1 = axesData_.yaxis .min.getValue(0.0);
  double ymax1 = axesData_.yaxis .max.getValue(1.0);

  double xmin2 = axesData_.x2axis.min.getValue(0.0);
  double xmax2 = axesData_.x2axis.max.getValue(1.0);
  double ymin2 = axesData_.y2axis.min.getValue(0.0);
  double ymax2 = axesData_.y2axis.max.getValue(1.0);

  plotXAxis1_ = CGnuPlotAxis(this, 1, CORIENTATION_HORIZONTAL); plotXAxis1_.setRange(xmin1, xmax1);
  plotXAxis2_ = CGnuPlotAxis(this, 2, CORIENTATION_HORIZONTAL); plotXAxis2_.setRange(xmin2, xmax2);

  plotYAxis1_ = CGnuPlotAxis(this, 1, CORIENTATION_VERTICAL  ); plotYAxis1_.setRange(ymin1, ymax1);
  plotYAxis2_ = CGnuPlotAxis(this, 2, CORIENTATION_VERTICAL  ); plotYAxis2_.setRange(ymin2, ymax2);
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

  double xb = pixelWidthToWindowWidth(1);
  double xf = 1.0;

  if      (getHistogramStyle() == HistogramStyle::CLUSTERED) {
    CBBox2D bbox = getDisplayRange(1, 1);

    renderer->setRange(bbox);

  //double y1 = std::min(0.0, ymin);
    double y2 = std::max(0.0, bbox.getBottom());

  //axesData_.yaxis.max.setValue(y1);
  //axesData_.yaxis.max.setValue(ymax);

    double w = xf*(xmax - xmin)/(numPoints*numPlots) - 2*xb;

    for (auto plot : plots) {
      CGnuPlotLineStyleP lineStyle = plot->calcLineStyle();

      CGnuPlotPlot::FillType    fillType    = plot->fillType();
      CGnuPlotPlot::FillPattern fillPattern = plot->fillPattern();

      CRGBA c = (fillType == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

      //---

      double d = (plot->ind() - 1 - numPlots/2.0)*w;

      for (const auto &point : plot->getPoints2D()) {
        CPoint2D p;

        if (point.getPoint(p)) {
          CBBox2D bbox(p.x + d, y2, p.x + d + w, p.y);

          plot->drawBar(p.x, p.y, bbox, fillType, fillPattern, lineStyle->color(c),
                        lineStyle->color(CRGBA(0,0,0)));
        }
      }
    }
  }
  else if (getHistogramStyle() == HistogramStyle::ROWSTACKED) {
    ymin = 0;
    ymax = 0;

    for (auto plot : plots) {
      double ymin1, ymax1;

      plot->getYRange(&ymin1, &ymax1);

      ymax += ymax1;
    }

    axesData_.yaxis.max.setValue(ymin);
    axesData_.yaxis.max.setValue(ymax);

    //---

    double w = (xmax - xmin)/(numPoints - 1);
    double x = xmin - w/2.0;

    for (uint i = 0; i < numPoints; ++i) {
      double h = 0;

      for (auto plot : plots) {
        CGnuPlotLineStyleP lineStyle = plot->calcLineStyle();

        CGnuPlotPlot::FillPattern fillPattern = plot->fillPattern();
        CGnuPlotPlot::FillType    fillType    = plot->fillType();

        //---

        const CGnuPlotPoint &point = plot->getPoint2D(i);

        double y;

        if (! point.getY(y))
          continue;

        CBBox2D bbox(x, h, x + w, h + y);

        CRGBA c = (fillType == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

        plot->drawBar(x, y, bbox, fillType, fillPattern, lineStyle->color(c),
                      lineStyle->color(CRGBA(0,0,0)));

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
  std::set<int> xSet, ySet;

  for (auto plot : plots_) {
    xSet.insert(plot->xind());
    ySet.insert(plot->yind());
  }

  drawBorder();

  for (const auto &xi : xSet)
    drawXAxes(xi, xi == 1 && xSet.find(2) == xSet.end());

  for (const auto &yi : ySet)
    drawYAxes(yi, yi == 1 && ySet.find(2) == ySet.end());
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

  AxisData &xaxis = (xind == 2 ? axesData_.x2axis : axesData_.xaxis);

  CGnuPlotAxis &plotXAxis = (xind == 2 ? plotXAxis2_ : plotXAxis1_);
  CGnuPlotAxis &plotYAxis = plotYAxis1_;

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
  double ymin1 = plotYAxis.getStart();
  double ymax1 = plotYAxis.getEnd  ();

  if (hasHistograms()) {
    plotXAxis.setMajorIncrement(1);

    if (getHistogramStyle() == HistogramStyle::CLUSTERED ||
        getHistogramStyle() == HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    if (getHistogramStyle() == HistogramStyle::ROWSTACKED)
      ymin1 = 0;

    plotXAxis.setDrawMinorTickMark(false);
  }

  renderer->setRange(getDisplayRange(xind, 1));

  if (xaxis.displayed) {
    plotXAxis.setLabel(xaxis.str);

    if (getLogScale(LogScale::X))
      plotXAxis.setLogarithmic(getLogScale(LogScale::X));
    else
      plotXAxis.resetLogarithmic();

    plotXAxis.setFont(renderer->getFont());
    plotXAxis.setDrawLine(false);
    plotXAxis.setDrawTickMark(xaxis.showTics);

    // draw major (bottom)
    plotXAxis.setTickInside(xind == 1);
    plotXAxis.setDrawTickLabel(true);
    plotXAxis.setLabelInside(xind == 2);
    plotXAxis.setDrawLabel(true);

    if (xaxis.isTime)
      plotXAxis.setTimeFormat(xaxis.format);

    plotXAxis.drawAxis(xind == 2 ? ymax1 : ymin1);

    // draw minor (top)
    if (drawOther) {
      plotXAxis.setTickInside(false);
      plotXAxis.setDrawTickLabel(false);
      plotXAxis.setDrawLabel(false);
      plotXAxis.setDrawTickMark(xaxis.mirror);

      plotXAxis.drawAxis(ymax1);
    }
  }

  if (xaxis.grid)
    plotXAxis.drawGrid(ymin1, ymax1);
}

void
CGnuPlotGroup::
drawYAxes(int yind, bool drawOther)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  AxisData &yaxis = (yind == 2 ? axesData_.y2axis : axesData_.yaxis);

  CGnuPlotAxis &plotXAxis = plotXAxis1_;
  CGnuPlotAxis &plotYAxis = (yind == 2 ? plotYAxis2_ : plotYAxis1_);

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
//double ymin1 = plotYAxis.getStart();
//double ymax1 = plotYAxis.getEnd  ();

  if (hasHistograms()) {
    if (getHistogramStyle() == HistogramStyle::CLUSTERED ||
        getHistogramStyle() == HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    //if (getHistogramStyle() == HistogramStyle::ROWSTACKED)
    //  ymin1 = 0;

    plotYAxis.setDrawMinorTickMark(false);
  }

  renderer->setRange(getDisplayRange(1, yind));

  if (yaxis.displayed) {
    plotYAxis.setLabel(yaxis.str);

    if (getLogScale(LogScale::Y))
      plotYAxis.setLogarithmic(getLogScale(LogScale::Y));
    else
      plotYAxis.resetLogarithmic();

    plotYAxis.setFont(renderer->getFont());
    plotYAxis.setDrawLine(false);
    plotYAxis.setDrawTickMark(yaxis.showTics);

    // draw major (left)
    plotYAxis.setTickInside(yind == 1);
    plotYAxis.setDrawTickLabel(true);
    plotYAxis.setLabelInside(yind == 2);
    plotYAxis.setDrawLabel(true);

    if (yaxis.isTime)
      plotYAxis.setTimeFormat(yaxis.format);

    plotYAxis.drawAxis(yind == 2 ? xmax1 : xmin1);

    // draw right
    if (drawOther) {
      plotYAxis.setTickInside(false);
      plotYAxis.setDrawTickLabel(false);
      plotYAxis.setDrawLabel(false);
      plotYAxis.setDrawTickMark(yaxis.mirror);

      plotYAxis.drawAxis(xmax1);
    }
  }

  if (yaxis.grid)
    plotYAxis.drawGrid(xmin1, xmax1);
}

void
CGnuPlotGroup::
drawKey()
{
  if (! getKeyDisplayed()) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  CBBox2D bbox = getDisplayRange(1, 1);

  renderer->setRange(bbox);

  //---

  CBBox2D rbbox = (keyData().outside ? getRegionBBox() : bbox);

  //---

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = pixelWidthToWindowWidth  (1);
  double ph = pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = 64*pw;

  CSize2D size;

  double textWidth = 0.0, textHeight = 0.0;

  for (auto plot : plots_) {
    std::string label = plot->keyTitle();

    textWidth = std::max(textWidth, font->getStringWidth(label)*pw);

    textHeight += font_size*ph;
  }

  size = CSize2D(textWidth + ll + 3*bx, textHeight + 2*by);

  CHAlignType halign = getKeyHAlign();
  CVAlignType valign = getKeyVAlign();

  double x1 = 0, y1 = 0;

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = rbbox.getLeft () + bx;
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = rbbox.getRight() - bx - size.width;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = rbbox.getXMid() - size.width/2;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = rbbox.getTop   () - by - size.height;
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = rbbox.getBottom() + by;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = rbbox.getYMid() - size.height/2;

  double x2 = x1 + size.width;
  double y2 = y1 + size.height;

  if (keyData().box) {
    CBBox2D bbox1(x1, y1, x2, y2);

    renderer->drawRect(bbox1);
  }

  double y = y2 - by;

  for (auto plot : plots_) {
    PlotStyle          style     = plot->getStyle();
    CGnuPlotLineStyleP lineStyle = plot->calcLineStyle();

    double xx = (keyData().reverse ? x1 + bx : x2 - ll - bx);
    double yy = y - font_size*ph/2;

    if (hasHistograms() || style == PlotStyle::BOXES || style == PlotStyle::CANDLESTICKS) {
      double h = (font_size - 4)*ph;

      CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

      if      (plot->fillStyle().style() == CGnuPlotPlot::FillType::PATTERN)
        renderer->patternRect(hbbox, plot->fillStyle().pattern(),
                               lineStyle->color(CRGBA(0,0,0)));
      else if (plot->fillStyle().style() == CGnuPlotPlot::FillType::SOLID)
        renderer->fillRect(hbbox, lineStyle->color(CRGBA(1,1,1)));

      renderer->drawRect(hbbox, lineStyle->color(CRGBA(0,0,0)));
    }
    else {
      if (style == PlotStyle::LINES || style == PlotStyle::LINES_POINTS)
        renderer->drawLine(CPoint2D(xx, yy), CPoint2D(xx + ll, yy),
                           lineStyle->width(), lineStyle->color(CRGBA(1,0,0)));

      if (style == PlotStyle::POINTS || style == PlotStyle::LINES_POINTS)
        renderer->drawSymbol(CPoint2D(xx + ll/2, yy), plot->pointType(),
                              plot->pointSize(), lineStyle->color(CRGBA(1,0,0)));

      if (style == PlotStyle::FILLEDCURVES) {
        double h = 4*ph;

        CBBox2D cbbox(xx, yy - h, xx + ll, yy + h);
        renderer->fillRect(cbbox, lineStyle->color(CRGBA(1,1,1)));
      }
    }

    std::string label = plot->keyTitle();

    //double lw = font->getStringWidth(label);

    if (keyData().reverse)
      renderer->drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                                 CVALIGN_TYPE_TOP, 0, label);
    else
      renderer->drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                                 CVALIGN_TYPE_TOP, 0, label);

    y -= font_size*ph;
  }
}

void
CGnuPlotGroup::
drawAnnotations()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  for (auto arrow : arrows_)
    arrow->draw(renderer);

  for (auto label : labels_)
    label->draw(renderer);

  for (auto rect : rects_)
    rect->draw(renderer);

  for (auto ellipse : ellipses_)
    ellipse->draw(renderer);

  for (auto poly : polygons_)
    poly->draw(renderer);
}

std::string
CGnuPlotGroup::
getXAxisValueStr(int ind, int i, double x) const
{
  const AxisData &xaxis = (ind == 2 ? axesData().x2axis : axesData().xaxis);

  auto pl = xaxis.ticlabel.find(i);

  if (pl != xaxis.ticlabel.end())
    return (*pl).second;

  return formatX(x);
}

std::string
CGnuPlotGroup::
getYAxisValueStr(int ind, int i, double y) const
{
  const AxisData &yaxis = (ind == 2 ? axesData().y2axis : axesData().yaxis);

  auto pl = yaxis.ticlabel.find(i);

  if (pl != yaxis.ticlabel.end())
    return (*pl).second;

  return formatY(y);
}

std::string
CGnuPlotGroup::
formatX(double x) const
{
  if (axesData().xaxis.isTime) {
    static char buffer[512];

    time_t t(x);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, axesData().xaxis.format.c_str(), tm1);

    return buffer;
  }
  else
    return CStrUtil::toString(x);
}

std::string
CGnuPlotGroup::
formatY(double y) const
{
  if (axesData().yaxis.isTime) {
    static char buffer[512];

    time_t t(y);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, axesData().yaxis.format.c_str(), tm1);

    return buffer;
  }
  else
    return CStrUtil::toString(y);
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

  CGnuPlotAxis plotXAxis(const_cast<CGnuPlotGroup *>(this), CORIENTATION_HORIZONTAL);
  CGnuPlotAxis plotYAxis(const_cast<CGnuPlotGroup *>(this), CORIENTATION_VERTICAL);

  plotXAxis.setRange(xmin, xmax);
  plotYAxis.setRange(ymin, ymax);

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
  double ymin1 = plotYAxis.getStart();
  double ymax1 = plotYAxis.getEnd  ();

  if (hasHistograms()) {
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
hasHistograms() const
{
  for (auto plot : plots_) {
    if (plot->getStyle() == PlotStyle::HISTOGRAMS)
      return true;
  }

  return false;
}

void
CGnuPlotGroup::
getHistograms(Plots &plots) const
{
  for (auto plot : plots_) {
    if (plot->getStyle() == PlotStyle::HISTOGRAMS)
      plots.push_back(plot);
  }
}

CBBox2D
CGnuPlotGroup::
getRegionBBox() const
{
  const CISize2D &s = window()->size();

  double x1 = region().getXMin()*(s.width  - 1);
  double y1 = region().getYMin()*(s.height - 1);
  double x2 = region().getXMax()*(s.width  - 1);
  double y2 = region().getXMax()*(s.height - 1);

  CPoint2D p1 = pixelToWindow(CPoint2D(x1, y1));
  CPoint2D p2 = pixelToWindow(CPoint2D(x2, y2));

  return CBBox2D(p1, p2);
}

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
