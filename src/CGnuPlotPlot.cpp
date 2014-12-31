#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotStyle.h>
#include <CGnuPlotUtil.h>

#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>

//------

namespace {
  double Deg2Rad(double d) { return M_PI*d/180.0; }
  double Rad2Deg(double d) { return 180.0*d/M_PI; }
}

//------

int CGnuPlotPlot::nextId_ = 1;

CGnuPlotPlot::
CGnuPlotPlot(CGnuPlotWindow *window) :
 window_(window), id_(nextId_++), is3D_(false), parentPlot_(0), region_(0,0,1,1),
 margin_(10,10,10,10), ind_(0), style_(CGnuPlot::PlotStyle::POINTS),
 smooth_(CGnuPlot::Smooth::NONE), histogramStyle_(CGnuPlot::HistogramStyle::NONE),
 fitX_(false), fitY_(false), contour_(this), contourSet_(false), surfaceSet_(false),
 trianglePattern3D_(3), renderer_(0)
{
  setSmooth(plot()->getSmooth());

  setFitX(plot()->getFitX());
  setFitY(plot()->getFitY());
}

CGnuPlotPlot::
~CGnuPlotPlot()
{
}

void
CGnuPlotPlot::
setRenderer(CGnuPlotRenderer *renderer)
{
  renderer_ = renderer;
}

CGnuPlot *
CGnuPlotPlot::
plot() const
{
  return window_->plot();
}

void
CGnuPlotPlot::
init()
{
  CGnuPlot *plot = this->plot();

  setFillStyle (plot->fillStyle ());
  setLineStyle (plot->lineStyle ());
  setPointStyle(plot->pointStyle());

  setLineStyleNum(plot->lineStyleNum());
  setHistogramStyle(plot->histogramStyle());

  setAxesData(plot->axesData());
  setKeyData (plot->keyData ());
  setMargin  (plot->margin  ());
  setPlotSize(plot->plotSize());

  xind_ = plot->xind();
  yind_ = plot->yind();

  setLogScaleMap(plot->logScaleMap());
  setPalette(plot->palette());
  setTrianglePattern3D(plot->trianglePattern3D());

  setArrows    (plot->arrows    ());
  setLabels    (plot->labels    ());
  setRectangles(plot->rectangles());
  setEllipses  (plot->ellipses  ());
  setPolygons  (plot->polygons  ());
}

void
CGnuPlotPlot::
addSubPlots(const Plots &plots)
{
  subPlots_.clear();

  for (auto plot : plots) {
    plot->setInd(subPlots_.size() + 1);

    subPlots_.push_back(plot);

    plot->parentPlot_ = this;
  }
}

void
CGnuPlotPlot::
clearPoints()
{
  if (plot()->isDebug())
    std::cerr << "Clear Points" << std::endl;

  points2D_.clear();
  points3D_.clear();

  reset3D();
}

void
CGnuPlotPlot::
addPoint2D(double x, double y)
{
  assert(! is3D_);

  std::vector<CExprValuePtr> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));

  addPoint2D(values, false);
}

void
CGnuPlotPlot::
addPoint2D(const Values &values, bool discontinuity)
{
  assert(! is3D_);

  if (plot()->isDebug()) {
    std::cerr << "Add Point [" << points2D_.size() << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points2D_.push_back(Point(values, discontinuity));
}

void
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, double z)
{
  assert(is3D_);

  std::vector<CExprValuePtr> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));
  values.push_back(CExprInst->createRealValue(z));

  if (plot()->isDebug()) {
    std::cerr << "Add Point [" << points3D_[iy].size() << "," << iy << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points3D_[iy].push_back(Point(values, false));

  reset3D();
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
fit()
{
  if (! getFitX() && ! getFitY())
    return;

  if (! subPlots_.empty()) {
    COptReal xmin, ymin, xmax, ymax, xmin2, ymin2, xmax2, ymax2;

    for (auto plot : subPlots_) {
      xmin .updateMin(plot->axesData_.xaxis .min); xmax .updateMax(plot->axesData_.xaxis .max);
      ymin .updateMin(plot->axesData_.yaxis .min); ymax .updateMax(plot->axesData_.yaxis .max);

      xmin2.updateMin(plot->axesData_.x2axis.min); xmax2.updateMax(plot->axesData_.x2axis.max);
      ymin2.updateMin(plot->axesData_.y2axis.min); ymax2.updateMax(plot->axesData_.y2axis.max);
    }

    axesData_.xaxis .min = xmin ; axesData_.xaxis .max = xmax ;
    axesData_.yaxis .min = ymin ; axesData_.yaxis .max = ymax ;
    axesData_.x2axis.min = xmin2; axesData_.x2axis.max = xmax2;
    axesData_.y2axis.min = ymin2; axesData_.y2axis.max = ymax2;

    for (auto plot : subPlots_)
      plot->axesData_= axesData_;
  }
  else {
    AxisData &xaxis = (xind_ == 2 ? axesData_.x2axis : axesData_.xaxis);
    AxisData &yaxis = (yind_ == 2 ? axesData_.y2axis : axesData_.yaxis);

    COptReal xmin, ymin, zmin, xmax, ymax, zmax;

    if (! is3D_) {
      uint np = numPoints2D();

      for (uint i = 0; i < np; ++i) {
        Point p = getPoint2D(i);

        double x, y;

        if (p.getX(x)) {
          xmin.updateMin(x); xmax.updateMax(x);
        }

        if (p.getY(y)) {
          ymin.updateMin(y); ymax.updateMax(y);
        }
      }
    }
    else {
      std::pair<int,int> np = numPoints3D();

      for (int iy = 0; iy < np.second; ++iy) {
        for (int ix = 0; ix < np.first; ++ix) {
          const Point &point = getPoint3D(ix, iy);

          double x = point.getX();
          double y = point.getY();
          double z = point.getZ();

          xmin.updateMin(x); xmax.updateMax(x);
          ymin.updateMin(y); ymax.updateMax(y);
          zmin.updateMin(z); zmax.updateMax(z);
        }
      }
    }

    if (getFitX()) {
      if (! xaxis.min.isValid() && xmin.isValid())
        xaxis.min = xmin;

      if (! xaxis.max.isValid() && xmax.isValid())
        xaxis.max = xmax;
    }

    if (getFitY()) {
      if (! yaxis.min.isValid() && ymin.isValid())
        yaxis.min = ymin;

      if (! yaxis.max.isValid() && ymax.isValid())
        yaxis.max = ymax;
    }
  }
}

void
CGnuPlotPlot::
smooth()
{
  if (! is3D_) {
    if (smooth_ == CGnuPlot::Smooth::UNIQUE) {
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

        addPoint2D(x, y);
      }
    }
  }
}

const CGnuPlot::AxesData &
CGnuPlotPlot::
axesData() const
{
  if (! subPlots_.empty()) {
    if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS)
      return axesData_;
    else
      return subPlots_[0]->axesData();
  }
  else
    return axesData_;
}

