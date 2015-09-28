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
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotRectObject.h>
#include <CGnuPlotPointObject.h>
#include <CGnuPlotArrowObject.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotStyleBase.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotStyleAdjacencyRenderer.h>

#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>

int CGnuPlotPlot::nextId_ = 1;

CGnuPlotPlot::
CGnuPlotPlot(CGnuPlotGroup *group, PlotStyle style) :
 group_(group), style_(style), id_(nextId_++), lineStyle_(app()),
 contour_(this), barCache_(this), bubbleCache_(this), ellipseCache_(this), pieCache_(this),
 polygonCache_(this), rectCache_(this), pointCache_(this), arrowCache_(this),
 tableFile_(group->app())
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
getKeyLabels(CGnuPlotPlot::KeyLabels &labels) const
{
  labels.push_back(CGnuPlotKeyLabel(keyTitleString()));

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

  setEnhanced(plot->device()->isEnhanced());

  setTableFile(plot->tableFile());

  setNewHistogramId(plot->newHistogramId());
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

std::pair<int,int>
CGnuPlotPlot::
numPoints3D() const
{
  assert(is3D());

  if (points3D_.empty()) return std::pair<int,int>(0,0);

  return std::pair<int,int>(points3D_.begin()->second.size(), points3D_.size());
}

const CGnuPlotPoint &
CGnuPlotPlot::
getPoint3D(int ix, int iy) const
{
  assert(is3D());

  auto p = points3D_.find(iy);

  if (p == points3D_.end())
    assert(false);

  assert(ix >= 0 && ix < int((*p).second.size()));

  return (*p).second[ix];
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

  std::vector<CExprValuePtr> values;

  for (auto r : rvals)
    values.push_back(CExprInst->createRealValue(r));

  return addPoint2D(values, false);
}

int
CGnuPlotPlot::
addPoint2D(double x, CExprValuePtr y)
{
  assert(! is3D());

  std::vector<CExprValuePtr> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(y);

  return addPoint2D(values, false);
}

int
CGnuPlotPlot::
addPoint2D(const Values &values, bool discontinuity, bool bad, const Params &params)
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

  points2D_.push_back(CGnuPlotPoint(values, discontinuity, bad, params));

  return int(points2D_.size()) - 1;
}

int
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, double z)
{
  assert(is3D());

  std::vector<CExprValuePtr> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));
  values.push_back(CExprInst->createRealValue(z));

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, CExprValuePtr z)
{
  assert(is3D());

  std::vector<CExprValuePtr> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));
  values.push_back(z);

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, const Values &values, bool discontinuity, bool bad)
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

  points3D_[iy].push_back(CGnuPlotPoint(values, discontinuity, bad));

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
  if (smooth_ == Smooth::NONE)
    return;

  if (! is3D()) {
    if      (smooth_ == Smooth::UNIQUE) {
      // sorted points (no duplicates)
      MappedPoints points;

      mapPoints(points);

      clearPoints();

      for (const auto &p : points) {
        double x = p.first;
        double y = 0.0;

        for (const auto &y1 : p.second)
          y += y1;

        y /= p.second.size();

        (void) addPoint2D(x, y);
      }
    }
    else if (smooth_ == Smooth::FREQUENCY) {
      // sorted points (summed y)
      MappedPoints points;

      mapPoints(points);

      clearPoints();

      for (const auto &p : points) {
        double x = p.first;
        double y = 0.0;

        for (const auto &y1 : p.second)
          y += y1;

        (void) addPoint2D(x, y);
      }
    }
    else {
      group_->app()->errorMsg("Unsupported smooth");
    }
  }
}

