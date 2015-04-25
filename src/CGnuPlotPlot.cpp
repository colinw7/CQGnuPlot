#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotStyle.h>
#include <CGnuPlotBarObject.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotRectObject.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotStyleBase.h>
#include <CGnuPlotUtil.h>

#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>

int CGnuPlotPlot::nextId_ = 1;

CGnuPlotPlot::
CGnuPlotPlot(CGnuPlotGroup *group, PlotStyle style) :
 group_(group), style_(style), id_(nextId_++), contour_(this),
 barCache_(this), bubbleCache_(this), pieCache_(this), rectCache_(this)
{
  setSmooth(app()->getSmooth());
}

CGnuPlotPlot::
~CGnuPlotPlot()
{
}

CGnuPlot *
CGnuPlotPlot::
app() const
{
  return window()->app();
}

CGnuPlotWindow *
CGnuPlotPlot::
window() const
{
  return group()->window();
}

bool
CGnuPlotPlot::
is3D() const
{
  return group_->is3D();
}

void
CGnuPlotPlot::
init()
{
  CGnuPlot *plot = this->app();

  setBars    (plot->bars());
  setBoxWidth(plot->boxWidth());

  setFillStyle (plot->fillStyle ());
  setPointStyle(plot->pointStyle());
  setArrowStyle(plot->arrowStyle());

  xind_ = plot->xind();
  yind_ = plot->yind();

  setFilledCurve(plot->filledCurve());
  setHidden3D(plot->hidden3D());
  setWhiskerBars(plot->whiskerBars());

  setBinary(plot->isBinary());
  setMatrix(plot->isMatrix());
}

void
CGnuPlotPlot::
setStyle(PlotStyle style)
{
  style_ = style;

  bymin_.setInvalid();
  bymax_.setInvalid();
}
void
CGnuPlotPlot::
clearPoints()
{
  if (app()->isDebug())
    std::cerr << "Clear Points" << std::endl;

  points2D_.clear();
  points3D_.clear();

  reset3D();
}

int
CGnuPlotPlot::
addPoint2D(double x, double y)
{
  assert(! is3D());

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));

  return addPoint2D(values, false);
}

int
CGnuPlotPlot::
addPoint2D(double x, CExprValueP y)
{
  assert(! is3D());

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(y);

  return addPoint2D(values, false);
}