void
CGnuPlotPlot::
setAxesData(const AxesData &a)
{
  if (! subPlots_.empty()) {
    if (getStyle() != CGnuPlot::PlotStyle::HISTOGRAMS) {
      for (auto plot : subPlots_)
        plot->setAxesData(a);
    }
  }

  axesData_ = a;
}

void
CGnuPlotPlot::
showXAxis(bool show)
{
  axesData_.xaxis.displayed = show;

  stateChanged(CGnuPlot::ChangeState::AXIS_DISPLAY);
}

void
CGnuPlotPlot::
showYAxis(bool show)
{
  axesData_.yaxis.displayed = show;

  stateChanged(CGnuPlot::ChangeState::AXIS_DISPLAY);
}

const CGnuPlot::KeyData &
CGnuPlotPlot::
rootKeyData() const
{
  if (parentPlot_)
    return parentPlot_->keyData_;
  else
    return keyData_;
}

void
CGnuPlotPlot::
setRootKeyData(const KeyData &k)
{
  if (parentPlot_)
    parentPlot_->keyData_ = k;
  else
    keyData_ = k;
}

const CGnuPlot::KeyData &
CGnuPlotPlot::
keyData() const
{
  if (! subPlots_.empty()) {
    if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS)
      return keyData_;
    else
      return subPlots_[0]->keyData();
  }
  else
    return keyData_;
}

void
CGnuPlotPlot::
setKeyData(const KeyData &k)
{
  if (! subPlots_.empty()) {
    if (getStyle() != CGnuPlot::PlotStyle::HISTOGRAMS) {
      for (auto plot : subPlots_)
        plot->setKeyData(k);
    }
  }

  keyData_ = k;
}

const CBBox2D &
CGnuPlotPlot::
region() const
{
  if (! subPlots_.empty()) {
    if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS)
      return region_;
    else
      return subPlots_[0]->region();
  }
  else
    return region_;
}

void
CGnuPlotPlot::
setRegion(const CBBox2D &r)
{
  if (! subPlots_.empty()) {
    if (getStyle() != CGnuPlot::PlotStyle::HISTOGRAMS) {
      for (auto plot : subPlots_)
        plot->setRegion(r);
    }
  }

  region_ = r;
}

const CRange2D &
CGnuPlotPlot::
margin() const
{
  if (! subPlots_.empty()) {
    if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS)
      return margin_;
    else
      return subPlots_[0]->margin();
  }
  else
    return margin_;
}

void
CGnuPlotPlot::
setMargin(const CRange2D &m)
{
  if (! subPlots_.empty()) {
    if (getStyle() != CGnuPlot::PlotStyle::HISTOGRAMS) {
      for (auto plot : subPlots_)
        plot->setMargin(m);
    }
  }

  margin_ = m;
}

const CGnuPlot::PlotSize &
CGnuPlotPlot::
plotSize() const
{
  if (! subPlots_.empty()) {
    if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS)
      return plotSize_;
    else
      return subPlots_[0]->plotSize();
  }
  else
    return plotSize_;
}

void
CGnuPlotPlot::
setPlotSize(const PlotSize &s)
{
  if (! subPlots_.empty()) {
    if (getStyle() != CGnuPlot::PlotStyle::HISTOGRAMS) {
      for (auto plot : subPlots_)
        plot->setPlotSize(s);
    }
  }

  plotSize_ = s;
}

void
CGnuPlotPlot::
draw()
{
  renderer_->setMargin(margin());
  renderer_->setRange(getDisplayRange());

  if (plotSize().ratio.isValid())
    renderer_->setRatio(plotSize().ratio.getValue());
  else
    renderer_->unsetRatio();

  // root plot with children
  if (! subPlots_.empty()) {
    if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS)
      drawHistogram(subPlots_);
    else {
      for (auto plot : subPlots_) {
        plot->setRenderer(renderer_);

        plot->draw();
      }

      drawAxes();
    }
  }
  // normal plot (root with no children or child)
  else {
    if      (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS) {
      if (! parentPlot_)
        drawHistogram({ this });
    }
    else if (getStyle() == CGnuPlot::PlotStyle::TEST_TERMINAL)
      drawTerminal();
    else if (getStyle() == CGnuPlot::PlotStyle::TEST_PALETTE)
      drawPalette();
    else {
      if (is3D_)
        draw3D();
      else {
        draw2D();

        if (! parentPlot_)
          drawAxes();
      }
    }

    drawAnnotations();
  }

  drawKey();
}

void
CGnuPlotPlot::
drawHistogram(const Plots &plots)
{
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

  ymin = 0.0; // TODO: needed

  //---

  uint numPlots = plots.size();

  uint numPoints = 0;

  for (auto plot : plots)
    numPoints = std::max(numPoints, plot->numPoints2D());

  if (numPoints == 0) return;

  //---

  double xb = pixelWidthToWindowWidth(1);
  double xf = 1.0;

  if      (getHistogramStyle() == CGnuPlot::HistogramStyle::CLUSTERED) {
    CBBox2D range(xmin - 1, ymin, xmax + 1, ymax);

    renderer_->setRange(range);

    axesData_.yaxis.max.setValue(ymin);
    axesData_.yaxis.max.setValue(ymax);

    double w = xf*(xmax - xmin)/(numPoints*numPlots) - 2*xb;

    for (auto plot : plots) {
      CGnuPlot::LineStyle lineStyle = plot->calcLineStyle();

      FillType    fillType    = plot->fillType();
      FillPattern fillPattern = plot->fillPattern();

      //---

      double d = (plot->ind() - 1 - numPlots/2.0)*w;

      for (auto point : plot->getPoints2D()) {
        double x, y;

        if (! point.getX(x) || ! point.getY(y))
          continue;

        CBBox2D bbox(x + d, ymin, x + d + w, y);

        CRGBA c = (fillType == CGnuPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(255,255,255));

        drawBar(x, y, bbox, fillType, fillPattern, lineStyle.color(c),
                lineStyle.color(CRGBA(0,0,0)));
      }
    }
  }
  else if (getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED) {
    ymin = 0;
    ymax = 0;

    for (auto plot : plots) {
      double ymin1, ymax1;

      plot->getYRange(&ymin1, &ymax1);

      ymax += ymax1;
    }

    CBBox2D range(xmin - 1, ymin, xmax + 1, ymax);

    axesData_.yaxis.max.setValue(ymin);
    axesData_.yaxis.max.setValue(ymax);

    //---

    double w = (xmax - xmin)/(numPoints - 1);
    double x = xmin - w/2.0;

    for (uint i = 0; i < numPoints; ++i) {
      double h = 0;

      for (auto plot : plots) {
        CGnuPlot::LineStyle lineStyle = plot->calcLineStyle();

        FillPattern fillPattern = plot->fillPattern();
        FillType    fillType    = plot->fillType();

        //---

        const CGnuPlotPlot::Point &point = plot->getPoint2D(i);

        double y;

        if (! point.getY(y))
          continue;

        CBBox2D bbox(x, h, x + w, h + y);

        CRGBA c = (fillType == CGnuPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(255,255,255));

        drawBar(x, y, bbox, fillType, fillPattern, lineStyle.color(c),
                lineStyle.color(CRGBA(0,0,0)));

        h += y;
      }

      x += w;
    }
  }

  drawAxes();
}

