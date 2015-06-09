#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotStyle.h>
#include <CGnuPlotBarObject.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotRectObject.h>
#include <CGnuPlotPolygonObject.h>
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
 barCache_(this), bubbleCache_(this), ellipseCache_(this), pieCache_(this),
 polygonCache_(this), rectCache_(this)
{
  setSmooth (app()->getSmooth());
  setBoxPlot(app()->getBoxPlot());
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
getKeyLabels(std::vector<CGnuPlotKeyLabel> &labels) const
{
  labels.push_back(CGnuPlotKeyLabel(keyTitle_));

  if (is3D() && isContourEnabled()) {
    std::vector<CGnuPlotContour::LevelData> levelDatas;

    contour_.getLevelData(levelDatas);

    for (const auto &l : levelDatas) {
      std::string str = CStrUtil::strprintf("%g", l.level);

      labels.push_back(CGnuPlotKeyLabel(str, l.c));
    }
  }
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

  setMapping(plot->mapping());

  setContourData(plot->contourData());
  setSurfaceData(plot->surfaceData());
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
setContourLevels(int n)
{
  contourData_.setLevels(n);

  contourSet_ = false;
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
  std::vector<double> values = {{x, y}};

  return addPoint2D(values);
}

int
CGnuPlotPlot::
addPoint2D(const std::vector<double> &rvals)
{
  assert(! is3D());

  std::vector<CExprValueP> values;

  for (auto r : rvals)
    values.push_back(CExprInst->createRealValue(r));

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

  CGnuPlotRenderer *renderer = app()->renderer();

  // normal plot (no children)
  if (getStyle() == PlotStyle::TEST_TERMINAL || getStyle() == PlotStyle::TEST_PALETTE) {
    CGnuPlotStyleBase *style = app()->getPlotStyle(getStyle());

    style->draw2D(this, renderer);
  }
  else {
    if (is3D())
      draw3D(renderer);
    else
      draw2D(renderer);
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
updateEllipseCacheSize(int n)
{
  ellipseCache_.updateSize(n);
}

void
CGnuPlotPlot::
updatePieCacheSize(int n)
{
  pieCache_.updateSize(n);
}

void
CGnuPlotPlot::
updatePolygonCacheSize(int n)
{
  polygonCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateRectCacheSize(int n)
{
  rectCache_.updateSize(n);
}

void
CGnuPlotPlot::
initRenderer(CGnuPlotRenderer *renderer)
{
  renderer->setRange(group_->getDisplayRange(xind(), yind()));
  renderer->setReverse(group_->xaxis(xind()).isReverse(), group_->yaxis(yind()).isReverse());
}

void
CGnuPlotPlot::
draw3D(CGnuPlotRenderer *renderer)
{
  assert(is3D());

  if (isContourEnabled())
    drawContour(renderer);

  if (isSurfaceData()) {
    if (isSurfaceEnabled())
      drawSurface(renderer);
  }
  else {
    CGnuPlotStyleBase *style = app()->getPlotStyle(getStyle());

    if (style && style->has3D())
      style->draw3D(this, renderer);
    else {
      if (isSurfaceEnabled())
        drawSurface(renderer);
    }
  }
}

void
CGnuPlotPlot::
drawContour(CGnuPlotRenderer *renderer)
{
  std::pair<int,int> np = numPoints3D();

  //---

  if (! contourSet_) {
    contour_.reset();

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

  contour_.drawContour(renderer);
}

bool
CGnuPlotPlot::
isSurfaceData() const
{
  std::pair<int,int> np = numPoints3D();

  return (np.first >= 2 && np.second >= 2);
}

void
CGnuPlotPlot::
drawSurface(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CRGBA c = lineStyle.calcColor(CRGBA(0,0,0));

  CGnuPlotCamera *camera = group()->camera();

  bool isCalcColor = lineStyle.color().isCalc();

  std::pair<int,int> np = numPoints3D();

  //---

  bool pm3D = (style_ == PlotStyle::PM3D || window()->pm3D().isEnabled());

  if (isSurfaceEnabled() || pm3D) {
    if (! surfaceSet_) {
      surfaceZPolygons_.clear();
      surfaceIJPoints_ .clear();

      surfaceZMin_.setInvalid();
      surfaceZMax_.setInvalid();

      for (int iy = 1; iy < np.second; ++iy) {
        for (int ix = 1; ix < np.first; ++ix) {
          // get surface points
          const CGnuPlotPoint &ppoint1 = getPoint3D(ix - 1, iy - 1);
          const CGnuPlotPoint &ppoint2 = getPoint3D(ix - 1, iy    );
          const CGnuPlotPoint &ppoint3 = getPoint3D(ix    , iy    );
          const CGnuPlotPoint &ppoint4 = getPoint3D(ix    , iy - 1);

          CPoint3D p1, p2, p3, p4;

          ppoint1.getPoint(p1);
          ppoint2.getPoint(p2);
          ppoint3.getPoint(p3);
          ppoint4.getPoint(p4);

          group_->mapLogPoint(p1);
          group_->mapLogPoint(p2);
          group_->mapLogPoint(p3);
          group_->mapLogPoint(p4);

          // average color
          double za;
          CRGBA  ca;

          if (! isCalcColor) {
            za = CGnuPlotUtil::avg({p1.z, p2.z, p3.z, p4.z});

            surfaceZMin_.updateMin(za);
            surfaceZMax_.updateMax(za);
          }
          else {
            double z1 = (ppoint1.getNumValues() > 3 ? p1.z : 0);
            double z2 = (ppoint2.getNumValues() > 3 ? p2.z : 0);
            double z3 = (ppoint3.getNumValues() > 3 ? p3.z : 0);
            double z4 = (ppoint4.getNumValues() > 3 ? p4.z : 0);

            za = CGnuPlotUtil::avg({z1, z2, z3, z4});

            int nc = ppoint1.getNumValues();

            CRGBA rgba1 = lineStyle.color().calcColor(this, ppoint1.getReal(nc));
            CRGBA rgba2 = lineStyle.color().calcColor(this, ppoint2.getReal(nc));
            CRGBA rgba3 = lineStyle.color().calcColor(this, ppoint3.getReal(nc));
            CRGBA rgba4 = lineStyle.color().calcColor(this, ppoint4.getReal(nc));

            ca = CGnuPlotUtil::avgT<CRGBA>({rgba1, rgba2, rgba3, rgba4}, CRGBA(0,0,0,0));
          }

          //---

          CPoint3D pt1 = camera->transform(p1);
          CPoint3D pt2 = camera->transform(p2);
          CPoint3D pt3 = camera->transform(p3);
          CPoint3D pt4 = camera->transform(p4);

          // average z
          double zm = CGnuPlotUtil::avg({pt1.z, pt2.z, pt3.z, pt4.z});

          //---

          // create surface polygon
          Points poly;

          poly.push_back(CPoint2D(pt1.x, pt1.y));
          poly.push_back(CPoint2D(pt2.x, pt2.y));
          poly.push_back(CPoint2D(pt3.x, pt3.y));
          poly.push_back(CPoint2D(pt4.x, pt4.y));

          surfaceZPolygons_[-zm].push_back(ZPoints(za, PointsColor(poly, ca)));

          surfaceIJPoints_[ix - 1][iy - 1] = poly;
        }
      }

      surfaceSet_ = true;
    }
  }

  if (isSurfaceEnabled()) {
    if (window()->hidden3D()) {
      CRGBA fc(1,1,1);

      for (auto polys : surfaceZPolygons_) {
        for (auto poly : polys.second) {
          const PointsColor &pc = poly.second;

          renderer->fillPolygon(pc.first, fc);
          renderer->drawPolygon(pc.first, 0, c);
        }
      }
    }
    else {
      int pattern = hidden3D_.trianglePattern;

      int nx = surfaceIJPoints_.size();

      for (auto ijpoly : surfaceIJPoints_) {
        int ix = ijpoly.first;

        int ny = ijpoly.second.size();

        for (auto ipoly : ijpoly.second) {
          int iy = ipoly.first;

          const Points &points = ipoly.second;

          if (pattern & 1) renderer->drawLine(points[0], points[1], 1, c);
          if (pattern & 2) renderer->drawLine(points[0], points[3], 1, c);
          if (pattern & 4) renderer->drawLine(points[0], points[2], 1, c);

          if (ix == nx - 1 || iy == ny - 1) {
            if (pattern & 2) renderer->drawLine(points[2], points[1], 1, c);

            if (pattern & 1) renderer->drawLine(points[2], points[3], 1, c);
          }
        }
      }
    }
  }

  if (style_ == PlotStyle::PM3D || pm3D) {
    for (auto polys : surfaceZPolygons_) {
      for (auto poly : polys.second) {
        double z = poly.first;

        const PointsColor &pc = poly.second;

        CRGBA rgba;

        if (! isCalcColor) {
          double r = (                      z - surfaceZMin_.getValue())/
                     (surfaceZMax_.getValue() - surfaceZMin_.getValue());

          CColor c = group_->palette()->getColor(r);

          rgba = c.rgba();
        }
        else
          rgba = pc.second;

        renderer->fillPolygon(pc.first, rgba);
      }
    }
  }
}

void
CGnuPlotPlot::
draw2D(CGnuPlotRenderer *renderer)
{
  assert(! is3D());

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
  if (getSmooth() == Smooth::BEZIER) {
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
}

void
CGnuPlotPlot::
drawClusteredHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &drawData)
{
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

    double xl = drawData.x2 + drawData.i*drawData.w + i*drawData.d;

    CBBox2D bbox(xl + drawData.xb, drawData.y2, xl + drawData.w - drawData.xb, y);

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

    double xl = drawData.x2 + drawData.i*drawData.w + i*drawData.d;

    double xlb = xl + drawData.xb;
    double xrb = xl + drawData.w - drawData.xb;

    CBBox2D bbox(xlb, drawData.y2, xrb, y);

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

    double xm = (xlb + + xrb)/2;

    renderer->drawClipLine(CPoint2D(xm , yl), CPoint2D(xm , yh), 1.0, lc);
    renderer->drawClipLine(CPoint2D(xlb, yl), CPoint2D(xrb, yl), 1.0, lc);
    renderer->drawClipLine(CPoint2D(xlb, yh), CPoint2D(xrb, yh), 1.0, lc);

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

CGnuPlotEllipseObject *
CGnuPlotPlot::
createEllipseObject() const
{
  return app()->device()->createEllipseObject(const_cast<CGnuPlotPlot *>(this));
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

CGnuPlotPolygonObject *
CGnuPlotPlot::
createPolygonObject() const
{
  return app()->device()->createPolygonObject(const_cast<CGnuPlotPlot *>(this));
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

        group_->mapLogPoint(p1);

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
    brenderer.setReverse(group_->xaxis(xind()).isReverse(), group_->yaxis(yind()).isReverse());

    style->draw2D(th, &brenderer);

    bbox = brenderer.bbox();

    return true;
  }

  //---

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
  std::vector<double> reals;

  (void) p.getReals(reals);

  if      (mapping() == CGnuPlotTypes::Mapping::SPHERICAL_MAPPING) {
    double theta = (reals.size() > 0 ? reals[0] : 0.0);
    double phi   = (reals.size() > 1 ? reals[1] : 0.0);
    double r     = (reals.size() > 2 ? reals[2] : 1.0);

    double theta1 = app()->angleToRad(theta);
    double phi1   = app()->angleToRad(phi);

    //p1 = app()->sphericalMap(p2);

    double x = r*cos(theta1)*cos(phi1);
    double y = r*sin(theta1)*cos(phi1);
    double z = r*sin(phi1);

    p1 = CPoint3D(x, y, z);
  }
  else if (mapping() == CGnuPlotTypes::Mapping::CYLINDRICAL_MAPPING) {
    double theta = (reals.size() > 0 ? reals[0] : 0.0);
    double z     = (reals.size() > 1 ? reals[1] : 0.0);
    double r     = (reals.size() > 2 ? reals[2] : 1.0);

    double theta1 = app()->angleToRad(theta);

    double x = r*cos(theta1);
    double y = r*sin(theta1);

    p1 = CPoint3D(x, y, z);
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