int
CGnuPlotPlot::
addPoint2D(const Values &values, bool discontinuity, const Params &params)
{
  assert(! is3D());

  if (app()->isDebug()) {
    std::cerr << "Add Point [" << points2D_.size() << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points2D_.push_back(CGnuPlotPoint(values, discontinuity, params));

  return int(points2D_.size()) - 1;
}

int
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, double z)
{
  assert(is3D());

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));
  values.push_back(CExprInst->createRealValue(z));

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, CExprValueP z)
{
  assert(is3D());

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));
  values.push_back(z);

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, const Values &values, bool discontinuity)
{
  assert(is3D());

  if (app()->isDebug()) {
    std::cerr << "Add Point [" << points3D_.size() << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points3D_[iy].push_back(CGnuPlotPoint(values, discontinuity));

  reset3D();

  return int(points3D_[iy].size()) - 1;
}

void
CGnuPlotPlot::
reset3D()
{
  contourSet_ = false;
  surfaceSet_ = false;
}

void
CGnuPlotPlot::
updateGroupRange()
{
  group_->fit();
}

void
CGnuPlotPlot::
smooth()
{
  if (! is3D()) {
    if (smooth_ == Smooth::UNIQUE) {
      typedef std::vector<double>     Values;
      typedef std::map<double,Values> Points;

      Points points;

      for (uint i = 0; i < numPoints2D(); ++i) {
        double x, y;

        points2D_[i].getX(x);
        points2D_[i].getY(y);

        points[x].push_back(y);
      }

      clearPoints();

      for (const auto &p : points) {
        double x = p.first;
        double y = 0.0;

        for (const auto &p1 : p.second)
          y += p1;

        y /= p.second.size();

        (void) addPoint2D(x, y);
      }
    }
  }
}

void
CGnuPlotPlot::
draw()
{
  if (! isDisplayed()) return;

  //---

  const CGnuPlotAxisData &xaxis = group_->xaxis(xind_);
  const CGnuPlotAxisData &yaxis = group_->yaxis(yind_);

  double xmin = xaxis.min().getValue(0.0);
  double ymin = yaxis.min().getValue(0.0);
  double xmax = xaxis.max().getValue(1.0);
  double ymax = yaxis.max().getValue(1.0);

  bbox_ = CBBox2D(xmin, ymin, xmax, ymax);

  //---

  // normal plot (no children)
  if      (getStyle() == PlotStyle::TEST_TERMINAL)
    drawTerminal();
  else if (getStyle() == PlotStyle::TEST_PALETTE)
    drawPalette();
  else {
    if (is3D())
      draw3D();
    else
      draw2D();
  }
}


void
CGnuPlotPlot::
updateBarCacheSize(int n)
{
  barCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateBubbleCacheSize(int n)
{
  bubbleCache_.updateSize(n);
}

void
CGnuPlotPlot::
updatePieCacheSize(int n)
{
  pieCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateRectCacheSize(int n)
{
  rectCache_.updateSize(n);
}

void
CGnuPlotPlot::
initRenderer()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRange(group_->getDisplayRange(xind(), yind()));
  renderer->setReverse(group_->xaxis(xind()).isReverse(), group_->yaxis(yind()).isReverse());
}

void
CGnuPlotPlot::
draw3D()
{
  assert(is3D());

  CGnuPlotRenderer *renderer = app()->renderer();

  if      (style_ == PlotStyle::POINTS)
    drawPoints(renderer);
  else if (style_ == PlotStyle::LINES)
    drawLines(renderer);
  else
    drawSurface(renderer);
}

void
CGnuPlotPlot::
drawSurface(CGnuPlotRenderer *renderer)
{
  CGnuPlotCamera *camera = group()->camera();

  std::pair<int,int> np = numPoints3D();

  //---

  if (window()->contour3D()) {
    if (! contourSet_) {
      std::vector<double> xc, yc, zc;

      xc.resize(np.first);
      yc.resize(np.second);
      zc.resize(np.first*np.second);

      for (int iy = 0; iy < np.second; ++iy) {
        for (int ix = 0; ix < np.first; ++ix) {
          const CGnuPlotPoint &point = getPoint3D(ix, iy);

          double x, y;

          point.getX(x); point.getY(y);

          xc[ix] = x;
          yc[iy] = y;

          zc[iy*np.first + ix] = point.getZ();
        }
      }

      contour_.setData(&xc[0], &yc[0], &zc[0], np.first, np.second);

      contourSet_ = true;
    }

    contour_.drawContour();
  }

  //---

  if (window()->surface3D() || window()->pm3D().isEnabled()) {
    if (! surfaceSet_) {
      surface_.clear();

      surfaceZMin_.setInvalid();
      surfaceZMax_.setInvalid();

      for (int iy = 1; iy < np.second; ++iy) {
        for (int ix = 1; ix < np.first; ++ix) {
          const CGnuPlotPoint &point1 = getPoint3D(ix - 1, iy - 1);
          const CGnuPlotPoint &point2 = getPoint3D(ix - 1, iy    );
          const CGnuPlotPoint &point3 = getPoint3D(ix    , iy    );
          const CGnuPlotPoint &point4 = getPoint3D(ix    , iy - 1);

          double z1 = CGnuPlotUtil::avg({point1.getZ(), point2.getZ(),
                                         point3.getZ(), point4.getZ()});

          surfaceZMin_.updateMin(z1);
          surfaceZMax_.updateMax(z1);

          CPoint3D p1(0,0,0), p2(0,0,0), p3(0,0,0), p4(0,0,0);

          double x, y, z;

          if (point1.getX(x) && point1.getY(y) && point1.getZ(z))
            p1 = camera->transform(CPoint3D(x, y, z));
          if (point2.getX(x) && point2.getY(y) && point2.getZ(z))
            p2 = camera->transform(CPoint3D(x, y, z));
          if (point3.getX(x) && point3.getY(y) && point3.getZ(z))
            p3 = camera->transform(CPoint3D(x, y, z));
          if (point4.getX(x) && point4.getY(y) && point4.getZ(z))
            p4 = camera->transform(CPoint3D(x, y, z));

          double zm = CGnuPlotUtil::avg({p1.z, p2.z, p3.z, p4.z});

          std::vector<CPoint2D> poly;

          poly.push_back(CPoint2D(p1.x, p1.y));
          poly.push_back(CPoint2D(p2.x, p2.y));
          poly.push_back(CPoint2D(p3.x, p3.y));
          poly.push_back(CPoint2D(p4.x, p4.y));

          surface_[-zm].push_back(ZPoints(z1, poly));
        }
      }

      surfaceSet_ = true;
    }
  }

  if (window()->surface3D()) {
    if (window()->hidden3D()) {
      for (auto polys : surface_) {
        for (auto poly : polys.second) {
          renderer->fillPolygon(poly.second, CRGBA(0.8,0.8,0.8));
          renderer->drawPolygon(poly.second, 0, CRGBA(0,0,0));
        }
      }
    }
    else {
      int pattern = hidden3D_.trianglePattern;

      for (auto polys : surface_) {
        for (auto poly : polys.second) {
          if (pattern & 1) renderer->drawLine(poly.second[0], poly.second[1], 1, CRGBA(0,0,0));
          if (pattern & 2) renderer->drawLine(poly.second[0], poly.second[3], 1, CRGBA(0,0,0));
          if (pattern & 4) renderer->drawLine(poly.second[0], poly.second[2], 1, CRGBA(0,0,0));
        }
      }
    }
  }

  if (window()->pm3D().isEnabled()) {
    for (auto polys : surface_) {
      for (auto poly : polys.second) {
        double z = poly.first;

        double r = (                      z - surfaceZMin_.getValue())/
                   (surfaceZMax_.getValue() - surfaceZMin_.getValue());

        CRGBA c = group_->palette()->getColor(r);

        renderer->fillPolygon(poly.second, c);
      }
    }
  }
}

void
CGnuPlotPlot::
draw2D()
{
  assert(! is3D());

  CGnuPlotRenderer *renderer = app()->renderer();

  //---

  renderer->setClip(bbox_);

  //---

  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);

  if (style) {
    style->draw2D(this, renderer);

    return;
  }

  //---

  // TODO: move inside line draw code ?
  if      (getSmooth() == Smooth::BEZIER) {
    const CGnuPlotLineStyle &lineStyle = this->lineStyle();

    const CRGBA &c = lineStyle.calcColor(CRGBA(1,0,0));

    uint np = numPoints2D();

    for (uint i1 = 0, i2 = 1, i3 = 2, i4 = 3; i4 < np;
           i1 = i4, i2 = i1 + 1, i3 = i2 + 1, i4 = i3 + 1) {
      const CGnuPlotPoint &point1 = getPoint2D(i1);
      const CGnuPlotPoint &point2 = getPoint2D(i2);
      const CGnuPlotPoint &point3 = getPoint2D(i3);
      const CGnuPlotPoint &point4 = getPoint2D(i4);

      double x1, y1, x2, y2, x3, y3, x4, y4;

      if (! point1.getX(x1) || ! point1.getY(y1) || ! point2.getX(x2) || ! point2.getY(y2) ||
          ! point3.getX(x3) || ! point3.getY(y3) || ! point4.getX(x4) || ! point4.getY(y4))
        continue;

      renderer->drawBezier(CPoint2D(x1, y1), CPoint2D(x2, y2),
                           CPoint2D(x3, y3), CPoint2D(x4, y4),
                           lineStyle.width(), c);
    }
  }
  else if (style_ == PlotStyle::ERRORBARS)
    drawErrorBars(renderer);
  else if (style_ == PlotStyle::DOTS)
    drawDots(renderer);
  else if (style_ == PlotStyle::HISTEPS || style_ == PlotStyle::STEPS ||
           style_ == PlotStyle::FSTEPS  || style_ == PlotStyle::FILLSTEPS)
    drawSteps(renderer);
  else if (style_ == PlotStyle::LINES)
    drawLines(renderer);
  else if (style_ == PlotStyle::LINES_POINTS) {
    drawLines(renderer);

    drawPoints(renderer);
  }
  else if (style_ == PlotStyle::POINTS)
    drawPoints(renderer);
  else if (style_ == PlotStyle::XERRORBARS)
    drawXErrorBars(renderer);
  else if (style_ == PlotStyle::XYERRORBARS)
    drawXYErrorBars(renderer);
  else if (style_ == PlotStyle::YERRORBARS)
    drawYErrorBars(renderer);
  else if (style_ == PlotStyle::XERRORLINES)
    drawXErrorLines(renderer);
  else if (style_ == PlotStyle::XYERRORLINES)
    drawXYErrorLines(renderer);
  else if (style_ == PlotStyle::YERRORLINES)
    drawYErrorLines(renderer);
}

void
CGnuPlotPlot::
drawDots(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  const CRGBA &c = lineStyle.calcColor(CRGBA(1,0,0));

  for (const auto &point : getPoints2D()) {
    CPoint2D p;

    if (point.getPoint(p))
      renderer->drawPoint(p, c);
  }
}

void
CGnuPlotPlot::
drawClusteredHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &drawData)
{
  double xb = 0;

  if (! renderer->isPseudo())
    xb = renderer->pixelWidthToWindowWidth(2);

  CRGBA c = (fillType() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  CRGBA lc = lineStyle().calcColor(c);
  CRGBA fc = lineStyle().calcColor(CRGBA(0,0,0));

  //---

  if (! renderer->isPseudo())
    updateBarCacheSize(getPoints2D().size());

  int i = 0;

  for (const auto &point : getPoints2D()) {
    double y;

    if (! point.getY(y))
      y = 0.0;

    double x = drawData.x2 + i*drawData.d;

    CBBox2D bbox(x + xb, drawData.y2, x + drawData.w - xb, y);

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = barObjects()[i];

      bar->setBBox (bbox);
      bar->setValue(y);

      if (! bar->isInitialized()) {
        bar->setFillType   (fillType());
        bar->setFillPattern(fillPattern());
        bar->setFillColor  (fc);

        bar->setBorder   (true);
        bar->setLineColor(lc);

        bar->setInitialized(true);
      }
    }
    else
      renderer->drawRect(bbox, lc, 1);

    ++i;
  }

  if (! renderer->isPseudo())
    drawBars(renderer);
}