void
CGnuPlotPlot::
drawBar(double /*x*/, double /*y*/, const CBBox2D &bbox, FillType fillType,
        FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor)
{
  if      (fillType == CGnuPlot::FillType::PATTERN)
    renderer_->patternRect(bbox, fillPattern, fillColor);
  else if (fillType == CGnuPlot::FillType::SOLID)
    renderer_->fillRect(bbox, fillColor);

  renderer_->drawRect(bbox, lineColor);
}

void
CGnuPlotPlot::
draw3D()
{
  assert(is3D_);

  const CGnuPlot::Camera &camera = window()->camera();

  std::pair<int,int> np = numPoints3D();

  //---

  if (window_->contour3D()) {
    if (! contourSet_) {
      std::vector<double> xc, yc, zc;

      xc.resize(np.first);
      yc.resize(np.second);
      zc.resize(np.first*np.second);

      for (int iy = 0; iy < np.second; ++iy) {
        for (int ix = 0; ix < np.first; ++ix) {
          const Point &point = getPoint3D(ix, iy);

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

  if (window()->surface3D() || window()->pm3D()) {
    if (! surfaceSet_) {
      surface_.clear();

      surfaceZMin_.setInvalid();
      surfaceZMax_.setInvalid();

      for (int iy = 1; iy < np.second; ++iy) {
        for (int ix = 1; ix < np.first; ++ix) {
          const Point &point1 = getPoint3D(ix - 1, iy - 1);
          const Point &point2 = getPoint3D(ix - 1, iy    );
          const Point &point3 = getPoint3D(ix    , iy    );
          const Point &point4 = getPoint3D(ix    , iy - 1);

          double z1 = CGnuPlotUtil::avg({point1.getZ(), point2.getZ(),
                                         point3.getZ(), point4.getZ()});

          surfaceZMin_.updateMin(z1);
          surfaceZMax_.updateMax(z1);

          CPoint3D p1(0,0,0), p2(0,0,0), p3(0,0,0), p4(0,0,0);

          double x, y, z;

          if (point1.getX(x) && point1.getY(y) && point1.getZ(z))
            p1 = camera.transform(CPoint3D(x, y, z));
          if (point2.getX(x) && point2.getY(y) && point2.getZ(z))
            p2 = camera.transform(CPoint3D(x, y, z));
          if (point3.getX(x) && point3.getY(y) && point3.getZ(z))
            p3 = camera.transform(CPoint3D(x, y, z));
          if (point4.getX(x) && point4.getY(y) && point4.getZ(z))
            p4 = camera.transform(CPoint3D(x, y, z));

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
          renderer_->fillPolygon(poly.second, CRGBA(0.8,0.8,0.8));
          renderer_->drawPolygon(poly.second, 0, CRGBA(0,0,0));
        }
      }
    }
    else {
      int pattern = trianglePattern3D();

      for (auto polys : surface_) {
        for (auto poly : polys.second) {
          if (pattern & 1) renderer_->drawLine(poly.second[0], poly.second[1]);
          if (pattern & 2) renderer_->drawLine(poly.second[0], poly.second[3]);
          if (pattern & 4) renderer_->drawLine(poly.second[0], poly.second[2]);
        }
      }
    }
  }

  if (window()->pm3D()) {
    for (auto polys : surface_) {
      for (auto poly : polys.second) {
        double z = poly.first;

        double r = (                      z - surfaceZMin_.getValue())/
                   (surfaceZMax_.getValue() - surfaceZMin_.getValue());

        CRGBA c = palette().getColor(r);

        renderer_->fillPolygon(poly.second, c);
      }
    }
  }
}

void
CGnuPlotPlot::
draw2D()
{
  assert(! is3D_);

  if (clearRect_.isValid()) {
    CPoint2D p1 = pixelToWindow(CPoint2D(0,0));
    CPoint2D p2 = pixelToWindow(CPoint2D(renderer_->pixelWidth () - 1,
                                         renderer_->pixelHeight() - 1));

    double xmin = (p2.x - p1.x)*clearRect_.getValue().getLeft  () + p1.x;
    double ymin = (p2.y - p1.y)*clearRect_.getValue().getBottom() + p1.y;
    double xmax = (p2.x - p1.x)*clearRect_.getValue().getRight () + p1.x;
    double ymax = (p2.y - p1.y)*clearRect_.getValue().getTop   () + p1.y;

    renderer_->fillRect(CBBox2D(xmin, ymin, xmax, ymax), CRGBA(1,1,1));
  }

  renderer_->setRegion(region());

  //---

  const AxisData &xaxis = (xind_ == 2 ? axesData().x2axis : axesData().xaxis);
  const AxisData &yaxis = (yind_ == 2 ? axesData().y2axis : axesData().yaxis);

  double xmin = xaxis.min.getValue(0.0);
  double ymin = yaxis.min.getValue(0.0);
  double xmax = xaxis.max.getValue(1.0);
  double ymax = yaxis.max.getValue(1.0);

  clip_ = CBBox2D(xmin, ymin, xmax, ymax);

  //---

  CGnuPlot::LineStyle lineStyle = calcLineStyle();

  uint np = numPoints2D();

  if (getSmooth() == CGnuPlot::Smooth::BEZIER) {
    for (uint i1 = 0, i2 = 1, i3 = 2, i4 = 3; i4 < np;
           i1 = i4, i2 = i1 + 1, i3 = i2 + 1, i4 = i3 + 1) {
      const Point &point1 = getPoint2D(i1);
      const Point &point2 = getPoint2D(i2);
      const Point &point3 = getPoint2D(i3);
      const Point &point4 = getPoint2D(i4);

      double x1, y1, x2, y2, x3, y3, x4, y4;

      if (! point1.getX(x1) || ! point1.getY(y1) || ! point2.getX(x2) || ! point2.getY(y2) ||
          ! point3.getX(x3) || ! point3.getY(y3) || ! point4.getX(x4) || ! point4.getY(y4))
        continue;

      renderer_->drawBezier(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3), CPoint2D(x4, y4));
    }
  }
  else if (style_ == CGnuPlot::PlotStyle::LABELS) {
    for (uint i = 0; i < np; ++i) {
      const Point &point = getPoint2D(i);

      double x, y;

      if (! point.getX(x) || ! point.getY(y))
        continue;

      std::string str;

      if (! point.getValue(3, str))
        continue;

      renderer_->drawText(CPoint2D(x, y), str);
    }
  }
  else {
    if (style_ == CGnuPlot::PlotStyle::LINES || style_ == CGnuPlot::PlotStyle::LINES_POINTS) {
      for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
        const Point &point1 = getPoint2D(i1);
        const Point &point2 = getPoint2D(i2);

        double x1, y1, x2, y2;

        if (! point1.getX(x1) || ! point1.getY(y1) || ! point2.getX(x2) || ! point2.getY(y2))
          continue;

        drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2),
                 lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));
      }
    }

    if (style_ == CGnuPlot::PlotStyle::DOTS) {
      for (uint i = 0; i < np; ++i) {
        const Point &point = getPoint2D(i);

        double x, y;

        if (! point.getX(x) || ! point.getY(y))
          continue;

        renderer_->drawPoint(CPoint2D(x, y));
      }
    }

    if (style_ == CGnuPlot::PlotStyle::POINTS || style_ == CGnuPlot::PlotStyle::LINES_POINTS) {
      for (uint i = 0; i < np; ++i) {
        const Point &point = getPoint2D(i);

        double x, y;

        if (! point.getX(x) || ! point.getY(y))
          continue;

        double size = pointSize();

        if (pointStyle().varSize()) {
          if (! point.getValue(3, size))
            size = 1;
        }

        mapLogPoint(&x, &y);

        renderer_->drawSymbol(CPoint2D(x, y), pointType(), size,
                              lineStyle.color(CRGBA(1,0,0)));
      }
    }
  }
}

