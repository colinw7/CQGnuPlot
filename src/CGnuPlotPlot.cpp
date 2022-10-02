#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotStyle.h>

#include <CGnuPlotArrowObject.h>
#include <CGnuPlotBoxBarObject.h>
#include <CGnuPlotBoxObject.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotImageObject.h>
#include <CGnuPlotFinanceBarObject.h>
#include <CGnuPlotLabelObject.h>
#include <CGnuPlotPathObject.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotPointObject.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotRectObject.h>

#include <CGnuPlotDevice.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotStyleBase.h>
#include <CGnuPlotUtil.h>

#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>
#include <CSmooth.h>

int CGnuPlotPlot::nextId_ = 1;

CGnuPlotPlot::
CGnuPlotPlot(CGnuPlotGroup *group, PlotStyle style) :
 group_(group), style_(style), id_(nextId_++), usingCols_(app()), imageStyle_(app()),
 lineStyle_(app()), contour_(this), surface_(this), arrowCache_(this), boxBarCache_(this),
 boxCache_(this), bubbleCache_(this), ellipseCache_(this), errorBarCache_(this),
 financeBarCache_(this), imageCache_(this), labelCache_(this), pathCache_(this), pieCache_(this),
 pointCache_(this), polygonCache_(this), rectCache_(this), tableFile_(app())
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
is2D() const
{
  return ! is3D();
}

void
CGnuPlotPlot::
set2D(bool b)
{
  return set3D(! b);
}

bool
CGnuPlotPlot::
is3D() const
{
  return group_->is3D();
}

void
CGnuPlotPlot::
set3D(bool b)
{
  return group_->set3D(b);
}