void
CGnuPlotPlot::
drawErrorBarsHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &drawData)
{
  double xb = 0;

  if (! renderer->isPseudo())
    xb = renderer->pixelWidthToWindowWidth(2);

  CRGBA c = (fillType() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  CRGBA lc = lineStyle().calcColor(c);
  CRGBA fc = lineStyle().calcColor(CRGBA(0,0,0));

  //---

  if (! renderer->isPseudo())
    updateBarCacheSize(getPoints2D().size());

  int i = 0;

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double y = reals[0];
    double yl, yh;

    if (reals.size() == 2) {
      yl = y - reals[1];
      yh = y - reals[1];
    }
    else {
      yl = reals[1];
      yh = reals[2];
    }

    double x = drawData.x2 + i*drawData.d;

    double xl = x + xb;
    double xh = x + drawData.w - xb;

    CBBox2D bbox(xl, drawData.y2, xh, y);

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = barObjects()[i];

      bar->setBBox (bbox);
      bar->setValue(y);

      if (! bar->isInitialized()) {
        bar->setFillType   (fillType());
        bar->setFillPattern(fillPattern());
        bar->setFillColor  (fc);

        bar->setBorder   (true);
        bar->setLineColor(lc);

        bar->setInitialized(true);
      }
    }
    else
      renderer->drawRect(bbox, lc, 1);

    double xm = x + drawData.w/2;

    renderer->drawClipLine(CPoint2D(xm, yl), CPoint2D(xm, yh), 1.0, lc);
    renderer->drawClipLine(CPoint2D(xl, yl), CPoint2D(xh, yl), 1.0, lc);
    renderer->drawClipLine(CPoint2D(xl, yh), CPoint2D(xh, yh), 1.0, lc);

    ++i;
  }

  if (! renderer->isPseudo())
    drawBars(renderer);
}

void
CGnuPlotPlot::
drawStackedHistogram(CGnuPlotRenderer *renderer, int i, const CBBox2D &bbox)
{
  const CGnuPlotPoint &point = getPoint2D(i);

  double y;

  if (! point.getY(y))
    return;

  CRGBA c = (fillType() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  CRGBA lc = lineStyle().calcColor(c);
  CRGBA fc = lineStyle().calcColor(CRGBA(0,0,0));

  //---

  if (! renderer->isPseudo()) {
    CGnuPlotBarObject *bar = barObjects()[i];

    bar->setBBox (bbox);
    bar->setValue(y);

    if (! bar->isInitialized()) {
      bar->setFillType   (fillType());
      bar->setFillPattern(fillPattern());
      bar->setFillColor  (fc);

      bar->setBorder   (true);
      bar->setLineColor(lc);

      bar->setInitialized(true);
    }
  }
  else
    renderer->drawRect(bbox, lc, 1);
}

void
CGnuPlotPlot::
drawBars(CGnuPlotRenderer *renderer)
{
  for (const auto &bar : barObjects())
    bar->draw(renderer);
}

void
CGnuPlotPlot::
drawSteps(CGnuPlotRenderer *renderer)
{
  double ymin = bbox_.getYMin();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  const CRGBA &c = lineStyle.calcColor(CRGBA(1,0,0));

  uint np = numPoints2D();

  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (point1.getPoint(p1) && point2.getPoint(p2)) {
      if      (style_ == PlotStyle::HISTEPS) {
        double xm = CGnuPlotUtil::avg({p1.x, p2.x});

        CPoint2D p3(xm, p1.y);
        CPoint2D p4(xm, p2.y);

        renderer->drawClipLine(p1, p3, lineStyle.width(), c);
        renderer->drawClipLine(p3, p4, lineStyle.width(), c);
        renderer->drawClipLine(p4, p2, lineStyle.width(), c);
      }
      else if (style_ == PlotStyle::STEPS) {
        CPoint2D p3(p2.x, p1.y);

        renderer->drawClipLine(p1, p3, lineStyle.width(), c);
        renderer->drawClipLine(p3, p2, lineStyle.width(), c);
      }
      else if (style_ == PlotStyle::FSTEPS) {
        CPoint2D p3(p1.x, p2.y);

        renderer->drawClipLine(p1, p3, lineStyle.width(), c);
        renderer->drawClipLine(p3, p2, lineStyle.width(), c);
      }
      else if (style_ == PlotStyle::FILLSTEPS) {
        CBBox2D bbox(p1.x, ymin, p2.x, p1.y);

        renderer->fillRect(bbox, c);
      }
    }
  }

  if (style_ != PlotStyle::FILLSTEPS) {
    for (const auto &point : getPoints2D()) {
      CPoint2D p;

      if (point.getPoint(p))
        renderer->drawSymbol(p, SymbolType::FILLED_BOX, 1.0, c);
    }
  }
}