void
CGnuPlotPlot::
drawAxes()
{
  assert(! parentPlot_);

  std::set<int> xSet, ySet;

  if (! subPlots_.empty()) {
    for (auto plot : subPlots_) {
      xSet.insert(plot->xind_);
      ySet.insert(plot->yind_);
    }
  }
  else {
    xSet.insert(1);
    ySet.insert(1);
  }

  drawBorder();

  for (const auto &xi : xSet)
    drawXAxes(xi, xi == 1 && xSet.find(2) == xSet.end());

  for (const auto &yi : ySet)
    drawYAxes(yi, yi == 1 && ySet.find(2) == ySet.end());
}

void
CGnuPlotPlot::
drawBorder()
{
  if (axesData().borders) {
    CBBox2D bbox = getDisplayRange();

    renderer_->setRange(bbox);

    double xmin1 = bbox.getLeft  ();
    double ymin1 = bbox.getBottom();
    double xmax1 = bbox.getRight ();
    double ymax1 = bbox.getTop   ();

    double bw = axesData().borderWidth;

    if (axesData().borders & (1<<0))
      renderer_->drawLine(CPoint2D(xmin1, ymin1), CPoint2D(xmax1, ymin1), bw);

    if (axesData().borders & (1<<1))
      renderer_->drawLine(CPoint2D(xmin1, ymax1), CPoint2D(xmax1, ymax1), bw);

    if (axesData().borders & (1<<2))
      renderer_->drawLine(CPoint2D(xmin1, ymin1), CPoint2D(xmin1, ymax1), bw);

    if (axesData().borders & (1<<3))
      renderer_->drawLine(CPoint2D(xmax1, ymin1), CPoint2D(xmax1, ymax1), bw);
  }
}