void
CGnuPlotPlot::
mapPoints(MappedPoints &points)
{
  for (uint i = 0; i < numPoints2D(); ++i) {
    double x, y;

    points2D_[i].getX(x);
    points2D_[i].getY(y);

    points[x].push_back(y);
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

  CPoint2D p1 = group_->mapLogPoint(xind(), yind(), 1, CPoint2D(xmin, ymin));
  CPoint2D p2 = group_->mapLogPoint(xind(), yind(), 1, CPoint2D(xmax, ymax));

  bbox_ = CBBox2D(p1, p2);

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
printValues() const
{
  assert(tableFile_.isEnabled());

  if (! is3D()) {
    for (const auto &point : getPoints2D()) {
      std::vector<double> reals;

      if (point.isDiscontinuity()) {
        std::stringstream ss;
        ss << std::endl;
        tableFile_.print(ss.str());
      }

      (void) point.getReals(reals);

      if (reals.size() < 2)
        continue;

      // TODO: inside/outside test
      std::stringstream ss;
      ss << reals[0] << " " << reals[1] << " i" << std::endl;
      tableFile_.print(ss.str());
    }
  }
  else {
    if (isSurfaceData()) {
      if (isSurfaceEnabled())
        printSurfaceValues();
    }
    else {
      CGnuPlotStyleBase *style = app()->getPlotStyle(getStyle());

      if (style && style->has3D())
        printSurfaceValues(); // TODO: different format ?
      else {
        if (isSurfaceEnabled())
          printSurfaceValues();
      }
    }

    if (isContourEnabled()) {
      printContourValues();
    }
  }
}

void
CGnuPlotPlot::
printSurfaceValues() const
{
  int iy = 0;

  for (const auto &ip : getPoints3D()) {
    std::stringstream ss;
    ss << std::endl;
    ss << "# IsoCurve " << iy << ", " << ip.second.size() << " points" << std::endl;
    ss << "# x y z type" << std::endl;
    tableFile_.print(ss.str());

    //---

    for (const auto &point : ip.second) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      if (reals.size() < 3)
        continue;

      // TODO: inside/outside test
      std::stringstream ss;
      ss << reals[0] << " " << reals[1] << " " << reals[2] << " i" << std::endl;
      tableFile_.print(ss.str());
    }

    ++iy;
  }
}

void
CGnuPlotPlot::
printContourValues() const
{
  initContour();

#if 0
  // draw contour points
  int nx = contour_.nx();
  int ny = contour_.ny();

  for (int j = 0; j < ny - 1; ++j) {
    std::stringstream ss;
    ss << std::endl;
    ss << "# Contour " << j << std::endl;
    tableFile_.print(ss.str());

    double y = contour_.y(j);

    for (int i = 0; i < nx - 1; ++i) {
      double x = contour_.x(i);
      double z = contour_.z(i, j);

      std::stringstream ss;
      ss << x << " " << y << " " << z << std::endl;
      tableFile_.print(ss.str());
    }
  }
#endif

  const CGnuPlotContour::LevelLines &llines = contour_.levelLines();

  for (const auto &ll : llines) {
    int                           l     = ll.first;
    const CGnuPlotContour::Lines &lines = ll.second;

    std::stringstream ss;
    ss << std::endl;
    ss << "# Contour " << l << std::endl;
    tableFile_.print(ss.str());

    double z = contour_.levelValue(l);

    int il = 0;

    for (const auto &line : lines) {
      std::stringstream ss;

      if (il == 0)
        ss << line.start().x << " " << line.start().y << " " << z << std::endl;

      ss << line.end().x << " " << line.end().y << " " << z << std::endl;

      tableFile_.print(ss.str());

      ++il;
    }
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
updatePointCacheSize(int n)
{
  pointCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateArrowCacheSize(int n)
{
  arrowCache_.updateSize(n);
}

void
CGnuPlotPlot::
initRenderer(CGnuPlotRenderer *renderer)
{
  renderer->setRange(group_->getMappedDisplayRange(xind(), yind()));
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
  initContour();

  contour_.drawContour(renderer);
}

void
CGnuPlotPlot::
initContour() const
{
  CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

  if (! th->contourSet_) {
    std::pair<int,int> np = numPoints3D();

    //---

    th->contour_.reset();

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

    th->contour_.setData(&xc[0], &yc[0], &zc[0], np.first, np.second);

    th->contourSet_ = true;
  }
}

bool
CGnuPlotPlot::
isSurfaceData() const
{
  int nx = points3D_.size();

  if (nx <= 1) return false;

  int ny = -1;

  for (const auto &p : points3D_) {
    int ny1 = p.second.size();

    if      (ny < 0)
      ny = ny1;
    else if (ny1 != ny)
      return false;
  }

  return true;
}

void
CGnuPlotPlot::
drawSurface(CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CRGBA c = lineStyle.calcColor(group_, CRGBA(0,0,0));

  CGnuPlotCameraP camera = group()->camera();

  bool isCalcColor = lineStyle.isCalcColor();

  std::pair<int,int> np = numPoints3D();

  //---

  bool pm3D = (style_ == PlotStyle::PM3D || group()->pm3D()->isEnabled());

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

          p1 = group_->mapLogPoint(xind(), yind(), 1, p1);
          p2 = group_->mapLogPoint(xind(), yind(), 1, p2);
          p3 = group_->mapLogPoint(xind(), yind(), 1, p3);
          p4 = group_->mapLogPoint(xind(), yind(), 1, p4);

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

            CRGBA rgba1 = lineStyle.calcColor(this, ppoint1.getReal(nc));
            CRGBA rgba2 = lineStyle.calcColor(this, ppoint2.getReal(nc));
            CRGBA rgba3 = lineStyle.calcColor(this, ppoint3.getReal(nc));
            CRGBA rgba4 = lineStyle.calcColor(this, ppoint4.getReal(nc));

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
    if (group()->hidden3D()) {
      CRGBA fc(1,1,1);

      for (auto polys : surfaceZPolygons_) {
        for (auto poly : polys.second) {
          const PointsColor &pc = poly.second;

          renderer->fillPolygon(pc.first, fc);
          renderer->drawPolygon(pc.first, 0, c, CLineDash());
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

  if (pm3D) {
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

    const CRGBA &c = lineStyle.calcColor(group_, CRGBA(1,0,0));

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
                           lineStyle.calcWidth(), c);
    }
  }
}

void
CGnuPlotPlot::
drawClusteredHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &drawData)
{
  if (! isDisplayed()) return;

  //---

  CRGBA c = (fillType() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  CRGBA lc = lineStyle().calcColor(group_, c);
  CRGBA fc = lineStyle().calcColor(group_, CRGBA(0,0,0));

  //---

  if (! renderer->isPseudo())
    updateBarCacheSize(getPoints2D().size());

  int i = 0;

  for (const auto &point : getPoints2D()) {
    double x, y;

    if      (! point.getX(x)) {
      x = i;
      y = 0.0;
    }
    else if (! point.getY(y)) {
      y = x;
      x = i;
    }

    CRGBA fc1 = fc;

    if (point.hasParam("fillcolor"))
      fc1 = point.getParamColor("fillcolor");

    double xl = drawData.x2 + drawData.i*drawData.w + i*drawData.d;

    CBBox2D bbox;

    if (! drawData.horizontal)
      bbox = CBBox2D(xl + drawData.xb, drawData.y2, xl + drawData.w - drawData.xb, y);
    else
      bbox = CBBox2D(drawData.y2, xl + drawData.xb, y, xl + drawData.w - drawData.xb);

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = barObjects()[i];

      CGnuPlotAxis *plotXAxis = group_->getPlotAxis(CGnuPlotTypes::AxisType::X, 1);

      if (plotXAxis && plotXAxis->hasMajorTicLabels()) {
        const CGnuPlotAxisData::RTicLabels &labels = plotXAxis->getMajorTicLabels();

        auto pl = labels.find(x);

        if (pl != labels.end())
          bar->setXValueStr((*pl).second);
      }

      bar->setBBox  (bbox);
      bar->setValues(x, y);

      if (! bar->isInitialized()) {
        bar->setFillType   (fillType());
        bar->setFillPattern(fillPattern());
        bar->setFillColor  (fc1);

        if (fillStyle().hasBorder()) {
          bar->setBorder(true);

          CRGBA lc1 = lc;

          fillStyle().calcColor(group_, lc1);

          bar->setLineColor(lc1);
        }
        else
          bar->setBorder(false);

        bar->setInitialized(true);
      }
    }
    else {
      renderer->drawRect(bbox, lc, 1);
    }

    ++i;
  }

  if (! renderer->isPseudo())
    drawBars(renderer);
}

void
CGnuPlotPlot::
drawErrorBarsHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &drawData)
{
  if (! isDisplayed()) return;

  //---

  CRGBA c = (fillType() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  CRGBA lc = lineStyle().calcColor(group_, c);
  CRGBA fc = lineStyle().calcColor(group_, CRGBA(0,0,0));

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

      bar->setBBox  (bbox);
      bar->setValues(i, y);

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
drawStackedHistogram(CGnuPlotRenderer *renderer, int i, const CBBox2D &bbox, bool isColumn)
{
  if (! isDisplayed()) return;

  //---

  CRGBA lc, fc;

  if (! isColumn) {
    CRGBA c = (fillType() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

    lc = lineStyle().calcColor(group_, c);
    fc = lineStyle().calcColor(group_, CRGBA(0,0,0));
  }
  else {
    lc = CRGBA(0,0,0);
    fc = CGnuPlotStyleInst->indexColor(i + 1);
  }

  //---

  const CGnuPlotPoint &point = getPoint2D(i);

  double y;

  if (! point.getY(y))
    return;

  //---

  if (! renderer->isPseudo()) {
    CGnuPlotBarObject *bar = barObjects()[i];

    bar->setBBox  (bbox);
    bar->setValues(i, y);

    if (! bar->isInitialized()) {
      bar->setFillType   (fillType());
      bar->setFillPattern(fillPattern());
      bar->setFillColor  (fc);

      if (fillStyle().hasBorder()) {
        bar->setBorder(true);

        CRGBA lc1 = lc;

        fillStyle().calcColor(group_, lc1);

        bar->setLineColor(lc1);
      }
      else
        bar->setBorder(false);

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

  if (ls.isValid())
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

CGnuPlotPolygonObject *
CGnuPlotPlot::
createPolygonObject() const
{
  return app()->device()->createPolygonObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotRectObject *
CGnuPlotPlot::
createRectObject() const
{
  return app()->device()->createRectObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotPointObject *
CGnuPlotPlot::
createPointObject() const
{
  return app()->device()->createPointObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotArrowObject *
CGnuPlotPlot::
createArrowObject() const
{
  return app()->device()->createArrowObject(const_cast<CGnuPlotPlot *>(this));
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
          int      ind;

          if (! mapPoint3D(p, p1, ind))
            continue;

          if (IsNaN(p1.x))
            continue;

          th->xmin_.updateMin(p1.x);
          th->xmax_.updateMax(p1.x);
        }
      }
    }
    else {
      CGnuPlotBBoxRenderer brenderer(app()->renderer());

      if (renderBBox(brenderer)) {
        CBBox2D bbox = brenderer.bbox();

        th->xmin_.updateMin(bbox.getLeft ());
        th->xmax_.updateMax(bbox.getRight());

        th->cbmin_.updateMin(brenderer.cbMin());
        th->cbmax_.updateMin(brenderer.cbMax());
      }
      else {
        for (const auto &p : getPoints2D()) {
          double x;

          if (! p.getX(x))
            continue;

          if (IsNaN(x))
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
          int      ind;

          if (! mapPoint3D(p, p1, ind))
            continue;

          if (IsNaN(p1.y))
            continue;

          th->ymin_.updateMin(p1.y);
          th->ymax_.updateMax(p1.y);
        }
      }
    }
    else {
      CGnuPlotBBoxRenderer brenderer(app()->renderer());

      if (renderBBox(brenderer)) {
        CBBox2D bbox = brenderer.bbox();

        th->ymin_.updateMin(bbox.getBottom());
        th->ymax_.updateMax(bbox.getTop   ());
      }
      else {
        for (const auto &p : getPoints2D()) {
          double y;

          if (! p.getY(y))
            continue;

          if (IsNaN(y))
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
        int      ind;

        if (! mapPoint3D(p, p1, ind))
          continue;

        p1 = group_->mapLogPoint(xind(), yind(), 1, p1);

        th->zmin_.updateMin(p1.z);
        th->zmax_.updateMax(p1.z);
      }
    }
  }

  *zmin = zmin_.getValue(-10);
  *zmax = zmax_.getValue( 10);
}

bool
CGnuPlotPlot::
calcBoundedYRange(double *ymin, double *ymax) const
{
  // TODO: use dummy renderer which updates bbox

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
          int      ind;

          if (! mapPoint3D(p, p1, ind))
            continue;

          if (p1.x < xmin || p1.x > xmax)
            continue;

          if (IsNaN(p1.y))
            continue;

          th->bymin_.updateMin(p1.y);
          th->bymax_.updateMax(p1.y);
        }
      }
    }
    else {
      CGnuPlotBBoxRenderer brenderer(app()->renderer());

      brenderer.setXRange(xmin, xmax);

      if (renderBBox(brenderer)) {
        CBBox2D bbox = brenderer.bbox();

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

          if (IsNaN(y))
            continue;

          th->bymin_.updateMin(y);
          th->bymax_.updateMax(y);
        }
      }
    }
  }

  *ymin = bymin_.getValue(-10);
  *ymax = bymax_.getValue( 10);

  return (bymin_.isValid() && bymax_.isValid());
}

bool
CGnuPlotPlot::
renderBBox(CGnuPlotBBoxRenderer &brenderer) const
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return false;

  CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

  brenderer.setReverse(group_->xaxis(xind()).isReverse(), group_->yaxis(yind()).isReverse());

  style->draw2D(th, &brenderer);

  return true;
}

bool
CGnuPlotPlot::
mouseTip(const CPoint2D &p, CGnuPlotTipData &tipData)
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return false;

  return style->mouseTip(this, p, tipData);
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
        int      ind;

        if (! mapPoint3D(p, p1, ind))
          continue;

        if (! IsNaN(p1.x)) {
          xmin.updateMin(p1.x);
          xmax.updateMax(p1.x);
        }

        if (! IsNaN(p1.y)) {
          ymin.updateMin(p1.y);
          ymax.updateMax(p1.y);
        }
      }
    }
  }
  else {
    for (const auto &p : getPoints2D()) {
      double x, y;

      if (! p.getXY(x, y))
        continue;

      if (! IsNaN(x)) {
        xmin.updateMin(x);
        xmax.updateMax(x);
      }

      if (! IsNaN(y)) {
        ymin.updateMin(y);
        ymax.updateMax(y);
      }
    }
  }

  bbox = CBBox2D(xmin.getValue(-10), ymin.getValue(-10),
                 xmax.getValue( 10), ymax.getValue( 10));
}

bool
CGnuPlotPlot::
mapPoint3D(const CGnuPlotPoint &p, CPoint3D &p1, int &ind) const
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

    ind = (reals.size() > 3 ? 3 : reals.size());
  }
  else if (mapping() == CGnuPlotTypes::Mapping::CYLINDRICAL_MAPPING) {
    double theta = (reals.size() > 0 ? reals[0] : 0.0);
    double z     = (reals.size() > 1 ? reals[1] : 0.0);
    double r     = (reals.size() > 2 ? reals[2] : 1.0);

    double theta1 = app()->angleToRad(theta);

    double x = r*cos(theta1);
    double y = r*sin(theta1);

    p1 = CPoint3D(x, y, z);

    ind = (reals.size() > 3 ? 3 : reals.size());
  }
  else {
    if (! p.getPoint(p1))
      return false;

    ind = 3;
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

//------

CGnuPlotAdjacencyData::
~CGnuPlotAdjacencyData()
{
  delete adjacency_;
  delete renderer_;
}