void
CGnuPlotPlot::
drawErrorBars(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CRGBA c = lineStyle.calcColor(CRGBA(1,0,0));

  typedef std::map<std::string,int> IndexMap;

  IndexMap indexMap;

  double pw = 0, ph = 0;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (4);
    ph = renderer->pixelHeightToWindowHeight(4);
  }

  for (const auto &point : getPoints2D()) {
    double x = 0;

    if (! point.getValue(1, x)) {
      std::string str;

      if (point.getValue(1, str)) {
        auto p = indexMap.find(str);

        if (p == indexMap.end()) {
          int ind = 0;

          auto p1 = indexMap.rbegin();

          if (p1 != indexMap.rend())
            ind = (*p1).second;

          p = indexMap.insert(p, IndexMap::value_type(str, ind));
        }

        x = (*p).second;
      }
    }

    double y = 0;

    (void) point.getValue(2, y);

    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    // x y xlow xhigh ylow yhigh
    if      (point.getNumValues() >= 6) {
      (void) point.getValue(3, xl);
      (void) point.getValue(4, xh);
      (void) point.getValue(5, yl);
      (void) point.getValue(6, yh);

      if (point.getNumValues() >= 6) {
        double x;

        if (point.getValue(7, x))
          c = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta ydelta
    else if (point.getNumValues() >= 4) {
      double dx = 0;
      double dy = 0;

      (void) point.getValue(3, dx);
      (void) point.getValue(4, dy);

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (point.getNumValues() >= 4) {
        double x;

        if (point.getValue(5, x))
          c = lineStyle.color().calcColor(this, x);
      }
    }
    // x y delta
    else if (point.getNumValues() >= 3) {
      double dx = 0;

      (void) point.getValue(3, dx);

      double dy = dx;

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;
    }

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, c);
    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, c);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, c);
    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, c);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, c);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, c);
  }
}

void
CGnuPlotPlot::
drawXYErrorBars(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double pw = 0, ph = 0;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (4);
    ph = renderer->pixelHeightToWindowHeight(4);
  }

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    CRGBA lc1 = lc;

    // x y xlow xhigh ylow yhigh
    if      ((! isCalcColor && reals.size() == 6) || (isCalcColor && reals.size() == 7)) {
      xl = reals[2];
      xh = reals[3];
      yl = reals[4];
      yh = reals[5];

      if (isCalcColor) {
        double x = reals[6];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta ydelta
    else if ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      double dx = reals[2];
      double dy = reals[3];

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double x = reals[4];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, lc1);
  }
}

void
CGnuPlotPlot::
drawXErrorBars(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double ph = 0;

  if (! renderer->isPseudo())
    ph = renderer->pixelHeightToWindowHeight(4);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;

    CRGBA lc1 = lc;

    // x y xlow xhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      xl = reals[2];
      xh = reals[3];

      if (isCalcColor) {
        double x = reals[4];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dx = reals[2];

      xl = x - dx;
      xh = x + dx;

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }

    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xh, y - ph), CPoint2D(xh, y + ph), 1.0, lc1);
  }
}

void
CGnuPlotPlot::
drawYErrorBars(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double pw = 0;

  if (! renderer->isPseudo())
    pw = renderer->pixelWidthToWindowWidth(4);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double yl = y;
    double yh = y;

    CRGBA lc1 = lc;

    // x y ylow yhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      yl = reals[2];
      yh = reals[3];

      if (isCalcColor) {
        double x = reals[4];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dy = reals[2];

      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, lc1);
  }
}

void
CGnuPlotPlot::
drawXYErrorLines(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double pw = 0, ph = 0;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (4);
    ph = renderer->pixelHeightToWindowHeight(4);
  }

  int      i = 0;
  CPoint2D p1(0,0);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    CRGBA lc1 = lc;

    // x y xlow xhigh ylow yhigh
    if      ((! isCalcColor && reals.size() == 6) || (isCalcColor && reals.size() == 7)) {
      xl = reals[2];
      xh = reals[3];
      yl = reals[4];
      yh = reals[5];

      if (isCalcColor) {
        double x = reals[6];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta ydelta
    else if ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      double dx = reals[2];
      double dy = reals[3];

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double x = reals[4];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }

    CPoint2D p2 = CPoint2D(x, y);

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, lc1);

    if (i > 0)
      renderer->drawLine(p1, p2, 1, lc1);

    p1 = p2;

    ++i;
  }
}

void
CGnuPlotPlot::
drawXErrorLines(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double ph = 0;

  if (! renderer->isPseudo())
    ph = renderer->pixelHeightToWindowHeight(4);

  int      i = 0;
  CPoint2D p1(0,0);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;

    CRGBA lc1 = lc;

    // x y xlow xhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      xl = reals[2];
      xh = reals[3];

      if (isCalcColor) {
        double x = reals[4];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dx = reals[2];

      xl = x - dx;
      xh = x + dx;

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }

    CPoint2D p2 = CPoint2D(x, y);

    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xh, y - ph), CPoint2D(xh, y + ph), 1.0, lc1);

    if (i > 0)
      renderer->drawLine(p1, p2, 1, lc1);

    p1 = p2;

    ++i;
  }
}

void
CGnuPlotPlot::
drawYErrorLines(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double pw = 0;

  if (! renderer->isPseudo())
    pw = renderer->pixelWidthToWindowWidth(4);

  int      i = 0;
  CPoint2D p1(0,0);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double yl = y;
    double yh = y;

    CRGBA lc1 = lc;

    // x y ylow yhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      yl = reals[2];
      yh = reals[3];

      if (isCalcColor) {
        double x = reals[4];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }
    // x y xdelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dy = reals[2];

      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(this, x);
      }
    }

    CPoint2D p2 = CPoint2D(x, y);

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, lc1);

    if (i > 0)
      renderer->drawLine(p1, p2, 1, lc1);

    p1 = p2;

    ++i;
  }
}