void
CGnuPlotPlot::
drawXAxes(int xind, bool drawOther)
{
  AxisData &xaxis = (xind == 2 ? axesData_.x2axis : axesData_.xaxis);
  AxisData &yaxis = axesData_.yaxis;

  double xmin = xaxis.min.getValue(0.0);
  double xmax = xaxis.max.getValue(1.0);

  double ymin = yaxis.min.getValue(0.0);
  double ymax = yaxis.max.getValue(1.0);

  CGnuPlotAxis plotXAxis(this, CORIENTATION_HORIZONTAL);
  CGnuPlotAxis plotYAxis(this, CORIENTATION_VERTICAL);

  plotXAxis.setRange(xmin, xmax);
  plotYAxis.setRange(ymin, ymax);

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
  double ymin1 = plotYAxis.getStart();
  double ymax1 = plotYAxis.getEnd  ();

  if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS) {
    plotXAxis.setMajorIncrement(1);

    if (getHistogramStyle() == CGnuPlot::HistogramStyle::CLUSTERED ||
        getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    if (getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED)
      ymin1 = 0;

    plotXAxis.setDrawMinorTickMark(false);
  }

  renderer_->setRange(CBBox2D(xmin1, ymin1, xmax1, ymax1));

  if (xaxis.displayed) {
    plotXAxis.setLabel(xaxis.str);

    if (getLogScale(CGnuPlot::LogScale::X))
      plotXAxis.setLogarithmic(getLogScale(CGnuPlot::LogScale::X));
    else
      plotXAxis.resetLogarithmic();

    plotXAxis.setFont(renderer_->getFont());
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
CGnuPlotPlot::
drawYAxes(int yind, bool drawOther)
{
  AxisData &yaxis = (yind == 2 ? axesData_.y2axis : axesData_.yaxis);
  AxisData &xaxis = axesData_.xaxis;

  double xmin = xaxis.min.getValue(0.0);
  double xmax = xaxis.max.getValue(1.0);

  double ymin = yaxis.min.getValue(0.0);
  double ymax = yaxis.max.getValue(1.0);

  CGnuPlotAxis plotXAxis(this, CORIENTATION_HORIZONTAL);
  CGnuPlotAxis plotYAxis(this, CORIENTATION_VERTICAL);

  plotXAxis.setRange(xmin, xmax);
  plotYAxis.setRange(ymin, ymax);

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
  double ymin1 = plotYAxis.getStart();
  double ymax1 = plotYAxis.getEnd  ();

  if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS) {
    if (getHistogramStyle() == CGnuPlot::HistogramStyle::CLUSTERED ||
        getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    if (getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED)
      ymin1 = 0;

    plotYAxis.setDrawMinorTickMark(false);
  }

  renderer_->setRange(CBBox2D(xmin1, ymin1, xmax1, ymax1));

  if (yaxis.displayed) {
    plotYAxis.setLabel(yaxis.str);

    if (getLogScale(CGnuPlot::LogScale::Y))
      plotYAxis.setLogarithmic(getLogScale(CGnuPlot::LogScale::Y));
    else
      plotYAxis.resetLogarithmic();

    plotYAxis.setFont(renderer_->getFont());
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

CGnuPlot::LineStyle
CGnuPlotPlot::
calcLineStyle() const
{
  if (lineStyleNum().isValid())
    return plot()->lineStyles(lineStyleNum().getValue());
  else
    return lineStyle();
}

void
CGnuPlotPlot::
drawTerminal()
{
  static double grid_dashes[]   = {1, 3};
  static uint   num_grid_dashes = 2;

  //---

  renderer_->setRegion(region());

  double px1, py1, px2, py2;

  windowToPixel(0.0, 0.0, &px1, &py1);
  windowToPixel(1.0, 1.0, &px2, &py2);

  double pxm = CGnuPlotUtil::avg({px1, px2});
  double pym = CGnuPlotUtil::avg({py1, py2});

  double pw = px2 - px1;
  double ph = py1 - py2;

  CFontPtr font = renderer_->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  //---

  renderer_->setLineDash(CLineDash(grid_dashes, num_grid_dashes));

  renderer_->drawLine(CPoint2D(0.5, 0.0), CPoint2D(0.5, 1.0));
  renderer_->drawLine(CPoint2D(0.0, 0.5), CPoint2D(1.0, 0.5));

  renderer_->setLineDash(CLineDash());

  //---

  drawHAlignedText(pixelToWindow(CPoint2D(px1 + 4, py2 + 4)), CHALIGN_TYPE_LEFT, 0,
                   CVALIGN_TYPE_TOP, 0, "terminal test", CRGBA(1,0,0));

  //---

  // symbols on right
  int    nlines = int(ph/font_size);
  double dy     = 1.0/nlines;

  for (int i = 0; i < nlines; ++i) {
    std::string str = CStrUtil::toString(i - 1);

    double w = font->getStringWidth(str);

    CRGBA c = CGnuPlotStyle::instance()->indexColor(i - 1);

    double x, y;

    pixelToWindow(px2 - 48 - w, py2 + font->getCharAscent(), &x, &y);

    renderer_->drawText(CPoint2D(x, y - i*dy), str, c);

    double x1, y1, x2, y2;

    pixelToWindow(px2 - 48, py2 + font->getCharAscent()/2, &x1, &y1);
    pixelToWindow(px2 - 24, py2 + font->getCharAscent()/2, &x2, &y2);

    renderer_->drawLine(CPoint2D(x1, y1 - i*dy), CPoint2D(x2, y1 - i*dy), 0, c);

    pixelToWindow(px2 - 16, py2 + font->getCharAscent()/2, &x1, &y1);

    renderer_->drawSymbol(CPoint2D(x1, y1 - i*dy),
                          CGnuPlotStyle::instance()->indexSymbol(i - 1), 1.0, c);
  }

  //---

  // text in center
  drawHAlignedText(pixelToWindow(CPoint2D(pxm, pym - 5*font_size)), CHALIGN_TYPE_LEFT, 0,
                   CVALIGN_TYPE_CENTER, 0, "left justified");
  drawHAlignedText(pixelToWindow(CPoint2D(pxm, pym - 4*font_size)), CHALIGN_TYPE_CENTER, 0,
                   CVALIGN_TYPE_CENTER, 0, "centre+d text");
  drawHAlignedText(pixelToWindow(CPoint2D(pxm, pym - 3*font_size)), CHALIGN_TYPE_RIGHT, 0,
                   CVALIGN_TYPE_CENTER, 0, "right justified");

  drawHAlignedText(pixelToWindow(CPoint2D(pxm, pym - font_size)), CHALIGN_TYPE_CENTER, 0,
                   CVALIGN_TYPE_CENTER, 0, "test of character width",
                   CGnuPlotStyle::instance()->indexColor(4));
  drawHAlignedText(CPoint2D(0.5, 0.5), CHALIGN_TYPE_CENTER, 0,
                   CVALIGN_TYPE_CENTER, 0, "12345678901234567890",
                   CGnuPlotStyle::instance()->indexColor(4));

  double w = font->getStringWidth("12345678901234567890");

  double x1, y1, x2, y2;

  pixelToWindow(pxm - w/2, pym + font_size/2, &x1, &y1);
  pixelToWindow(pxm + w/2, pym - font_size/2, &x2, &y2);

  renderer_->drawRect(CBBox2D(x1, y1, x2, y2), CGnuPlotStyle::instance()->indexColor(4));

  //---

  // rotated text on left
  drawVAlignedText(pixelToWindow(CPoint2D(px1 + 1*font_size, pym)), CHALIGN_TYPE_CENTER, 0,
                   CVALIGN_TYPE_CENTER, 0, "rotated ce+ntered text", CRGBA(0,1,0));

  drawRotatedText(pixelToWindow(CPoint2D(px1 + 2*font_size, pym)),
                  "rotated by +45 deg"    , CRGBA(0,1,0),  45);
  drawRotatedText(pixelToWindow(CPoint2D(px1 + 3*font_size, pym)),
                  "rotated by -45 deg"    , CRGBA(0,1,0), -45);

  //---

  // filled polygons
  double pl = pw/16;

  CPoint2D pp1 = CPoint2D(pxm + pw/4, pym - ph/4);
  CPoint2D pp2 = pp1 + CPoint2D(pl, pl);

  drawHAlignedText(pixelToWindow(pp1 - CPoint2D(0, pl)), CHALIGN_TYPE_CENTER, 0,
                   CVALIGN_TYPE_BOTTOM, 0, "filled polygons:");

  std::vector<CPoint2D> points1, points2;

  for (int i = 0; i < 6; ++i) {
    double a = Deg2Rad(i*60);

    points1.push_back(pixelToWindow(pp1 + CPoint2D(pl*cos(a), pl*sin(a))));
    points2.push_back(pixelToWindow(pp2 + CPoint2D(pl*cos(a), pl*sin(a))));
  }

  renderer_->fillPolygon(points1, CRGBA(0,0,1));
  renderer_->fillPolygon(points2, CRGBA(0,1,0,0.5));

  //---

  // arrows
  CPoint2D ac = pixelToWindow(CPoint2D(pxm - 100, pym + 100));
  double   al = 50;

  for (int i = 0; i < 8; ++i) {
    CGnuPlot::Arrow arrow;

    double a = i*Deg2Rad(45);

    double dx = al*cos(a);
    double dy = -al*sin(a);

    CPoint2D ac1 = pixelToWindow(CPoint2D(pxm - 100 + dx, pym + 100 + dy));

    arrow.from = ac;
    arrow.to   = ac1;

    arrow.length = pixelWidthToWindowWidth(al/5);
    arrow.angle  = 30;

    arrow.fhead = (i == 7);
    arrow.thead = (i != 3 && i != 7);

    arrow.filled = (i == 2);
    arrow.empty  = (i == 1 || i == 4 || i == 5 || i == 6 || i == 7);

    arrow.c = CRGBA(1,0,0);

    drawArrow(arrow);
  }

  //---

  // pattern fill
  double ptw = pw/30;
  double pth = ph/8;
  double ptb = 4;

  for (int i = 0; i <= 9; ++i) {
    double px = pxm + i*(ptw + ptb);

    CPoint2D p1 = pixelToWindow(CPoint2D(px      , py1 - pth));
    CPoint2D p2 = pixelToWindow(CPoint2D(px + ptw, py1      ));

    renderer_->patternRect(CBBox2D(p1, p2), CGnuPlotStyle::instance()->indexPattern(i),
                           CRGBA(0,0,0), CRGBA(1,1,1));

    renderer_->drawRect(CBBox2D(p1, p2), CRGBA(0,0,0));

    drawHAlignedText(pixelToWindow(CPoint2D(px + ptw/2, py1 - pth)), CHALIGN_TYPE_CENTER, 0,
                     CVALIGN_TYPE_BOTTOM, 0, CStrUtil::strprintf("%d", i));
  }

  drawHAlignedText(pixelToWindow(CPoint2D(pxm + 4*(ptw + ptb), py1 - pth - font_size)),
                   CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0,
                   "pattern fill");

  //---

  // line width bottom left
  double ll  = pw/8;
  double lb1 = 8;
  double lb2 = 8;

  for (int i = 1; i <= 6; ++i) {
    renderer_->drawLine(pixelToWindow(CPoint2D(px1 + lb1     , py1 - i*font_size)),
                        pixelToWindow(CPoint2D(px1 + lb1 + ll, py1 - i*font_size)), i);

    drawHAlignedText(pixelToWindow(CPoint2D(px1 + lb1 + lb2 + ll, py1 - i*font_size)),
                     CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_CENTER, 0,
                     CStrUtil::strprintf("lw %d", i));
  }

  drawHAlignedText(pixelToWindow(CPoint2D(px1 + lb1 + ll/2, py1 - 7*font_size - 4)),
                   CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP, 0,
                   "linewidth");
}

void
CGnuPlotPlot::
drawPalette()
{
  double px1, py1, px2, py2;

  windowToPixel(0.0, 0.0, &px1, &py1);
  windowToPixel(1.0, 1.0, &px2, &py2);

  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, py1 + 32, &wx1, &wy1);
  pixelToWindow(0, py1 + 64, &wx2, &wy2);

  bool   first = true;
  double r1, g1, b1, m1, x1;

  for (double i = px1; i <= px2; i += 1.0) {
    double wx, wy;

    pixelToWindow(i, 0, &wx, &wy);

    CRGBA c = palette_.getColor(wx);

    renderer_->drawLine(CPoint2D(wx, wy1), CPoint2D(wx, wy2), 0.0, c);

    //double x = (i - px1)/(px2 - px1);

    double x2 = wx;
    double r2 = c.getRed  ();
    double g2 = c.getGreen();
    double b2 = c.getBlue ();
    double m2 = c.getGray();

    if (! first) {
      renderer_->drawLine(CPoint2D(x1, r1), CPoint2D(x2, r2), 0, CRGBA(1,0,0));
      renderer_->drawLine(CPoint2D(x1, g1), CPoint2D(x2, g2), 0, CRGBA(0,1,0));
      renderer_->drawLine(CPoint2D(x1, b1), CPoint2D(x2, b2), 0, CRGBA(0,0,1));
      renderer_->drawLine(CPoint2D(x1, m1), CPoint2D(x2, m2), 0, CRGBA(0,0,0));
    }

    x1 = x2;
    r1 = r2;
    g1 = g2;
    b1 = b2;
    m1 = m2;

    first = false;
  }

  renderer_->drawRect(CBBox2D(0.0, wy1, 1.0, wy2), CRGBA(0,0,0));

#if 0
  CGnuPlotAxis xaxis(this, CORIENTATION_HORIZONTAL);
  CGnuPlotAxis yaxis(this, CORIENTATION_VERTICAL);

  xaxis.setFont(renderer_->getFont());
  yaxis.setFont(renderer_->getFont());

  xaxis.drawAxis(0.0);
  yaxis.drawAxis(0.0);

  xaxis.drawGrid(0.0, 1.0);
  yaxis.drawGrid(0.0, 1.0);
#endif
}

void
CGnuPlotPlot::
drawKey()
{
  if (! getKeyDisplayed()) return;

  CBBox2D bbox = getDisplayRange();

  renderer_->setRange(bbox);

  CFontPtr font = renderer_->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = pixelWidthToWindowWidth  (1);
  double ph = pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = 64*pw;

  CSize2D size;

  double textWidth = 0.0, textHeight = 0.0;

  if      (! subPlots_.empty()) {
    for (auto plot : subPlots_) {
      std::string label = plot->keyData_.title;

      textWidth = std::max(textWidth, font->getStringWidth(label)*pw);

      textHeight += font_size*ph;
    }
  }
  else {
    std::string label = keyData_.title;

    textWidth  = std::max(textWidth, font->getStringWidth(label)*pw);
    textHeight = font_size*ph;
  }

  size = CSize2D(textWidth + ll + 3*bx, textHeight + 2*by);

  CHAlignType halign = getKeyHAlign();
  CVAlignType valign = getKeyVAlign();

  double x1 = 0, y1 = 0;

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = bbox.getLeft () + bx;
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = bbox.getRight() - bx - size.width;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = bbox.getXMid() - size.width/2;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = bbox.getTop   () - by - size.height;
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = bbox.getBottom() + by;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = bbox.getYMid() - size.height/2;

  double x2 = x1 + size.width;
  double y2 = y1 + size.height;

  if      (! subPlots_.empty()) {
    if (keyData_.box) {
      CBBox2D bbox(x1, y1, x2, y2);

      renderer_->drawRect(bbox);
    }

    double y = y2 - by;

    for (auto plot : subPlots_) {
      CGnuPlot::PlotStyle style     = plot->getStyle();
      CGnuPlot::LineStyle lineStyle = plot->calcLineStyle();

      double xx = (keyData_.reverse ? x1 + bx : x2 - ll - bx);
      double yy = y - font_size*ph/2;

      if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS) {
        double h = (font_size - 4)*ph;

        CBBox2D bbox(xx, yy - h/2, xx + ll, yy + h/2);

        if      (plot->fillStyle().style() == CGnuPlot::FillType::PATTERN)
          renderer_->patternRect(bbox, plot->fillStyle().pattern(),
                                 lineStyle.color(CRGBA(0,0,0)));
        else if (plot->fillStyle().style() == CGnuPlot::FillType::SOLID)
          renderer_->fillRect(bbox, lineStyle.color(CRGBA(255,255,255)));

        renderer_->drawRect(bbox, lineStyle.color(CRGBA(0,0,0)));
      }
      else {
        if (style == CGnuPlot::PlotStyle::LINES || style == CGnuPlot::PlotStyle::LINES_POINTS)
          renderer_->drawLine(CPoint2D(xx, yy), CPoint2D(xx + ll, yy),
                              lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));

        if (style == CGnuPlot::PlotStyle::POINTS || style == CGnuPlot::PlotStyle::LINES_POINTS)
          renderer_->drawSymbol(CPoint2D(xx + ll/2, yy), plot->pointType(),
                                plot->pointSize(), lineStyle.color(CRGBA(1,0,0)));
      }

      std::string label = plot->keyData_.title;

      //double lw = font->getStringWidth(label);

      if (keyData_.reverse)
        drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                         CVALIGN_TYPE_TOP, 0, label);
      else
        drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                         CVALIGN_TYPE_TOP, 0, label);

      y -= font_size*ph;
    }
  }
  else if (! parentPlot_) {
    if (keyData_.box) {
      CBBox2D bbox(x1, y1, x2, y2);

      renderer_->drawRect(bbox);
    }

    double y = y2 - by;

    double xx = (keyData_.reverse ? x1 + bx : x2 - ll - bx);
    double yy = y - font_size*ph/2;

    CGnuPlot::LineStyle lineStyle = calcLineStyle();

    if (style_ == CGnuPlot::PlotStyle::LINES || style_ == CGnuPlot::PlotStyle::LINES_POINTS)
      renderer_->drawLine(CPoint2D(xx, yy), CPoint2D(xx + ll, yy),
                          lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));

    if (style_ == CGnuPlot::PlotStyle::POINTS || style_ == CGnuPlot::PlotStyle::LINES_POINTS)
      renderer_->drawSymbol(CPoint2D(xx + ll/2, yy), pointType(),
                            pointSize(), lineStyle.color(CRGBA(1,0,0)));

    std::string label = keyData_.title;

    //double lw = font->getStringWidth(label);

    if (keyData_.reverse)
      drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                       CVALIGN_TYPE_TOP, 0, label);
    else
      drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                       CVALIGN_TYPE_TOP, 0, label);
  }
}