void
CGnuPlotPlot::
getKeyLabels(CGnuPlotPlot::KeyLabels &labels) const
{
  if (style() == CGnuPlot::PlotStyle::TEST_PALETTE) {
    labels.push_back(CGnuPlotKeyLabel("red"  , CRGBA(1,0,0)));
    labels.push_back(CGnuPlotKeyLabel("green", CRGBA(0,1,0)));
    labels.push_back(CGnuPlotKeyLabel("blue" , CRGBA(0,0,1)));
    labels.push_back(CGnuPlotKeyLabel("NTSC" , CRGBA(0,0,0)));
  }
  else {
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
}

void
CGnuPlotPlot::
init()
{
  CGnuPlot *plot = this->app();

  //---

  setBars(plot->bars());

  //setBoxWidth(plot->boxWidth());

  setFillStyle (plot->fillStyle ());
  setPointStyle(plot->pointStyle());

  setAngleType(plot->angleType());

  xind_ = plot->xind();
  yind_ = plot->yind();

  setFilledCurve(plot->filledCurve());
  setHidden3D(plot->hidden3D());

  if (plot->whiskerBars().isValid())
    setWhiskerBars(plot->whiskerBars().getValue());
  else
    resetWhiskerBars();

  setBinary(plot->isBinary());

  setMatrixData(plot->matrixData());

  setMapping(plot->mapping());

  setContourData(plot->contourData());
  setSurfaceData(plot->surfaceData());

  setKeyData(plot->keyData());

  setEnhanced(plot->device()->isEnhanced());

  setTableFile(plot->tableFile());

  setNewHistogramId(plot->newHistogramId());

  setPolar     (plot->isPolar());
  setParametric(plot->isParametric());

  //---

  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  double idx = imageStyle.dx().getValue(1);
  double idy = imageStyle.dy().getValue(1);
  double idz = 1;
  double idt = imageStyle.dt().getValue(1);

  setDelta(Delta(idx, idy, idz, idt));

  if (imageStyle.origin().isValid() || imageStyle.angle().isValid()) {
    CPoint3D o = imageStyle.origin().getValue(CPoint3D(0,0,0));

    setRotate(CGnuPlotRotate(o.toPoint2D(), imageStyle.angle().getValue(0)));
  }

  if (imageStyle.center().isValid())
    setCenter(imageStyle.center());

  setOriginArray(imageStyle.originArray());

  setSamples   (plot->samples   ());
  setISOSamples(plot->isoSamples());
  setSampleVars(plot->sampleVars());
  setDummyVars (plot->dummyVars ());

  setAxesData(plot->axesData());
}

void
CGnuPlotPlot::
setStyle(PlotStyle style)
{
  style_ = style;

  bymin_.setInvalid();
  bymax_.setInvalid();

  clearCaches();
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
    values.push_back(app()->expr()->createRealValue(r));

  return addPoint2D(values, false);
}

int
CGnuPlotPlot::
addPoint2D(double x, CExprValuePtr y)
{
  assert(! is3D());

  std::vector<CExprValuePtr> values;

  values.push_back(app()->expr()->createRealValue(x));
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

  CExpr *expr = app()->expr();

  std::vector<CExprValuePtr> values;

  values.push_back(expr->createRealValue(x));
  values.push_back(expr->createRealValue(y));
  values.push_back(expr->createRealValue(z));

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, CExprValuePtr z)
{
  assert(is3D());

  CExpr *expr = app()->expr();

  std::vector<CExprValuePtr> values;

  values.push_back(expr->createRealValue(x));
  values.push_back(expr->createRealValue(y));
  values.push_back(z);

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, const CPoint3D &p)
{
  assert(is3D());

  CExpr *expr = app()->expr();

  std::vector<CExprValuePtr> values;

  values.push_back(expr->createRealValue(p.x));
  values.push_back(expr->createRealValue(p.y));
  values.push_back(expr->createRealValue(p.z));

  return addPoint3D(iy, values, false);
}

int
CGnuPlotPlot::
addPoint3D(int iy, const Values &values, bool discontinuity, bool bad, const Params &)
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

        y /= double(p.second.size());

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
    else if (smooth_ == Smooth::CSPLINES) {
      MappedPoints mappedPoints;

      mapPoints(mappedPoints);

      Points points;

      // smooth curve fit through points
      for (const auto &p : mappedPoints) {
        double x = p.first;

        double y = 0.0;

        for (const auto &y1 : p.second)
          y += y1;

        points.push_back(CPoint2D(x, y/double(p.second.size())));
      }

      CSmooth smooth(points);

      int nx, ny;

      app()->samples().get(nx, ny);

      clearPoints();

      double x1 = points.front().x;
      double dx = (points.back().x - x1)/(nx - 1);

      for (int i = 0; i < nx; ++i) {
        double x = x1 + i*dx;

        (void) addPoint2D(x, smooth.interp(x));
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
printPoints()
{
  for (uint i = 0; i < numPoints2D(); ++i) {
    const CGnuPlotPoint &point = points2D_[i];

    for (int j = 0; j < point.getNumValues(); ++j) {
      if (j > 0)
        std::cout << " ";

      const CExprValuePtr &value = point.getValue(j);

      if      (value->isIntegerValue()) {
        long l;

        value->getIntegerValue(l);

        std::cout << l;
      }
      else if (value->isRealValue()) {
        double r;

        value->getRealValue(r);

        std::cout << r;
      }
      else if (value->isStringValue()) {
        std::string s;

        value->getStringValue(s);

        std::cout << '"' << s << '"';
      }
      else
        std::cout << "?";
    }

    std::cout << std::endl;
  }
}

void
CGnuPlotPlot::
draw()
{
  if (! isDisplayed()) return;

  //---

  if (! is3D()) {
    const CGnuPlotAxisData &xaxis = group_->xaxis(xind());
    const CGnuPlotAxisData &yaxis = group_->yaxis(yind());

    double xmin = xaxis.min().getValue(0.0);
    double ymin = yaxis.min().getValue(0.0);
    double xmax = xaxis.max().getValue(1.0);
    double ymax = yaxis.max().getValue(1.0);

    CPoint2D p1 = group_->mapLogPoint(xind(), yind(), zind(), CPoint2D(xmin, ymin));
    CPoint2D p2 = group_->mapLogPoint(xind(), yind(), zind(), CPoint2D(xmax, ymax));

    bbox2D_ = CBBox2D(p1, p2);
  }
  else {
    const CGnuPlotAxisData &xaxis = group_->xaxis(xind());
    const CGnuPlotAxisData &yaxis = group_->yaxis(yind());
    const CGnuPlotAxisData &zaxis = group_->zaxis(zind());

    double xmin = xaxis.min().getValue(0.0);
    double ymin = yaxis.min().getValue(0.0);
    double zmin = zaxis.min().getValue(0.0);
    double xmax = xaxis.max().getValue(1.0);
    double ymax = yaxis.max().getValue(1.0);
    double zmax = zaxis.max().getValue(1.0);

    CPoint3D p1 = group_->mapLogPoint(xind(), yind(), zind(), CPoint3D(xmin, ymin, zmin));
    CPoint3D p2 = group_->mapLogPoint(xind(), yind(), zind(), CPoint3D(xmax, ymax, zmax));

    bbox3D_ = CBBox3D(p1, p2);
  }

  //---

  CGnuPlotRenderer *renderer = app()->renderer();

  // normal plot (no children)
  if (style() == PlotStyle::TEST_TERMINAL || style() == PlotStyle::TEST_PALETTE) {
    CGnuPlotStyleBase *style = app()->getPlotStyle(this->style());

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
      if (surfaceData_.isEnabled())
        printSurfaceValues();
    }
    else {
      CGnuPlotStyleBase *style = app()->getPlotStyle(this->style());

      if (style && style->has3D())
        printSurfaceValues(); // TODO: different format ?
      else {
        if (surfaceData_.isEnabled())
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
      std::stringstream ss1;
      ss1 << reals[0] << " " << reals[1] << " " << reals[2] << " i" << std::endl;
      tableFile_.print(ss1.str());
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

      std::stringstream ss1;
      ss1 << x << " " << y << " " << z << std::endl;
      tableFile_.print(ss1.str());
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
      std::stringstream ss1;

      if (il == 0)
        ss1 << line.start().x << " " << line.start().y << " " << z << std::endl;

      ss1 << line.end().x << " " << line.end().y << " " << z << std::endl;

      tableFile_.print(ss1.str());

      ++il;
    }
  }
}

//---

void
CGnuPlotPlot::
clearCaches()
{
  arrowCache_     .clear();
  boxBarCache_    .clear();
  clearBoxCache   ();
  bubbleCache_    .clear();
  ellipseCache_   .clear();
  errorBarCache_  .clear();
  financeBarCache_.clear();
  imageCache_     .clear();
  labelCache_     .clear();
  pathCache_      .clear();
  clearPieCache   ();
  pointCache_     .clear();
  polygonCache_   .clear();
  clearRectCache  ();
}

void
CGnuPlotPlot::
updateArrowCacheSize(int n)
{
  arrowCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateBoxBarCacheSize(int n)
{
  boxBarCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateBoxCacheSize(int n)
{
  boxCache_.updateSize(n);
}

void
CGnuPlotPlot::
clearBoxCache()
{
  boxCache_.clear();
}

void
CGnuPlotPlot::
updateBubbleCacheSize(int n)
{
  bubbleCache_.updateSize(n);
}

void
CGnuPlotPlot::
resetArrowCache()
{
  arrowCache_.reset();
}

void
CGnuPlotPlot::
resetBubbleCache()
{
  bubbleCache_.reset();
}

void
CGnuPlotPlot::
updateEllipseCacheSize(int n)
{
  ellipseCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateErrorBarCacheSize(int n)
{
  errorBarCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateFinanceBarCacheSize(int n)
{
  financeBarCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateImageCacheSize(int n)
{
  imageCache_.updateSize(n);
}

void
CGnuPlotPlot::
updateLabelCacheSize(int n)
{
  labelCache_.updateSize(n);
}

void
CGnuPlotPlot::
resetLabelCache()
{
  labelCache_.reset();
}

void
CGnuPlotPlot::
updatePathCacheSize(int n)
{
  pathCache_.updateSize(n);
}

void
CGnuPlotPlot::
updatePieCacheSize(int n)
{
  pieCache_.updateSize(n);
}

void
CGnuPlotPlot::
clearPieCache()
{
  pieCache_.clear();
}

void
CGnuPlotPlot::
updatePointCacheSize(int n)
{
  pointCache_.updateSize(n);
}

void
CGnuPlotPlot::
updatePolygonCacheSize(int n)
{
  polygonCache_.updateSize(n);
}

void
CGnuPlotPlot::
resetPolygonCache()
{
  polygonCache_.reset();
}

void
CGnuPlotPlot::
updateRectCacheSize(int n)
{
  rectCache_.updateSize(n);
}

void
CGnuPlotPlot::
clearRectCache()
{
  rectCache_.clear();
}

void
CGnuPlotPlot::
resetRectCache()
{
  rectCache_.reset();
}

//---

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

  bool pm3D = (style_ == PlotStyle::PM3D || group()->pm3D()->isEnabled());

  if (! pm3D) {
    if (isContourEnabled())
      drawContour(renderer);
  }

  CGnuPlotStyleBase *style = app()->getPlotStyle(this->style());

  if (style && style->has3D())
    if (this->style() == CGnuPlot::PlotStyle::IMAGE    ||
        this->style() == CGnuPlot::PlotStyle::RGBIMAGE ||
        this->style() == CGnuPlot::PlotStyle::POINTS   ||
        this->style() == CGnuPlot::PlotStyle::DOTS) {
      style->draw3D(this, renderer);
    }
    else {
      if (isSurfaceData()) {
        if (surfaceData_.isEnabled())
          drawSurface(renderer);
      }
      else {
        style->draw3D(this, renderer);
    }
  }
  else {
    if (surfaceData_.isEnabled())
      drawSurface(renderer);
  }

  if (pm3D) {
    if (isContourEnabled())
      drawContour(renderer);
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

    std::vector<double> xc, yc, zc, cc;

    xc.resize(np.first);
    yc.resize(np.second);

    int nz = np.first*np.second;

    zc.resize(nz);

    for (int iy = 0; iy < np.second; ++iy) {
      for (int ix = 0; ix < np.first; ++ix) {
        const CGnuPlotPoint &point = getPoint3D(ix, iy);

        double x, y;

        point.getX(x); point.getY(y);

        xc[ix] = x;
        yc[iy] = y;

        int iz = iy*np.first + ix;

        zc[iz] = point.getZ();

        if (point.getNumValues() == 4) {
          if (cc.empty())
            cc.resize(nz);

          cc[iz] = point.getReal(4);
        }
      }
    }

    bool has_c = ! cc.empty();

    th->contour_.setData(&xc[0], &yc[0], &zc[0], &cc[0], np.first, np.second, has_c);

    th->contourSet_ = true;
  }
}

bool
CGnuPlotPlot::
isSurfaceData() const
{
  int nx = int(points3D_.size());

  if (nx <= 1) return false;

  int ny = -1;

  for (const auto &p : points3D_) {
    int ny1 = int(p.second.size());

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
  bool pm3D = (style_ == PlotStyle::PM3D || group()->pm3D()->isEnabled());

  if (surfaceData_.isEnabled() || pm3D) {
    initSurface();
  }

  //---

  surface_.draw(renderer);
}

void
CGnuPlotPlot::
initSurface() const
{
  CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

  if (! th->surfaceSet_) {
    const CGnuPlotLineStyle &lineStyle = this->lineStyle();

    CGnuPlotCameraP camera = group()->camera();

    CGnuPlotColorBoxP cb = group()->colorBox();

    const COptPoint3D &center = this->center();

    //---

    std::pair<int,int> np = numPoints3D();

    bool isCalcColor = lineStyle.isCalcColor();
    bool isDataColor = false;

    COptReal cmin, cmax;

    if (! isCalcColor) {
      for (int iy = 0; iy < np.second; ++iy) {
        for (int ix = 0; ix < np.first; ++ix) {
          const CGnuPlotPoint &ppoint = getPoint3D(ix, iy);

          if (ppoint.getNumValues() >= 4) {
            double c = ppoint.getReal(4);

            cmin.updateMin(c);
            cmax.updateMax(c);
          }
        }
      }

      isDataColor = cmin.isValid();

      if (isDataColor) {
        cb->axis().setMin(cmin.getValue());
        cb->axis().setMax(cmax.getValue());
      }
    }

    th->surface_.setCalcColor(isCalcColor);
    th->surface_.setDataColor(isDataColor);

    //---

    CGnuPlotSurface::ZPolygons zpolygons;
    CGnuPlotSurface::IJPoints  ijpoints;
    COptReal                   zmin, zmax;

    int ind = 0;

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

        if (center.isValid()) {
          p1 += center.getValue();
          p2 += center.getValue();
          p3 += center.getValue();
          p4 += center.getValue();
        }

        p1 = group_->mapLogPoint(xind(), yind(), 1, p1);
        p2 = group_->mapLogPoint(xind(), yind(), 1, p2);
        p3 = group_->mapLogPoint(xind(), yind(), 1, p3);
        p4 = group_->mapLogPoint(xind(), yind(), 1, p4);

        // average color
        double za;
        CRGBA  ca;

        if      (th->surface_.isCalcColor()) {
          double z1 = (ppoint1.getNumValues() > 3 ? p1.z : 0);
          double z2 = (ppoint2.getNumValues() > 3 ? p2.z : 0);
          double z3 = (ppoint3.getNumValues() > 3 ? p3.z : 0);
          double z4 = (ppoint4.getNumValues() > 3 ? p4.z : 0);

          za = CGnuPlotUtil::avg({z1, z2, z3, z4});

          zmin.updateMin(za);
          zmax.updateMax(za);

          int nc = ppoint1.getNumValues();

          CRGBA rgba1 = lineStyle.calcColor(th, ppoint1.getReal(nc));
          CRGBA rgba2 = lineStyle.calcColor(th, ppoint2.getReal(nc));
          CRGBA rgba3 = lineStyle.calcColor(th, ppoint3.getReal(nc));
          CRGBA rgba4 = lineStyle.calcColor(th, ppoint4.getReal(nc));

          ca = CGnuPlotUtil::avgT<CRGBA>({rgba1, rgba2, rgba3, rgba4}, CRGBA(0,0,0,0));
        }
        else if (th->surface_.isDataColor()) {
          za = CGnuPlotUtil::avg({p1.z, p2.z, p3.z, p4.z});

          zmin.updateMin(za);
          zmax.updateMax(za);

          CRGBA rgba1 = cb->valueToColor(ppoint1.getReal(4)).rgba();
          CRGBA rgba2 = cb->valueToColor(ppoint2.getReal(4)).rgba();
          CRGBA rgba3 = cb->valueToColor(ppoint3.getReal(4)).rgba();
          CRGBA rgba4 = cb->valueToColor(ppoint4.getReal(4)).rgba();

          ca = CGnuPlotUtil::avgT<CRGBA>({rgba1, rgba2, rgba3, rgba4}, CRGBA(0,0,0,0));
        }
        else {
          za = CGnuPlotUtil::avg({p1.z, p2.z, p3.z, p4.z});

          zmin.updateMin(za);
          zmax.updateMax(za);
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
        Points poly({pt1.toPoint2D(), pt2.toPoint2D(), pt3.toPoint2D(), pt4.toPoint2D()});

        if (CGnuPlotUtil::isNaN(poly))
          poly.clear();

        CGnuPlotSurface::PointsIndColor pointsIndColor(CGnuPlotSurface::PointsInd(poly, ind), ca);

        zpolygons[zm].push_back(CGnuPlotSurface::ZPoints(za, pointsIndColor));

        ijpoints[ix - 1][iy - 1] = poly;

        ++ind;
      }
    }

    th->surface_.setData(zmin, zmax, zpolygons, ijpoints);

    th->surfaceSet_ = true;
  }
}

void
CGnuPlotPlot::
draw2D(CGnuPlotRenderer *renderer)
{
  assert(! is3D());

  //---

  renderer->setClip(bbox2D_);

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

    CGnuPlotStroke stroke(this);

    CRGBA c = stroke.color();

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
                           c, lineStyle.calcWidth());
    }
  }
}

void
CGnuPlotPlot::
drawClusteredHistogram(CGnuPlotRenderer *renderer, const DrawHistogramData &drawData)
{
  if (! isDisplayed()) return;

  //---

  CGnuPlotFill   fill  (this);
  CGnuPlotStroke stroke(this);

  //---

  if (! renderer->isPseudo())
    updateBoxBarCacheSize(int(getPoints2D().size()));

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

    COptRGBA fc;

    if (point.hasParam("fillcolor"))
      fc = point.getParamColor("fillcolor");

    double xl = drawData.x2 + drawData.i*drawData.w + i*drawData.d;

    CBBox2D bbox;

    if (! drawData.horizontal)
      bbox = CBBox2D(xl + drawData.xb, drawData.y2, xl + drawData.w - drawData.xb, y);
    else
      bbox = CBBox2D(drawData.y2, xl + drawData.xb, y, xl + drawData.w - drawData.xb);

    if (! renderer->isPseudo()) {
      CGnuPlotBoxBarObject *bar = boxBarObjects()[i];

      CGnuPlotAxis *plotXAxis = group_->getPlotAxis(CGnuPlotTypes::AxisType::X, 1);

      if (plotXAxis && plotXAxis->hasMajorTicLabels()) {
        const CGnuPlotAxisData::RTicLabels &labels = plotXAxis->getMajorTicLabels();

        auto pl = labels.find(x);

        if (pl != labels.end())
          bar->setXValueStr((*pl).second);
      }

      bar->setBBox  (bbox);
      bar->setValues(x, y);

      if (! bar->testAndSetUsed()) {
        CGnuPlotFillP   fill1  (bar->fill  ()->dup());
        CGnuPlotStrokeP stroke1(bar->stroke()->dup());

        if (fc.isValid())
          fill1->setColor(fc.getValue());

        bar->setFill  (fill1);
        bar->setStroke(stroke1);
      }
    }
    else {
    //CGnuPlotFill   fill1;
      CGnuPlotStroke stroke1;

      stroke1.setEnabled(true);

    //renderer->fillRect  (bbox, fill1);
      renderer->strokeRect(bbox, stroke1);
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

  CGnuPlotFill   fill  (this);
  CGnuPlotStroke stroke(this);

  //---

  if (! renderer->isPseudo())
    updateBoxBarCacheSize(int(getPoints2D().size()));

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

    COptRGBA fc;

    if (point.hasParam("fillcolor"))
      fc = point.getParamColor("fillcolor");

    double xl = drawData.x2 + drawData.i*drawData.w + i*drawData.d;

    double xlb = xl + drawData.xb;
    double xrb = xl + drawData.w - drawData.xb;

    CBBox2D bbox(xlb, drawData.y2, xrb, y);

    if (! renderer->isPseudo()) {
      CGnuPlotBoxBarObject *bar = boxBarObjects()[i];

      bar->setBBox  (bbox);
      bar->setValues(i, y);

      if (! bar->testAndSetUsed()) {
        CGnuPlotFillP   fill1   = bar->fill  ();
        CGnuPlotStrokeP stroke1 = bar->stroke();

        if (fc.isValid())
          fill1->setColor(fc.getValue());

        bar->setFill  (fill1  );
        bar->setStroke(stroke1);
      }
    }
    else {
      CGnuPlotFill   fill1;
      CGnuPlotStroke stroke1;

      renderer->fillRect  (bbox, fill1);
      renderer->strokeRect(bbox, stroke1);
    }

    double xm = (xlb + + xrb)/2;

    renderer->strokeClipLine(CPoint2D(xm , yl), CPoint2D(xm , yh), stroke);
    renderer->strokeClipLine(CPoint2D(xlb, yl), CPoint2D(xrb, yl), stroke);
    renderer->strokeClipLine(CPoint2D(xlb, yh), CPoint2D(xrb, yh), stroke);

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

  COptRGBA fc, lc;

  if (isColumn) {
    fc = CGnuPlotStyleInst->indexColor(i + 1);
    lc = CRGBA(0,0,0);
  }

  //---

  const CGnuPlotPoint &point = getPoint2D(i);

  double y;

  if (! point.getY(y))
    return;

  //---

  if (! renderer->isPseudo()) {
    CGnuPlotBoxBarObject *bar = boxBarObjects()[i];

    bar->setBBox  (bbox);
    bar->setValues(i, y);

    if (! bar->testAndSetUsed()) {
      CGnuPlotFillP   fill   = bar->fill();
      CGnuPlotStrokeP stroke = bar->stroke();

      if (fc.isValid())
        fill->setColor(fc.getValue());

      if (lc.isValid())
        stroke->setColor(lc.getValue());

      bar->setFill  (fill  );
      bar->setStroke(stroke);
    }
  }
  else {
    CGnuPlotFill   fill  (this);
    CGnuPlotStroke stroke(this);

    renderer->fillRect  (bbox, fill  );
    renderer->strokeRect(bbox, stroke);
  }
}

void
CGnuPlotPlot::
drawBars(CGnuPlotRenderer *renderer)
{
  for (const auto &bar : boxBarObjects())
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

//-----

CGnuPlotArrowObject *
CGnuPlotPlot::
createArrowObject() const
{
  return app()->device()->createArrowObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotBoxBarObject *
CGnuPlotPlot::
createBoxBarObject() const
{
  return app()->device()->createBoxBarObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotEndBar *
CGnuPlotPlot::
createEndBar() const
{
  return app()->device()->createEndBar(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotBoxObject *
CGnuPlotPlot::
createBoxObject() const
{
  return app()->device()->createBoxObject(const_cast<CGnuPlotPlot *>(this));
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

CGnuPlotErrorBarObject *
CGnuPlotPlot::
createErrorBarObject() const
{
  return app()->device()->createErrorBarObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotFinanceBarObject *
CGnuPlotPlot::
createFinanceBarObject() const
{
  return app()->device()->createFinanceBarObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotImageObject *
CGnuPlotPlot::
createImageObject() const
{
  return app()->device()->createImageObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotLabelObject *
CGnuPlotPlot::
createLabelObject() const
{
  return app()->device()->createLabelObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotPathObject *
CGnuPlotPlot::
createPathObject() const
{
  return app()->device()->createPathObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotPieObject *
CGnuPlotPlot::
createPieObject() const
{
  return app()->device()->createPieObject(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotPointObject *
CGnuPlotPlot::
createPointObject() const
{
  return app()->device()->createPointObject(const_cast<CGnuPlotPlot *>(this));
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

CGnuPlotFill *
CGnuPlotPlot::
createFill() const
{
  return app()->device()->createFill(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotStroke *
CGnuPlotPlot::
createStroke() const
{
  return app()->device()->createStroke(const_cast<CGnuPlotPlot *>(this));
}

CGnuPlotMark *
CGnuPlotPlot::
createMark() const
{
  return app()->device()->createMark(const_cast<CGnuPlotPlot *>(this));
}

//------

void
CGnuPlotPlot::
calcXRange(double *xmin, double *xmax) const
{
  // calc range for all x values
  if (! xmin_.isValid() || ! xmax_.isValid()) {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    if (is3D() && ! isImageStyle()) {
      for (const auto &ip : getPoints3D()) {
        for (const auto &p : ip.second) {
          CPoint3D p1;
          int      ind;

          if (! mapPoint3D(p, p1, ind))
            continue;

          if (COSNaN::is_nan(p1.x))
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

        if (bbox.isSet()) {
          th->xmin_.updateMin(bbox.getLeft ());
          th->xmax_.updateMax(bbox.getRight());
        }

        th->cbmin_.updateMin(brenderer.cbMin());
        th->cbmax_.updateMin(brenderer.cbMax());
      }
      else {
        for (const auto &p : getPoints2D()) {
          double x;

          if (! p.getX(x))
            continue;

          if (COSNaN::is_nan(x))
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

    if (is3D() && ! isImageStyle()) {
      for (const auto &ip : getPoints3D()) {
        for (const auto &p : ip.second) {
          CPoint3D p1;
          int      ind;

          if (! mapPoint3D(p, p1, ind))
            continue;

          if (COSNaN::is_nan(p1.y))
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

        if (bbox.isSet()) {
          th->ymin_.updateMin(bbox.getBottom());
          th->ymax_.updateMax(bbox.getTop   ());
        }
      }
      else {
        for (const auto &p : getPoints2D()) {
          double y;

          if (! p.getY(y))
            continue;

          if (COSNaN::is_nan(y))
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

    if (is3D() && ! isImageStyle()) {
      for (const auto &ip : getPoints3D()) {
        for (const auto &p : ip.second) {
          CPoint3D p1;
          int      ind;

          if (! mapPoint3D(p, p1, ind))
            continue;

          if (p1.x < xmin || p1.x > xmax)
            continue;

          if (COSNaN::is_nan(p1.y))
            continue;

          th->bymin_.updateMin(p1.y);
          th->bymax_.updateMax(p1.y);
        }
      }
    }
    else {
      CGnuPlotBBoxRenderer brenderer(app()->renderer());

      CPoint2D p1 = group_->mapLogPoint(xind(), yind(), 1, CPoint2D(xmin, 1));
      CPoint2D p2 = group_->mapLogPoint(xind(), yind(), 1, CPoint2D(xmax, 1));

      brenderer.setXRange(p1.x, p2.x);

      if (renderBBox(brenderer)) {
        CBBox2D bbox1 = brenderer.bbox();

        if (bbox1.isSet()) {
          CPoint2D pl1 = group_->unmapLogPoint(xind(), yind(), 1, bbox1.getLL());
          CPoint2D pl2 = group_->unmapLogPoint(xind(), yind(), 1, bbox1.getUR());

          th->bymin_.updateMin(pl1.y);
          th->bymax_.updateMax(pl2.y);
        }
      }
      else {
        for (const auto &p : getPoints2D()) {
          double x, y;

          if (! p.getXY(x, y))
            continue;

          if (x < xmin || x > xmax)
            continue;

          if (COSNaN::is_nan(y))
            continue;

          th->bymin_.updateMin(y);
          th->bymax_.updateMax(y);
        }
      }
    }
  }

  if (! group_->yaxis(yind()).isLogValue()) {
    *ymin = bymin_.getValue(-10);
    *ymax = bymax_.getValue( 10);
  }
  else {
    *ymin = bymin_.getValue(1);
    *ymax = bymax_.getValue(10);
  }

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
mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData)
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return false;

  return style->mouseTip(this, mouseEvent, tipData);
}

void
CGnuPlotPlot::
mousePress(const CGnuPlotMouseEvent &mouseEvent)
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return;

  style->mousePress(this, mouseEvent);
}

void
CGnuPlotPlot::
mouseMove(const CGnuPlotMouseEvent &mouseEvent, bool pressed)
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return;

  style->mouseMove(this, mouseEvent, pressed);
}

void
CGnuPlotPlot::
mouseRelease(const CGnuPlotMouseEvent &mouseEvent)
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return;

  style->mouseRelease(this, mouseEvent);
}

bool
CGnuPlotPlot::
mouseProbe(CGnuPlotProbeEvent &probeEvent)
{
  CGnuPlotStyleBase *style = app()->getPlotStyle(style_);
  if (! style) return false;

  return style->mouseProbe(this, probeEvent);
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

  CGnuPlotAxisData &xaxis = group_->xaxis(xind());

  if (xaxis.min().isValid())
    *xmin = xaxis.min().getValue();
  if (xaxis.max().isValid())
    *xmax = xaxis.max().getValue();

  //CGnuPlotAxisData &yaxis = group_->yaxis(yind());

  //if (yaxis.min().isValid()) ymin_ = yaxis.min().getValue();
  //if (yaxis.max().isValid()) ymax_ = yaxis.max().getValue();
}

void
CGnuPlotPlot::
getPointsRange(CBBox2D &bbox) const
{
  COptReal xmin, ymin, xmax, ymax;

  if (is3D() && ! isImageStyle()) {
    for (const auto &ip : getPoints3D()) {
      for (const auto &p : ip.second) {
        CPoint3D p1;
        int      ind;

        if (! mapPoint3D(p, p1, ind))
          continue;

        if (! COSNaN::is_nan(p1.x)) {
          xmin.updateMin(p1.x);
          xmax.updateMax(p1.x);
        }

        if (! COSNaN::is_nan(p1.y)) {
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

      if (! COSNaN::is_nan(x)) {
        xmin.updateMin(x);
        xmax.updateMax(x);
      }

      if (! COSNaN::is_nan(y)) {
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

  (void) p.getReals(reals, CGnuPlotPoint::GetOpts().setForce(true));

  if      (mapping() == CGnuPlotTypes::Mapping::SPHERICAL_MAPPING) {
    double theta = (reals.size() > 0 ? reals[0] : 0.0);
    double phi   = (reals.size() > 1 ? reals[1] : 0.0);
    double r     = (reals.size() > 2 ? reals[2] : 1.0);

    double theta1 = angleToRad(theta);
    double phi1   = angleToRad(phi);

    //p1 = app()->sphericalMap(p2);

    double x = r*cos(theta1)*cos(phi1);
    double y = r*sin(theta1)*cos(phi1);
    double z = r*sin(phi1);

    p1 = CPoint3D(x, y, z);

    ind = (reals.size() > 3 ? 3 : int(reals.size()));
  }
  else if (mapping() == CGnuPlotTypes::Mapping::CYLINDRICAL_MAPPING) {
    double theta = (reals.size() > 0 ? reals[0] : 0.0);
    double z     = (reals.size() > 1 ? reals[1] : 0.0);
    double r     = (reals.size() > 2 ? reals[2] : 1.0);

    double theta1 = angleToRad(theta);

    double x = r*cos(theta1);
    double y = r*sin(theta1);

    p1 = CPoint3D(x, y, z);

    ind = (reals.size() > 3 ? 3 : int(reals.size()));
  }
  else {
    if (! p.getPoint(p1, CGnuPlotPoint::GetOpts().setForce(true)))
      return false;

    ind = 3;
  }

  return true;
}

#if 0
CPoint3D
CGnuPlotPlot::
sphericalMap(const CPoint2D &p) const
{
  static double rad = 6378137.0;          // Radius of the Earth (in meters)
  static double f   = 1.0/298.257223563;  // Flattening factor WGS84 Model

  double alt = 0.0; // altitude

  double cosLon = cos(angleToRad(p.x));
  double sinLon = sin(angleToRad(p.x));
  double cosLat = cos(angleToRad(p.y));
  double sinLat = sin(angleToRad(p.y));

  double ff = (1.0 - f)*(1.0 - f);
  double c  = 1.0/sqrt(cosLat*cosLat + ff*sinLat*sinLat);
  double s  = c*ff;

  double x = (rad*c + alt)*cosLat*cosLon;
  double y = (rad*c + alt)*cosLat*sinLon;
  double z = (rad*s + alt)*sinLat;

  return CPoint3D(x, y, z);
}
#endif

CPoint2D
CGnuPlotPlot::
convertPolarAxisPoint(const CPoint2D &p, bool &inside) const
{
  CGnuPlotAxis *plotTAxis = group_->getPlotAxis(CGnuPlotTypes::AxisType::T, 1, true);
  CGnuPlotAxis *plotRAxis = group_->getPlotAxis(CGnuPlotTypes::AxisType::R, 1, true);

  CPoint2D p1 = p;

  inside = plotTAxis->mappedInside(p1.x) && plotRAxis->mappedInside(p1.y);

  if (! plotRAxis->logBase().isValid()) {
    double rmin = plotRAxis->getStart();
    double rmax = plotRAxis->getEnd  ();
    double s    = rmax - rmin;

    p1.y = CGnuPlotUtil::map(p1.y, rmin, rmax, 0, s);
  }

  p1 = convertPolarPoint(p1);

  return p1;
}

CPoint2D
CGnuPlotPlot::
convertPolarPoint(const CPoint2D &p) const
{
  double a = angleToRad(p.x);
  double r = p.y;

  double x = r*cos(a);
  double y = r*sin(a);

  return CPoint2D(x, y);
}

double
CGnuPlotPlot::
angleToRad(double a) const
{
  return (isAngleDegrees() ? CAngle::Deg2Rad(a) : a);
}

double
CGnuPlotPlot::
angleToDeg(double a) const
{
  return (! isAngleDegrees() ? CAngle::Rad2Deg(a) : a);
}

void
CGnuPlotPlot::
setStyleValue(const std::string &name, CGnuPlotStyleValue *value)
{
  auto p = styleValues_.find(name);

  if (p != styleValues_.end())
    delete (*p).second;

  styleValues_[name] = value;
}

CGnuPlotStyleValue *
CGnuPlotPlot::
styleValue(const std::string &name) const
{
  auto p = styleValues_.find(name);

  if (p == styleValues_.end())
    return 0;

  return (*p).second;
}