void
CGnuPlotPlot::
drawLines(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  if (is3D()) {
    for (const auto &ip : getPoints3D()) {
      uint np = ip.second.size();

      CPoint3D p1;

      uint i = 0;

      typedef std::vector<CPoint3D> Points;

      while (i < np) {
        // find first point
        for ( ; i < np; ++i) {
          const CGnuPlotPoint &point1 = ip.second[i];

          if (mapPoint3D(point1, p1))
            break;
        }

        ++i;

        Points points;

        points.push_back(p1);

        // get next continuous points
        for ( ; i < np; ++i) {
          CPoint3D p2;

          const CGnuPlotPoint &point2 = ip.second[i];

          if (! mapPoint3D(point2, p2) || point2.isDiscontinuity())
            break;

          points.push_back(p2);

          p1 = p2;
        }

        // TODO: clip
        renderer->drawPath(points, lineStyle.width(),
                           lineStyle.calcColor(CRGBA(1,0,0)), lineStyle.dash());
      }
    }
  }
  else {
    uint np = numPoints2D();

    CPoint2D p1;

    uint i = 0;

    typedef std::vector<CPoint2D> Points;

    while (i < np) {
      for ( ; i < np; ++i) {
        const CGnuPlotPoint &point1 = getPoint2D(i);

        if (point1.getPoint(p1))
          break;
      }

      ++i;

      Points points;

      points.push_back(p1);

      for ( ; i < np; ++i) {
        CPoint2D p2;

        const CGnuPlotPoint &point2 = getPoint2D(i);

        if (! point2.getPoint(p2) || point2.isDiscontinuity())
          break;

        points.push_back(p2);

        p1 = p2;
      }

      // TODO: clip
      renderer->drawPath(points, lineStyle.width(),
                         lineStyle.calcColor(CRGBA(1,0,0)), lineStyle.dash());
    }

#if 0
    for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
      const CGnuPlotPoint &point1 = getPoint2D(i1);
      const CGnuPlotPoint &point2 = getPoint2D(i2);

      CPoint2D p1, p2;

      if (! point1.isDiscontinuity() && point1.getPoint(p1) && point2.getPoint(p2))
        renderer->drawClipLine(p1, p2, lineStyle.width(), lineStyle.calcColor(CRGBA(1,0,0)),
                               lineStyle.dash());
    }
#endif
  }
}

void
CGnuPlotPlot::
drawPoints(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c = lineStyle.calcColor(CRGBA(1,0,0));

  double size = pointSize();

  uint pointNum = 0;

  if (is3D()) {
    for (const auto &ip : getPoints3D()) {
      for (const auto &point : ip.second) {
        std::vector<double> reals;

        (void) point.getReals(reals);

        CPoint3D p(0, 0, 0);

        uint valueNum = 0;

        if      (reals.size() == 2) {
          p.x = reals[valueNum++];
          p.y = reals[valueNum++];
          p.z = pointNum;
        }
        else if (reals.size() >= 3) {
          p.x = reals[valueNum++];
          p.y = reals[valueNum++];
          p.z = reals[valueNum++];
        }

        renderer->drawSymbol(p, pointType(), size, c);
      }
    }
  }
  else {
    for (const auto &point : getPoints2D()) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      CPoint2D p(0, 0);

      uint valueNum = 0;

      if      (reals.size() == 1) {
        p.x = pointNum;
        p.y = reals[valueNum++];
      }
      else if (reals.size() >= 2) {
        p.x = reals[valueNum++];
        p.y = reals[valueNum++];
      }

      group()->mapLogPoint(p);

      double size1 = size;

      if ((! isCalcColor && valueNum < reals.size()) ||
          (  isCalcColor && valueNum < reals.size() - 1))
        size1 = reals[valueNum++];

      CRGBA c1 = c;

      if (isCalcColor && valueNum < reals.size()) {
        double x = reals[valueNum++];

        c1 = lineStyle.color().calcColor(this, x);
      }

      renderer->drawSymbol(p, pointType(), size1, c1);
    }
  }
}

double
CGnuPlotPlot::
getXSpacing() const
{
  COptReal dx;

  uint np = numPoints2D();

  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (point1.getPoint(p1) && point2.getPoint(p2)) {
      double dx1 = std::abs(p2.x - p1.x);

      dx.updateMin(dx1);
    }
  }

  return dx.getValue(1);
}

void
CGnuPlotPlot::
setLineStyleId(int ind)
{
  CGnuPlotLineStyleP ls = app()->lineStyle(ind);

  if (ls.get())
    setLineStyle(*ls);
}