void
CGnuPlotPlot::
drawAnnotations()
{
  for (auto arrow : arrows_)
    drawArrow(arrow);

  for (auto label : labels_)
    drawLabel(label);

  for (auto rect : rects_)
    drawRectangle(rect);

  for (auto ellipse : ellipses_)
    drawEllipse(ellipse);

  for (auto poly : polygons_)
    drawPolygon(poly);
}

void
CGnuPlotPlot::
drawArrow(const CGnuPlot::Arrow &arrow)
{
  CPoint2D from = arrow.from.p;
  CPoint2D to   = (arrow.relative ? arrow.from.p + arrow.to.p : arrow.to.p);

  double fx, fy, tx, ty;

  windowToPixel(from.x, from.y, &fx, &fy);
  windowToPixel(to  .x, to  .y, &tx, &ty);

  double w = (arrow.lineWidth > 0 ? arrow.lineWidth : 1);

  double a = atan2(ty - fy, tx - fx);

  double aa = Deg2Rad(arrow.angle > 0 ? arrow.angle : 30);

  double l  = (arrow.length > 0 ? renderer_->windowWidthToPixelWidth(arrow.length) : 16);
  double l1 = l*cos(aa);

  double c = cos(a), s = sin(a);

  double x1 = fx, y1 = fy;
  double x4 = tx, y4 = ty;

  double x2 = x1 + l1*c;
  double y2 = y1 + l1*s;
  double x3 = x4 - l1*c;
  double y3 = y4 - l1*s;

  double x11 = x1, y11 = y1;
  double x41 = x4, y41 = y4;

  if (arrow.fhead) {
    if (arrow.filled || arrow.empty) {
      x11 = x2;
      y11 = y2;
    }
    else {
      x11 = x1 + w*c;
      y11 = y1 + w*s;
    }
  }

  if (arrow.thead) {
    if (arrow.filled || arrow.empty) {
      x41 = x3;
      y41 = y3;
    }
    else {
      x41 = x4 - w*c;
      y41 = y4 - w*s;
    }
  }

  double ba = Deg2Rad(arrow.backAngle > 0 ? arrow.backAngle : 90);

  renderer_->setMapping(false);

  if (arrow.fhead) {
    double a1 = a + aa;
    double a2 = a - aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xf1 = x1 + l*c1;
    double yf1 = y1 + l*s1;
    double xf2 = x1 + l*c2;
    double yf2 = y1 + l*s2;

    double xf3 = x2, yf3 = y2;

    if (! arrow.filled && ! arrow.empty) {
      renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(xf1, yf1), w, arrow.c);
      renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(xf2, yf2), w, arrow.c);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x1, y1, x2, y2, xf1, yf1, xf1 - 10*c3, yf1 - 10*s3, &xf3, &yf3);

        x11 = xf3;
        y11 = yf3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x1 , y1 ));
      points.push_back(CPoint2D(xf1, yf1));
      points.push_back(CPoint2D(xf3, yf3));
      points.push_back(CPoint2D(xf2, yf2));

      if (arrow.filled)
        renderer_->fillPolygon(points, arrow.c);
      else
        renderer_->drawPolygon(points, w, arrow.c);
    }
  }

  if (arrow.thead) {
    double a1 = a + M_PI - aa;
    double a2 = a + M_PI + aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xt1 = x4 + l*c1;
    double yt1 = y4 + l*s1;
    double xt2 = x4 + l*c2;
    double yt2 = y4 + l*s2;

    double xt3 = x3, yt3 = y3;

    if (! arrow.filled && ! arrow.empty) {
      renderer_->drawLine(CPoint2D(x4, y4), CPoint2D(xt1, yt1), w, arrow.c);
      renderer_->drawLine(CPoint2D(x4, y4), CPoint2D(xt2, yt2), w, arrow.c);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + M_PI - ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x3, y3, x4, y4, xt1, yt1, xt1 - 10*c3, yt1 - 10*s3, &xt3, &yt3);

        x41 = xt3;
        y41 = yt3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x4 , y4 ));
      points.push_back(CPoint2D(xt1, yt1));
      points.push_back(CPoint2D(xt3, yt3));
      points.push_back(CPoint2D(xt2, yt2));

      if (arrow.filled)
        renderer_->fillPolygon(points, arrow.c);
      else
        renderer_->drawPolygon(points, w, arrow.c);
    }
  }

  renderer_->drawLine(CPoint2D(x11, y11), CPoint2D(x41, y41), w, arrow.c);

  renderer_->setMapping(true);
}