void
CGnuPlotPlot::
drawTerminal()
{
  static double grid_dashes[]   = {1, 3};
  static uint   num_grid_dashes = 2;

  //---

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(CBBox2D(0, 0, 1, 1));

  double px1, py1, px2, py2;

  renderer->windowToPixel(0.0, 0.0, &px1, &py1);
  renderer->windowToPixel(1.0, 1.0, &px2, &py2);

  double pxm = CGnuPlotUtil::avg({px1, px2});
  double pym = CGnuPlotUtil::avg({py1, py2});

  double pw = px2 - px1;
  double ph = py1 - py2;

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  //---

  CLineDash lineDash(grid_dashes, num_grid_dashes);

  renderer->drawLine(CPoint2D(0.5, 0.0), CPoint2D(0.5, 1.0), 1, CRGBA(0,0,0), lineDash);
  renderer->drawLine(CPoint2D(0.0, 0.5), CPoint2D(1.0, 0.5), 1, CRGBA(0,0,0), lineDash);

  //---

  {
  CPoint2D p1;

  renderer->pixelToWindow(CPoint2D(px1 + 4, py2 + 4), p1);

  renderer->drawHAlignedText(p1, CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_TOP, 0,
                             "terminal test", CRGBA(1,0,0));
  }

  //---

  // symbols on right
  int    nlines = int(ph/font_size);
  double dy     = 1.0/nlines;

  for (int i = 0; i < nlines; ++i) {
    std::string str = CStrUtil::toString(i - 1);

    double w = font->getStringWidth(str);

    CRGBA c = CGnuPlotStyleInst->indexColor(i - 1);

    double x, y;

    renderer->pixelToWindow(px2 - 48 - w, py2 + font->getCharAscent(), &x, &y);

    renderer->drawText(CPoint2D(x, y - i*dy), str, c);

    double x1, y1, x2, y2;

    renderer->pixelToWindow(px2 - 48, py2 + font->getCharAscent()/2, &x1, &y1);
    renderer->pixelToWindow(px2 - 24, py2 + font->getCharAscent()/2, &x2, &y2);

    renderer->drawLine(CPoint2D(x1, y1 - i*dy), CPoint2D(x2, y1 - i*dy), 0, c);

    renderer->pixelToWindow(px2 - 16, py2 + font->getCharAscent()/2, &x1, &y1);

    renderer->drawSymbol(CPoint2D(x1, y1 - i*dy), CGnuPlotStyleInst->indexSymbol(i - 1), 1.0, c);
  }

  //---

  // text in center
  {
  CPoint2D p1, p2, p3;

  renderer->pixelToWindow(CPoint2D(pxm, pym - 5*font_size), p1);
  renderer->pixelToWindow(CPoint2D(pxm, pym - 4*font_size), p2);
  renderer->pixelToWindow(CPoint2D(pxm, pym - 3*font_size), p3);

  renderer->drawHAlignedText(p1, CHALIGN_TYPE_LEFT  , 0, CVALIGN_TYPE_CENTER, 0,
                             "left justified", CRGBA(0,0,0));
  renderer->drawHAlignedText(p2, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "centre+d text", CRGBA(0,0,0));
  renderer->drawHAlignedText(p3, CHALIGN_TYPE_RIGHT , 0, CVALIGN_TYPE_CENTER, 0,
                             "right justified", CRGBA(0,0,0));

  CPoint2D p4;

  renderer->pixelToWindow(CPoint2D(pxm, pym - font_size), p4);

  renderer->drawHAlignedText(p4, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "test of character width", CGnuPlotStyleInst->indexColor(4));
  renderer->drawHAlignedText(CPoint2D(0.5, 0.5),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "12345678901234567890", CGnuPlotStyleInst->indexColor(4));

  double w = font->getStringWidth("12345678901234567890");

  double x1, y1, x2, y2;

  renderer->pixelToWindow(pxm - w/2, pym + font_size/2, &x1, &y1);
  renderer->pixelToWindow(pxm + w/2, pym - font_size/2, &x2, &y2);

  renderer->drawRect(CBBox2D(x1, y1, x2, y2), CGnuPlotStyleInst->indexColor(4), 1);
  }

  //---

  {
  // rotated text on left
  CPoint2D p1, p2, p3;

  renderer->pixelToWindow(CPoint2D(px1 + 1*font_size, pym), p1);
  renderer->pixelToWindow(CPoint2D(px1 + 2*font_size, pym), p2);
  renderer->pixelToWindow(CPoint2D(px1 + 3*font_size, pym), p3);

  renderer->drawVAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "rotated ce+ntered text", CRGBA(0,1,0));

  renderer->drawRotatedText(p2, "rotated by +45 deg",  45,
                            CHALIGN_TYPE_LEFT, CVALIGN_TYPE_BOTTOM, CRGBA(0,1,0));
  renderer->drawRotatedText(p3, "rotated by -45 deg", -45,
                            CHALIGN_TYPE_LEFT, CVALIGN_TYPE_BOTTOM, CRGBA(0,1,0));
  }

  //---

  // filled polygons
  {
  double pl = pw/16;

  CPoint2D pp1 = CPoint2D(pxm + pw/4, pym - ph/4);
  CPoint2D pp2 = pp1 + CPoint2D(pl, pl);

  CPoint2D p1;

  renderer->pixelToWindow(pp1 - CPoint2D(0, pl), p1);

  renderer->drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0,
                             "filled polygons:", CRGBA(0,0,0));

  std::vector<CPoint2D> points1, points2;

  for (int i = 0; i < 6; ++i) {
    double a = CAngle::Deg2Rad(i*60);

    CPoint2D p1, p2;

    renderer->pixelToWindow(pp1 + CPoint2D(pl*cos(a), pl*sin(a)), p1);
    renderer->pixelToWindow(pp2 + CPoint2D(pl*cos(a), pl*sin(a)), p2);

    points1.push_back(p1);
    points2.push_back(p2);
  }

  renderer->fillPolygon(points1, CRGBA(0,0,1));
  renderer->fillPolygon(points2, CRGBA(0,1,0,0.5));
  }

  //---

  // arrows
  {
  CPoint2D ac;
  double   al = 50;

  renderer->pixelToWindow(CPoint2D(pxm - 100, pym + 100), ac);

  for (int i = 0; i < 8; ++i) {
    CGnuPlotArrow arrow(group());

    double a = i*CAngle::Deg2Rad(45);

    double dx = al*cos(a);
    double dy = -al*sin(a);

    CPoint2D ac1;

    renderer->pixelToWindow(CPoint2D(pxm - 100 + dx, pym + 100 + dy), ac1);

    arrow.setFrom(ac);
    arrow.setTo  (ac1);

    arrow.setHeadLength(renderer->pixelWidthToWindowWidth(al/5));

    arrow.setHeadAngle(30);

    arrow.setFHead(i == 7);
    arrow.setTHead(i != 3 && i != 7);

    arrow.setHeadFilled(i == 2);
    arrow.setHeadEmpty (i == 1 || i == 4 || i == 5 || i == 6 || i == 7);

    arrow.setStrokeColor(CRGBA(1,0,0));

    arrow.draw(renderer);
  }
  }

  //---

  {
  // pattern fill
  double ptw = pw/30;
  double pth = ph/8;
  double ptb = 4;

  for (int i = 0; i <= 9; ++i) {
    double px = pxm + i*(ptw + ptb);

    CPoint2D p1, p2;

    renderer->pixelToWindow(CPoint2D(px      , py1 - pth), p1);
    renderer->pixelToWindow(CPoint2D(px + ptw, py1      ), p2);

    renderer->patternRect(CBBox2D(p1, p2), CGnuPlotStyleInst->indexPattern(i),
                           CRGBA(0,0,0), CRGBA(1,1,1));

    renderer->drawRect(CBBox2D(p1, p2), CRGBA(0,0,0), 1);

    CPoint2D p3;

    renderer->pixelToWindow(CPoint2D(px + ptw/2, py1 - pth), p3);

    renderer->drawHAlignedText(p3, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0,
                               CStrUtil::strprintf("%d", i), CRGBA(0,0,0));
  }

  CPoint2D p4;

  renderer->pixelToWindow(CPoint2D(pxm + 4*(ptw + ptb), py1 - pth - font_size), p4);

  renderer->drawHAlignedText(p4, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0,
                             "pattern fill", CRGBA(0,0,0));
  }

  //---

  {
  // line width bottom left
  double ll  = pw/8;
  double lb1 = 8;
  double lb2 = 8;

  for (int i = 1; i <= 6; ++i) {
    CPoint2D p1, p2;

    renderer->pixelToWindow(CPoint2D(px1 + lb1     , py1 - i*font_size), p1);
    renderer->pixelToWindow(CPoint2D(px1 + lb1 + ll, py1 - i*font_size), p2);

    renderer->drawLine(p1, p2, i, CRGBA(0,0,0));

    CPoint2D p3;

    renderer->pixelToWindow(CPoint2D(px1 + lb1 + lb2 + ll, py1 - i*font_size), p3);

    renderer->drawHAlignedText(p3, CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_CENTER, 0,
                               CStrUtil::strprintf("lw %d", i), CRGBA(0,0,0));
  }

  CPoint2D p4;

  renderer->pixelToWindow(CPoint2D(px1 + lb1 + ll/2, py1 - 7*font_size - 4), p4);

  renderer->drawHAlignedText(p4, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP, 0,
                             "linewidth", CRGBA(0,0,0));
  }
}