void
CGnuPlotPlot::
drawLabel(const CGnuPlot::Label &label)
{
  CVAlignType valign = (label.front ? CVALIGN_TYPE_TOP : CVALIGN_TYPE_CENTER);

  drawHAlignedText(label.pos.p, label.align, 0, valign, 0, label.text);
}

void
CGnuPlotPlot::
drawRectangle(const CGnuPlot::Rectangle &rect)
{
  renderer_->fillRect(CBBox2D(rect.from, rect.to), rect.fillColor);
  renderer_->drawRect(CBBox2D(rect.from, rect.to), rect.strokeColor);
}

void
CGnuPlotPlot::
drawEllipse(const CGnuPlot::Ellipse &ellipse)
{
  renderer_->fillEllipse(ellipse.p.p, ellipse.rx, ellipse.ry, ellipse.fillColor);
  renderer_->drawEllipse(ellipse.p.p, ellipse.rx, ellipse.ry, ellipse.strokeColor);
}

void
CGnuPlotPlot::
drawPolygon(const CGnuPlot::Polygon &poly)
{
  renderer_->fillPolygon(poly.points, poly.fillColor);
  renderer_->drawPolygon(poly.points, 1.0, poly.strokeColor);
}

void
CGnuPlotPlot::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str, const CRGBA &c)
{
  renderer_->drawHAlignedText(pos, halign, x_offset, valign, y_offset, str, c);
}

void
CGnuPlotPlot::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str, const CRGBA &c)
{
  renderer_->drawVAlignedText(pos, halign, x_offset, valign, y_offset, str, c);
}

void
CGnuPlotPlot::
drawRotatedText(const CPoint2D &p, const std::string &text, const CRGBA &c, double a)
{
  renderer_->drawRotatedText(p, text, c, a);
}

void
CGnuPlotPlot::
drawLine(const CPoint2D &p1, const CPoint2D &p2, double w, const CRGBA &c)
{
  double x1 = p1.x, y1 = p1.y;
  double x2 = p2.x, y2 = p2.y;

  if (CMathGeom2D::clipLine(clip_.getXMin(), clip_.getYMin(), clip_.getXMax(), clip_.getYMax(),
                            &x1, &y1, &x2, &y2))
    renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2), w, c);
}

CBBox2D
CGnuPlotPlot::
getDisplayRange() const
{
  double xmin, ymin, xmax, ymax;

  calcXRange(&xmin, &xmax);
  calcYRange(&ymin, &ymax);

  CGnuPlotAxis plotXAxis(const_cast<CGnuPlotPlot *>(this), CORIENTATION_HORIZONTAL);
  CGnuPlotAxis plotYAxis(const_cast<CGnuPlotPlot *>(this), CORIENTATION_VERTICAL);

  plotXAxis.setRange(xmin, xmax);
  plotYAxis.setRange(ymin, ymax);

  double xmin1 = plotXAxis.getStart();
  double xmax1 = plotXAxis.getEnd  ();
  double ymin1 = plotYAxis.getStart();
  double ymax1 = plotYAxis.getEnd  ();

  if (getStyle() == CGnuPlot::PlotStyle::HISTOGRAMS) {
    if (getHistogramStyle() == CGnuPlot::HistogramStyle::CLUSTERED ||
        getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED) {
      xmin1 -= 1;
      xmax1 += 1;
    }

    if (getHistogramStyle() == CGnuPlot::HistogramStyle::ROWSTACKED)
      ymin1 = 0;
  }

  return CBBox2D(xmin1, ymin1, xmax1, ymax1);
}

void
CGnuPlotPlot::
calcXRange(double *xmin, double *xmax) const
{
  const AxisData &xaxis = (xind_ == 2 ? axesData().x2axis : axesData().xaxis);

  if (! xaxis.min.isValid() || ! xaxis.max.isValid()) {
    double xmin1, xmax1;

    getXRange(&xmin1, &xmax1);

    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    AxisData &xaxis = (xind_ == 2 ? th->axesData_.x2axis : th->axesData_.xaxis);

    xaxis.min = xmin1;
    xaxis.max = xmax1;
  }

  *xmin = xaxis.min.getValue(-10);
  *xmax = xaxis.max.getValue( 10);
}

void
CGnuPlotPlot::
calcYRange(double *ymin, double *ymax) const
{
  const AxisData &yaxis = (yind_ == 2 ? axesData().y2axis : axesData().yaxis);

  if (! yaxis.min.isValid() || ! axesData().yaxis.max.isValid()) {
    double ymin1, ymax1;

    getYRange(&ymin1, &ymax1);

    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    AxisData &yaxis = (yind_ == 2 ? th->axesData_.y2axis : th->axesData_.yaxis);

    yaxis.min = ymin1;
    yaxis.max = ymax1;
  }

  *ymin = yaxis.min.getValue(-1);
  *ymax = yaxis.max.getValue( 1);
}

std::string
CGnuPlotPlot::
getXAxisValueStr(int i, double x) const
{
  const AxisData &xaxis = (xind_ == 2 ? axesData().x2axis : axesData().xaxis);

  auto pl = xaxis.ticlabel.find(i);

  if (pl != xaxis.ticlabel.end())
    return (*pl).second;

  return formatX(x);
}

std::string
CGnuPlotPlot::
getYAxisValueStr(int i, double y) const
{
  const AxisData &yaxis = (yind_ == 2 ? axesData().y2axis : axesData().yaxis);

  auto pl = yaxis.ticlabel.find(i);

  if (pl != yaxis.ticlabel.end())
    return (*pl).second;

  return formatY(y);
}

std::string
CGnuPlotPlot::
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
CGnuPlotPlot::
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

CPoint2D
CGnuPlotPlot::
windowToPixel(const CPoint2D &p) const
{
  double px, py;

  windowToPixel(p.x, p.y, &px, &py);

  return CPoint2D(px, py);
}

void
CGnuPlotPlot::
windowToPixel(double wx, double wy, double *px, double *py) const
{
  renderer_->setRegion(region());

  renderer_->windowToPixel(wx, wy, px, py);
}

CPoint2D
CGnuPlotPlot::
pixelToWindow(const CPoint2D &p) const
{
  double wx, wy;

  pixelToWindow(p.x, p.y, &wx, &wy);

  return CPoint2D(wx, wy);
}

void
CGnuPlotPlot::
pixelToWindow(double px, double py, double *wx, double *wy) const
{
  renderer_->setRegion(region());

  renderer_->pixelToWindow(px, py, wx, wy);
}

double
CGnuPlotPlot::
pixelWidthToWindowWidth(double w) const
{
  CPoint2D w1 = pixelToWindow(CPoint2D(0, 0));
  CPoint2D w2 = pixelToWindow(CPoint2D(w, w));

  return w2.x - w1.x;
}

double
CGnuPlotPlot::
pixelHeightToWindowHeight(double h) const
{
  CPoint2D w1 = pixelToWindow(CPoint2D(0, 0));
  CPoint2D w2 = pixelToWindow(CPoint2D(h, h));

  return w1.y - w2.y;
}

void
CGnuPlotPlot::
mapLogPoint(double *x, double *y) const
{
  int xbase = getLogScale(CGnuPlot::LogScale::X);
  int ybase = getLogScale(CGnuPlot::LogScale::Y);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);

  if (xbase > 1) *x = log(std::max(*x, 0.5))/xlogscale;
  if (ybase > 1) *y = log(std::max(*y, 0.5))/ylogscale;
}

void
CGnuPlotPlot::
unmapLogPoint(double *x, double *y) const
{
  int xbase = getLogScale(CGnuPlot::LogScale::X);
  int ybase = getLogScale(CGnuPlot::LogScale::Y);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);

  if (xbase > 1) *x = exp((*x)*xlogscale);
  if (ybase > 1) *y = exp((*y)*ylogscale);
}