void
CGnuPlotPlot::
drawPalette()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(CBBox2D(0, 0, 1, 1));

  double px1, py1, px2, py2;

  renderer->windowToPixel(0.0, 0.0, &px1, &py1);
  renderer->windowToPixel(1.0, 1.0, &px2, &py2);

  double wx1, wy1, wx2, wy2;

  renderer->pixelToWindow(0, py1 + 32, &wx1, &wy1);
  renderer->pixelToWindow(0, py1 + 64, &wx2, &wy2);

  bool   first = true;
  double r1, g1, b1, m1, x1;

  for (double i = px1; i <= px2; i += 1.0) {
    double wx, wy;

    renderer->pixelToWindow(i, 0, &wx, &wy);

    CRGBA c = group_->palette()->getColor(wx);

    renderer->drawLine(CPoint2D(wx, wy1), CPoint2D(wx, wy2), 0.0, c);

    //double x = (i - px1)/(px2 - px1);

    double x2 = wx;
    double r2 = c.getRed  ();
    double g2 = c.getGreen();
    double b2 = c.getBlue ();
    double m2 = c.getGray();

    if (! first) {
      renderer->drawLine(CPoint2D(x1, r1), CPoint2D(x2, r2), 0, CRGBA(1,0,0));
      renderer->drawLine(CPoint2D(x1, g1), CPoint2D(x2, g2), 0, CRGBA(0,1,0));
      renderer->drawLine(CPoint2D(x1, b1), CPoint2D(x2, b2), 0, CRGBA(0,0,1));
      renderer->drawLine(CPoint2D(x1, m1), CPoint2D(x2, m2), 0, CRGBA(0,0,0));
    }

    x1 = x2;
    r1 = r2;
    g1 = g2;
    b1 = b2;
    m1 = m2;

    first = false;
  }

  renderer->drawRect(CBBox2D(0.0, wy1, 1.0, wy2), CRGBA(0,0,0), 1);

#if 0
  CGnuPlotAxis *xaxis = group()->getPlotAxis('x', 1);
  CGnuPlotAxis *yaxis = group()->getPlotAxis('y', 1);

  xaxis->drawAxis(0.0);
  yaxis->drawAxis(0.0);

  xaxis->drawGrid(0.0, 1.0);
  yaxis->drawGrid(0.0, 1.0);
#endif
}

CGnuPlotBarObject *
CGnuPlotPlot::
createBarObject() const
{
  return app()->device()->createBarObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotBubbleObject *
CGnuPlotPlot::
createBubbleObject() const
{
  return app()->device()->createBubbleObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotPieObject *
CGnuPlotPlot::
createPieObject() const
{
  return app()->device()->createPieObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotRectObject *
CGnuPlotPlot::
createRectObject() const
{
  return app()->device()->createRectObject(const_cast<CGnuPlotPlot *>(this));
}

void
CGnuPlotPlot::
calcXRange(double *xmin, double *xmax) const
{
  // calc range for all x values
  if (! xmin_.isValid() || ! xmax_.isValid()) {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    if (is3D()) {
      for (const auto &ip : getPoints3D()) {
        for (const auto &p : ip.second) {
          CPoint3D p1;

          if (! mapPoint3D(p, p1))
            continue;

          th->xmin_.updateMin(p1.x);
          th->xmax_.updateMax(p1.x);
        }
      }
    }
    else {
      CBBox2D bbox;

      if (renderBBox(bbox)) {
        th->xmin_.updateMin(bbox.getLeft ());
        th->xmax_.updateMax(bbox.getRight());
      }
      else {
        for (const auto &p : getPoints2D()) {
          double x;

          if (! p.getX(x))
            continue;

          th->xmin_.updateMin(x);
          th->xmax_.updateMax(x);
        }
      }
    }
  }

  *xmin = xmin_.getValue(-10);
  *xmax = xmax_.getValue( 10);
}

void
CGnuPlotPlot::
calcYRange(double *ymin, double *ymax) const
{
  // calc range for all y values
  if (! ymin_.isValid() || ! ymax_.isValid()) {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    if (is3D()) {
      for (const auto &ip : getPoints3D()) {
        for (const auto &p : ip.second) {
          CPoint3D p1;

          if (! mapPoint3D(p, p1))
            continue;

          th->ymin_.updateMin(p1.y);
          th->ymax_.updateMax(p1.y);
        }
      }
    }
    else {
      CBBox2D bbox;

      if (renderBBox(bbox)) {
        th->ymin_.updateMin(bbox.getBottom());
        th->ymax_.updateMax(bbox.getTop   ());
      }
      else {
        for (const auto &p : getPoints2D()) {
          double y;

          if (! p.getY(y))
            continue;

          th->ymin_.updateMin(y);
          th->ymax_.updateMax(y);
        }
      }
    }
  }

  *ymin = ymin_.getValue(-10);
  *ymax = ymax_.getValue( 10);
}

void
CGnuPlotPlot::
calcZRange(double *zmin, double *zmax) const
{
  assert(is3D());

  // calc range for all z values
  if (! zmin_.isValid() || ! zmax_.isValid()) {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    for (const auto &ip : getPoints3D()) {
      for (const auto &p : ip.second) {
        CPoint3D p1;

        if (! mapPoint3D(p, p1))
          continue;

        th->zmin_.updateMin(p1.z);
        th->zmax_.updateMax(p1.z);
      }
    }
  }

  *zmin = zmin_.getValue(-10);
  *zmax = zmax_.getValue( 10);
}

void
CGnuPlotPlot::
calcBoundedYRange(double *ymin, double *ymax) const
{
  // TODO: use dumy renderer which updates bbox

  // get y range constrained by parent group x range
  if (! bymin_.isValid() || ! bymax_.isValid()) {
    double xmin, xmax;

    getGroupXRange(&xmin, &xmax);

    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    CBBox2D bbox;

    if (is3D()) {
      for (const auto &ip : getPoints3D()) {
        for (const auto &p : ip.second) {
          CPoint3D p1;

          if (! mapPoint3D(p, p1))
            continue;

          if (p1.x < xmin || p1.x > xmax)
            continue;

          th->bymin_.updateMin(p1.y);
          th->bymax_.updateMax(p1.y);
        }
      }
    }
    else {
      if (renderBBox(bbox)) {
        th->bymin_.updateMin(bbox.getBottom());
        th->bymax_.updateMax(bbox.getTop   ());
      }
      else {
        for (const auto &p : getPoints2D()) {
          double x, y;

          if (! p.getXY(x, y))
            continue;

          if (x < xmin || x > xmax)
            continue;

          th->bymin_.updateMin(y);
          th->bymax_.updateMax(y);
        }
      }
    }
  }

  *ymin = bymin_.getValue(-10);
  *ymax = bymax_.getValue( 10);
}

bool
CGnuPlotPlot::
renderBBox(CBBox2D &bbox) const
{
  CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

  CGnuPlotBBoxRenderer brenderer(app()->renderer());

  //---

  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);

  if (style) {
    style->draw2D(th, &brenderer);

    bbox = brenderer.bbox();

    return true;
  }

  //---

  if      (style_ == PlotStyle::ERRORBARS)
    th->drawErrorBars(&brenderer);
  else if (style_ == PlotStyle::XERRORBARS)
    th->drawXErrorBars(&brenderer);
  else if (style_ == PlotStyle::XYERRORBARS)
    th->drawXYErrorBars(&brenderer);
  else if (style_ == PlotStyle::YERRORBARS)
    th->drawYErrorBars(&brenderer);
  else if (style_ == PlotStyle::XERRORLINES)
    th->drawXErrorLines(&brenderer);
  else if (style_ == PlotStyle::XYERRORLINES)
    th->drawXYErrorLines(&brenderer);
  else if (style_ == PlotStyle::YERRORLINES)
    th->drawYErrorLines(&brenderer);
  else
    return false;

  bbox = brenderer.bbox();

  return true;
}

void
CGnuPlotPlot::
recalcBoundedYRange(double *ymin, double *ymax) const
{
  CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

  th->bymin_.setInvalid();
  th->bymax_.setInvalid();

  calcBoundedYRange(ymin, ymax);
}

void
CGnuPlotPlot::
getGroupXRange(double *xmin, double *xmax) const
{
  calcXRange(xmin, xmax);

  CGnuPlotAxisData &xaxis = group_->xaxis(xind_);

  if (xaxis.min().isValid())
    *xmin = xaxis.min().getValue();
  if (xaxis.max().isValid())
    *xmax = xaxis.max().getValue();

  //CGnuPlotAxisData &yaxis = group_->yaxis(yind_);

  //if (yaxis.min().isValid()) ymin_ = yaxis.min().getValue();
  //if (yaxis.max().isValid()) ymax_ = yaxis.max().getValue();
}

void
CGnuPlotPlot::
getPointsRange(CBBox2D &bbox) const
{
  COptReal xmin, ymin, xmax, ymax;

  if (is3D()) {
    for (const auto &ip : getPoints3D()) {
      for (const auto &p : ip.second) {
        CPoint3D p1;

        if (! mapPoint3D(p, p1))
          continue;

        xmin.updateMin(p1.x); ymin.updateMin(p1.y);
        xmax.updateMax(p1.x); ymax.updateMax(p1.y);
      }
    }
  }
  else {
    for (const auto &p : getPoints2D()) {
      double x, y;

      if (! p.getXY(x, y))
        continue;

      xmin.updateMin(x); ymin.updateMin(y);
      xmax.updateMax(x); ymax.updateMax(y);
    }
  }

  bbox = CBBox2D(xmin.getValue(-10), ymin.getValue(-10),
                 xmax.getValue( 10), ymax.getValue( 10));
}

bool
CGnuPlotPlot::
mapPoint3D(const CGnuPlotPoint &p, CPoint3D &p1) const
{
  if (app()->mapping() == CGnuPlotTypes::Mapping::SPHERICAL_MAPPING) {
    CPoint2D p2;

    if (! p.getPoint(p2))
      return false;

    p1 = app()->sphericalMap(p2);
  }
  else {
    if (! p.getPoint(p1))
      return false;
  }

  return true;
}

void
CGnuPlotPlot::
setStyleValue(const std::string &name, StyleValue *value)
{
  auto p = styleValues_.find(name);

  if (p == styleValues_.end())
    delete (*p).second;

  styleValues_[name] = value;
}

CGnuPlotPlot::StyleValue *
CGnuPlotPlot::
styleValue(const std::string &name) const
{
  auto p = styleValues_.find(name);

  if (p == styleValues_.end())
    return 0;

  return (*p).second;
}
