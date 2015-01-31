#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotAxis.h>
#include <CGnuPlotStyle.h>
#include <CGnuPlotUtil.h>

#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>
#include <CBoxWhisker.h>

int CGnuPlotPlot::nextId_ = 1;

CGnuPlotPlot::
CGnuPlotPlot(CGnuPlotGroup *group) :
 group_(group), id_(nextId_++), contour_(this)
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

void
CGnuPlotPlot::
init()
{
  CGnuPlot *plot = this->app();

  setBoxWidth(plot->getBoxWidth());

  setFillStyle (plot->fillStyle ());
  setPointStyle(plot->pointStyle());
  setArrowStyle(plot->arrowStyle());

  setAxesData(plot->axesData());

  xind_ = plot->xind();
  yind_ = plot->yind();

  setPalette(plot->palette());
  setFilledCurve(plot->filledCurve());
  setTrianglePattern3D(plot->trianglePattern3D());
  setWhiskerBars(plot->whiskerBars());

  setBinary(plot->isBinary());
  setMatrix(plot->isMatrix());
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

void
CGnuPlotPlot::
addPoint2D(double x, double y)
{
  assert(! is3D_);

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));

  addPoint2D(values, false);
}

void
CGnuPlotPlot::
addPoint2D(double x, CExprValueP y)
{
  assert(! is3D_);

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(y);

  addPoint2D(values, false);
}

void
CGnuPlotPlot::
addPoint2D(const Values &values, bool discontinuity)
{
  assert(! is3D_);

  if (app()->isDebug()) {
    std::cerr << "Add Point [" << points2D_.size() << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points2D_.push_back(CGnuPlotPoint(values, discontinuity));
}

void
CGnuPlotPlot::
addPoint3D(int iy, double x, double y, double z)
{
  assert(is3D_);

  std::vector<CExprValueP> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));
  values.push_back(CExprInst->createRealValue(z));

  if (app()->isDebug()) {
    std::cerr << "Add Point [" << points3D_[iy].size() << "," << iy << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points3D_[iy].push_back(CGnuPlotPoint(values, false));

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
  CGnuPlot::AxisData &xaxis = this->xaxis(xind_);
  CGnuPlot::AxisData &yaxis = this->yaxis(yind_);

  bool fitX = (! xaxis.min.isValid() || ! xaxis.max.isValid());
  bool fitY = (! yaxis.min.isValid() || ! yaxis.max.isValid());

  if (! fitX && ! fitY) return;

  if (! is3D_) {
    if (style_ == PlotStyle::PARALLELAXES) {
      int nc = 0;

      for (const auto &p : getPoints2D())
        nc = std::max(nc, p.getNumValues());

      if (lineStyle().colorSpec().isVariable())
        --nc;

      xaxis.min = 1;
      xaxis.max = nc;

      yaxis.min = 0;
      yaxis.max = 1;
    }

    if (! xaxis.min.isValid() || ! xaxis.max.isValid()) {
      if      (style_ == PlotStyle::BOXPLOT)
        calcBoxPlotRange();
      else if (isImageStyle()) {
        xaxis.min = 0;
        xaxis.max = imageStyle_.w - 1;
      }
      else {
        double dx = 0;

        if (style_ == PlotStyle::BOXES)
          dx = 1;

        COptReal xmin, xmax;

        for (const auto &p : getPoints2D()) {
          double x;

          if (p.getX(x)) {
            xmin.updateMin(x - dx);
            xmax.updateMax(x + dx);
          }
        }

        if (! xaxis.min.isValid()) xaxis.min = xmin;
        if (! xaxis.max.isValid()) xaxis.max = xmax;
      }
    }

    COptReal ymin, ymax;

    if (! yaxis.min.isValid() || ! yaxis.max.isValid()) {
      if (isImageStyle()) {
        yaxis.min = 0;
        yaxis.max = imageStyle_.h - 1;
      }
      else {
        for (const auto &p : getPoints2D()) {
          double x, y;

          if (p.getXY(x, y)) {
            if (x >= xaxis.min.getValue() && x <= xaxis.max.getValue()) {
              ymin.updateMin(y);
              ymax.updateMax(y);
            }
          }
        }

        if (! yaxis.min.isValid() && ymin.isValid()) yaxis.min = ymin;
        if (! yaxis.max.isValid() && ymax.isValid()) yaxis.max = ymax;
      }
    }
  }
  else {
    COptReal xmin, ymin, zmin, xmax, ymax, zmax;

    std::pair<int,int> np = numPoints3D();

    for (int iy = 0; iy < np.second; ++iy) {
      for (int ix = 0; ix < np.first; ++ix) {
        const CGnuPlotPoint &point = getPoint3D(ix, iy);

        double x = point.getX();
        double y = point.getY();
        double z = point.getZ();

        xmin.updateMin(x); xmax.updateMax(x);
        ymin.updateMin(y); ymax.updateMax(y);
        zmin.updateMin(z); zmax.updateMax(z);
      }
    }

    if (fitX) {
      if (! xaxis.min.isValid() && xmin.isValid()) xaxis.min = xmin;
      if (! xaxis.max.isValid() && xmax.isValid()) xaxis.max = xmax;
    }

    if (fitY) {
      if (! yaxis.min.isValid() && ymin.isValid()) yaxis.min = ymin;
      if (! yaxis.max.isValid() && ymax.isValid()) yaxis.max = ymax;
    }
  }
}

void
CGnuPlotPlot::
updateGroupRange()
{
  group_->fit();
}

void
CGnuPlotPlot::
calcBoxPlotRange()
{
  CGnuPlot::AxisData &xaxis = this->xaxis(xind_);

  double x;
  bool   discrete = false;

  for (const auto &point : getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double x1;

      if (point.getValue(1, x1))
        x = x1;

      if (point.getNumValues() >= 4)
        discrete = true;

      break;
    }
  }

  if (discrete) {
    typedef std::set<int>         IVals;
    typedef std::set<std::string> SVals;

    IVals  iyv;
    SVals  syv;

    for (const auto &point : getPoints2D()) {
      if (point.getNumValues() >= 4) {
        int         i;
        std::string s;

        if      (point.getValue(4, i))
          iyv.insert(i);
        else if (point.getValue(4, s))
          syv.insert(s);
      }
    }

    xaxis.min = 0;
    xaxis.max = iyv.size() + syv.size() + 1;
  }
  else {
    xaxis.min = x - 1;
    xaxis.max = x + 1;
  }
}

void
CGnuPlotPlot::
smooth()
{
  if (! is3D_) {
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

        addPoint2D(x, y);
      }
    }
  }
}

void
CGnuPlotPlot::
draw()
{
  if (! isDisplayed()) return;

  // normal plot (no children)
  if      (getStyle() == PlotStyle::TEST_TERMINAL)
    drawTerminal();
  else if (getStyle() == PlotStyle::TEST_PALETTE)
    drawPalette();
  else {
    if (is3D_)
      draw3D();
    else
      draw2D();
  }
}

void
CGnuPlotPlot::
drawBar(double /*x*/, double /*y*/, const CBBox2D &bbox, FillType fillType,
        FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  if      (fillType == FillType::PATTERN)
    renderer->patternRect(bbox, fillPattern, fillColor);
  else if (fillType == FillType::SOLID)
    renderer->fillRect(bbox, fillColor);

  renderer->drawRect(bbox, lineColor);
}

void
CGnuPlotPlot::
draw3D()
{
  assert(is3D_);

  CGnuPlotRenderer *renderer = app()->renderer();

  const CGnuPlot::Camera &camera = window()->camera();

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

  if (window()->surface3D() || window()->pm3D()) {
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
          renderer->fillPolygon(poly.second, CRGBA(0.8,0.8,0.8));
          renderer->drawPolygon(poly.second, 0, CRGBA(0,0,0));
        }
      }
    }
    else {
      int pattern = trianglePattern3D();

      for (auto polys : surface_) {
        for (auto poly : polys.second) {
          if (pattern & 1) renderer->drawLine(poly.second[0], poly.second[1]);
          if (pattern & 2) renderer->drawLine(poly.second[0], poly.second[3]);
          if (pattern & 4) renderer->drawLine(poly.second[0], poly.second[2]);
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

        renderer->fillPolygon(poly.second, c);
      }
    }
  }
}

void
CGnuPlotPlot::
draw2D()
{
  assert(! is3D_);

  CGnuPlotRenderer *renderer = app()->renderer();

  if (clearRect_.isValid()) {
    const CISize2D &s = window()->size();

    const CBBox2D &clearRect = this->clearRect();

    CPoint2D p1 = group()->pixelToWindow(CPoint2D(0           , 0           ));
    CPoint2D p2 = group()->pixelToWindow(CPoint2D(s.width  - 1, s.height - 1));

    double xmin = (p2.x - p1.x)*clearRect.getLeft  () + p1.x;
    double ymin = (p2.y - p1.y)*clearRect.getBottom() + p1.y;
    double xmax = (p2.x - p1.x)*clearRect.getRight () + p1.x;
    double ymax = (p2.y - p1.y)*clearRect.getTop   () + p1.y;

    renderer->fillRect(CBBox2D(xmin, ymin, xmax, ymax), CRGBA(1,1,1));
  }

  //---

  const CGnuPlot::AxisData &xaxis = this->xaxis(xind_);
  const CGnuPlot::AxisData &yaxis = this->yaxis(yind_);

  double xmin = xaxis.min.getValue(0.0);
  double ymin = yaxis.min.getValue(0.0);
  double xmax = xaxis.max.getValue(1.0);
  double ymax = yaxis.max.getValue(1.0);

  CBBox2D bbox(xmin, ymin, xmax, ymax);

  renderer->setClip(bbox);

  //---

  if      (getSmooth() == Smooth::BEZIER) {
    const CGnuPlotLineStyle &lineStyle = this->lineStyle();

    const CRGBA &c = lineStyle.color(CRGBA(1,0,0));

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
  else if (style_ == PlotStyle::BOXERRORBARS) {
    drawBoxErrorBars(bbox);
  }
  else if (style_ == PlotStyle::BOXES) {
    drawBoxes(bbox);
  }
  else if (style_ == PlotStyle::BOXPLOT) {
    drawBoxPlot(bbox);
  }
  else if (style_ == PlotStyle::BOXXYERRORBARS) {
    drawBoxXYErrorBars(bbox);
  }
  else if (style_ == PlotStyle::CANDLESTICKS) {
    drawCandleSticks(bbox);
  }
  else if (style_ == PlotStyle::CIRCLES) {
    drawCircles(bbox);
  }
  else if (style_ == PlotStyle::ELLIPSES) {
    drawEllipses(bbox);
  }
  else if (style_ == PlotStyle::DOTS) {
    drawDots(bbox);
  }
  else if (style_ == PlotStyle::FILLEDCURVES) {
    drawFilledCurves(bbox);
  }
  else if (style_ == PlotStyle::FINANCEBARS) {
    drawFinanceBars(bbox);
  }
  else if (style_ == PlotStyle::HISTEPS || style_ == PlotStyle::STEPS ||
           style_ == PlotStyle::FSTEPS  || style_ == PlotStyle::FILLSTEPS) {
    drawSteps(bbox);
  }
  else if (isImageStyle()) {
    if (isBinary())
      drawBinaryImage(bbox);
    else
      drawImage(bbox);
  }
  else if (style_ == PlotStyle::IMPULSES) {
    drawImpulses(bbox);
  }
  else if (style_ == PlotStyle::LABELS) {
    drawLabels(bbox);
  }
  else if (style_ == PlotStyle::LINES) {
    drawLines();
  }
  else if (style_ == PlotStyle::LINES_POINTS) {
    drawLines();

    drawPoints();
  }
  else if (style_ == PlotStyle::PARALLELAXES) {
    drawParallelAxes(bbox);
  }
  else if (style_ == PlotStyle::POINTS) {
    drawPoints();
  }
  else if (style_ == PlotStyle::VECTORS) {
    drawVectors(bbox);
  }
  else if (style_ == PlotStyle::XERRORBARS) {
  }
  else if (style_ == PlotStyle::XYERRORBARS) {
    drawXYErrorBars(bbox);
  }
  else if (style_ == PlotStyle::YERRORBARS) {
  }
  else if (style_ == PlotStyle::XERRORLINES) {
  }
  else if (style_ == PlotStyle::XYERRORLINES) {
  }
  else if (style_ == PlotStyle::YERRORLINES) {
  }
}

void
CGnuPlotPlot::
drawBoxErrorBars(const CBBox2D &bbox)
{
//double xmin = bbox.getXMin();
  double ymin = bbox.getYMin();
//double xmax = bbox.getYMax();
//double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  double y2 = std::max(0.0, ymin);

  CRGBA lc = CRGBA(0,0,0); // lineStyle.color(CRGBA(0,0,0));
  CRGBA fc = (fillType() == FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 3) continue;

    double x  = reals[0];
    double y  = reals[1];
    double dx = getBoxWidth().getSpacing(getXSpacing());
    double dy = 0.0;
    double yl = y;
    double yh = y;

    // x y ydelta
    if      (reals.size() == 3) {
      dy = reals[2];

      yl = y - dy;
      yh = y + dy;
    }
    else if (reals.size() == 4) {
      // x y ydelta xdelta
      if (getBoxWidth().width != -2) {
        dx = reals[2];
        dy = reals[3];

        yl = y - dy;
        yh = y + dy;
      }
      // x y ylow yhigh
      else {
        yl = reals[2];
        yh = reals[3];
      }
    }
    // x y ylow yhigh xdelta
    else if (reals.size() >= 5) {
      yl = reals[2];
      yh = reals[3];
      dx = reals[4];

      if (reals.size() == 6) {
        int ls = 1;

        if (! point.getValue(6, ls))
          ls = 1;

        fc = CGnuPlotStyle::instance()->indexColor(ls);
      }
    }

    CBBox2D bbox(x - dx/2, y2, x + dx/2, y);

    fc.setAlpha(0.5);

    drawBar(x, y, bbox, fillType(), fillPattern(), fc, lc);

    drawLine(CPoint2D(x, yl), CPoint2D(x, yh), lineStyle.width(), lc);

    double w = dx/2;

    drawLine(CPoint2D(x - w/2, yl), CPoint2D(x + w/2, yl), lineStyle.width(), lc);
    drawLine(CPoint2D(x - w/2, yh), CPoint2D(x + w/2, yh), lineStyle.width(), lc);
  }
}

void
CGnuPlotPlot::
drawBoxes(const CBBox2D &bbox)
{
//double xmin = bbox.getXMin();
  double ymin = bbox.getYMin();
//double xmax = bbox.getYMax();
//double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  double bw = getBoxWidth().getSpacing(getXSpacing());

//double y1 = std::min(0.0, ymin);
  double y2 = std::max(0.0, ymin);

  CRGBA ftc = (fillType() == FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  const CRGBA &lc = lineStyle.color(ftc);
  const CRGBA &fc = lineStyle.color(CRGBA(0,0,0));

  for (const auto &point : getPoints2D()) {
    double bw1 = bw;

    std::vector<double> reals;

    if (point.getReals(reals) && reals.size() >= 2) {
      double x = reals[0];
      double y = reals[1];

      if (reals.size() > 2)
        bw1 = reals[2];

        CBBox2D bbox(x - bw1/2, y2, x + bw1/2, y);

      drawBar(x, y, bbox, fillType(), fillPattern(), lc, fc);
    }
  }
}

void
CGnuPlotPlot::
drawBoxPlot(const CBBox2D & /*bbox*/)
{
//double xmin = bbox.getXMin();
//double ymin = bbox.getYMin();
//double xmax = bbox.getYMax();
//double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  CRGBA fc = lineStyle.color(CRGBA(1,0,0));
  CRGBA lc = CRGBA(0,0,0);

  fc.setAlpha(0.5);

  double bw = getBoxWidth().getSpacing(0.1);

  double x        = 0;
  bool   discrete = false;

  for (const auto &point : getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double x1;

      if (point.getValue(1, x1))
        x = x1;

      if (point.getNumValues() >= 4)
        discrete = true;

      break;
    }
  }

  //----

  typedef std::vector<double>         YVals;
  typedef std::map<int,YVals>         IYVals;
  typedef std::map<std::string,YVals> SYVals;

  IYVals iyv;
  SYVals syv;

  for (const auto &point : getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double y = 0, y1 = 0;

      if (point.getValue(2, y1))
        y = y1;

      if (discrete && point.getNumValues() >= 4) {
        int         i;
        std::string s;

        if      (point.getValue(4, i))
          iyv[i].push_back(y);
        else if (point.getValue(4, s))
          syv[s].push_back(y);
      }
      else
        iyv[0].push_back(y);
    }
  }

  // always use index for x (ignore x value) ??
  double ix = (syv.empty() && iyv.size() == 1 ? x : 1);

  for (const auto &yv : iyv) {
    CBoxWhisker whisker(yv.second);

    double x1 = ix - bw/2;
    double x2 = ix + bw/2;

    drawLine(CPoint2D(ix, whisker.min()), CPoint2D(ix, whisker.max()), 1, lc);

    drawLine(CPoint2D(x1, whisker.min()), CPoint2D(x2, whisker.min()), 1, lc);
    drawLine(CPoint2D(x1, whisker.max()), CPoint2D(x2, whisker.max()), 1, lc);

    CBBox2D bbox(x1, whisker.lower(), x2, whisker.upper());

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc);

    drawLine(CPoint2D(x1, whisker.median()), CPoint2D(x2, whisker.median()), 1, lc);

    double opw = renderer->pixelWidthToWindowWidth  (4);
    double oph = renderer->pixelHeightToWindowHeight(4);

    for (const auto &o : whisker.outliers())
      renderer->drawEllipse(CPoint2D(ix, whisker.value(o)), opw, oph, 0, lc);

    ++ix;
  }

  for (const auto &yv : syv) {
    //const std::string &s = yv.first;

    CBoxWhisker whisker(yv.second);

    double x1 = ix - bw/2;
    double x2 = ix + bw/2;

    drawLine(CPoint2D(ix, whisker.min()), CPoint2D(ix, whisker.max()), 1, lc);

    drawLine(CPoint2D(x1, whisker.min()), CPoint2D(x2, whisker.min()), 1, lc);
    drawLine(CPoint2D(x1, whisker.max()), CPoint2D(x2, whisker.max()), 1, lc);

    CBBox2D bbox(x1, whisker.lower(), x2, whisker.upper());

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc);

    drawLine(CPoint2D(x1, whisker.median()), CPoint2D(x2, whisker.median()), 1, lc);

    double opw = renderer->pixelWidthToWindowWidth  (4);
    double oph = renderer->pixelHeightToWindowHeight(4);

    for (const auto &o : whisker.outliers())
      renderer->drawEllipse(CPoint2D(ix, whisker.value(o)), opw, oph, 0, lc);

    ++ix;
  }
}

void
CGnuPlotPlot::
drawBoxXYErrorBars(const CBBox2D & /*bbox*/)
{
  CRGBA lc = CRGBA(0,0,0); // lineStyle.color(CRGBA(0,0,0));
  CRGBA fc = (fillType() == FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 4) continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    // x y xlow xhigh ylow yhigh
    if      (reals.size() >= 6) {
      xl = reals[2];
      xh = reals[3];
      yl = reals[4];
      yh = reals[5];

      if (reals.size() >= 6) {
        int ls = 1;

        if (! point.getValue(7, ls))
          ls = 1;

        fc = CGnuPlotStyle::instance()->indexColor(ls);
      }
    }
    // x y xdelta ydelta
    else if (reals.size() >= 4) {
      double dx = reals[2];
      double dy = reals[3];

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (reals.size() >= 4) {
        int ls = 1;

        if (! point.getValue(5, ls))
          ls = 1;

        fc = CGnuPlotStyle::instance()->indexColor(ls);
      }
    }

    CBBox2D bbox(xl, yl, xh, yh);

    fc.setAlpha(0.5);

    drawBar(x, y, bbox, fillType(), fillPattern(), fc, lc);
  }
}

void
CGnuPlotPlot::
drawCandleSticks(const CBBox2D & /*bbox*/)
{
//double xmin = bbox.getXMin();
//double ymin = bbox.getYMin();
//double xmax = bbox.getYMax();
//double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  CRGBA c1 = (fillType() == FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  const CRGBA &c  = lineStyle.color(c1);
  double       lw = lineStyle.width();

  double bw = getBoxWidth().getSpacing(0.1);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (point.getReals(reals) && reals.size() >= 5) {
      double x = reals[0];

      double bmin = reals[1];
      double wmin = reals[2];
      double wmax = reals[3];
      double bmax = reals[4];

      double bw1 = bw;

      if (reals.size() > 5) bw1 = reals[5];

      drawLine(CPoint2D(x, wmin), CPoint2D(x, bmin), lw, c);
      drawLine(CPoint2D(x, bmax), CPoint2D(x, wmax), lw, c);

      double x1 = x - bw1/2;
      double x2 = x + bw1/2;

      if (whiskerBars() > 0) {
        drawLine(CPoint2D(x1, wmin), CPoint2D(x2, wmin), lw, c);
        drawLine(CPoint2D(x1, wmax), CPoint2D(x2, wmax), lw, c);
      }

      CBBox2D bbox(x1, bmin, x2, bmax);

      if      (fillType() == FillType::PATTERN)
        renderer->patternRect(bbox, fillPattern(), c);
      else if (fillType() == FillType::SOLID)
        renderer->fillRect(bbox, c);
      else if (bmin > bmax)
        renderer->fillRect(bbox, c);

      renderer->drawRect(bbox, c);
    }
  }
}

void
CGnuPlotPlot::
drawCircles(const CBBox2D & /*bbox*/)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  const CRGBA &lc = lineStyle.color(CRGBA(1,0,0));

  CRGBA fc = lc;

  if (fillStyle().transparent())
    fc.setAlpha(fillStyle().density());

   for (const auto &point : getPoints2D()) {
    double x, y, z;

    if (point.getXYZ(x, y, z)) {
      double w = z;
      double h = renderer->pixelHeightToWindowHeight(renderer->windowWidthToPixelWidth(w));

      if (fillStyle().style() == FillType::SOLID)
        renderer->fillEllipse(CPoint2D(x, y), w, h, 0, fc);

      if (fillStyle().border())
        renderer->drawEllipse(CPoint2D(x, y), w, h, 0, lc);
    }
  }
}

void
CGnuPlotPlot::
drawEllipses(const CBBox2D & /*bbox*/)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  CRGBA lc = lineStyle.color(CRGBA(1,0,0));
  CRGBA fc = lc;

  if (fillStyle().transparent())
    fc.setAlpha(fillStyle().density());

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 2)
      continue;

    double x = reals[0];
    double y = reals[1];

    double w = 0.5;
    double h = 0.3;
    double a = 0;

    if      (reals.size() == 3) {
      w = reals[2];
      h = w;
    }
    else if (reals.size() == 4) {
      w = reals[2];
      h = reals[3];
    }
    else if (reals.size() >= 5) {
      w = reals[2];
      h = reals[3];
      a = reals[4];

      if (reals.size() >= 6) {
        int ls = 1;

        if (! point.getValue(6, ls))
          ls = 1;

        lc = CGnuPlotStyle::instance()->indexColor(ls);
      }
    }

    if (fillStyle().style() == FillType::SOLID)
      renderer->fillEllipse(CPoint2D(x, y), w/2, h/2, a, fc);

    if (fillStyle().border())
      renderer->drawEllipse(CPoint2D(x, y), w/2, h/2, a, lc);
  }
}

void
CGnuPlotPlot::
drawDots(const CBBox2D & /*bbox*/)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  const CRGBA &c = lineStyle.color(CRGBA(1,0,0));

  for (const auto &point : getPoints2D()) {
    CPoint2D p;

    if (point.getPoint(p))
      renderer->drawPoint(p, c);
  }
}

void
CGnuPlotPlot::
drawFilledCurves(const CBBox2D &bbox)
{
  double xmin = bbox.getXMin();
  double ymin = bbox.getYMin();
  double xmax = bbox.getYMax();
  double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool fillbetween = false;

  if (! filledCurve_.xaxis && ! filledCurve_.yaxis && ! filledCurve_.xyval.isValid()) {
    if (! getPoints2D().empty() && getPoints2D()[0].getNumValues() > 2)
      fillbetween = true;
  }

  if (! fillbetween) {
    std::vector<CPoint2D> points;

    for (const auto &point : getPoints2D()) {
      CPoint2D p;

      if (point.getPoint(p))
        points.push_back(p);
    }

    //---

    bool inside = false;

    COptReal x, y;

    if      (filledCurve_.yval.isValid())
      y = filledCurve_.yval.getValue();
    else if (filledCurve_.xaxis == 1)
      y = ymin;
    else if (filledCurve_.xaxis == 2)
      y = ymax;
    else if (filledCurve_.yaxis == 1)
      x = xmin;
    else if (filledCurve_.yaxis == 2)
      x = xmax;
    else
      y = 0;

    bool bxy = filledCurve_.xyval.isValid();

    std::vector<CPoint2D> points1;

    for (const auto &p : points) {
      if ((filledCurve_.above && ((x.isValid() && x.getValue() > p.x) ||
                                  (y.isValid() && y.getValue() > p.y))) ||
          (filledCurve_.below && ((x.isValid() && x.getValue() < p.x) ||
                                  (y.isValid() && y.getValue() < p.y)))) {
        if (inside) {
          if (! points1.empty()) {
            const CPoint2D &p1 = points1.back();

            if      (x.isValid())
              points1.push_back(CPoint2D(x.getValue(), CGnuPlotUtil::avg({p1.y,p.y})));
            else if (y.isValid())
              points1.push_back(CPoint2D(CGnuPlotUtil::avg({p1.x,p.x}), y.getValue()));
          }
          else {
            if      (x.isValid())
              points1.push_back(CPoint2D(x.getValue(), p.y));
            else if (y.isValid())
              points1.push_back(CPoint2D(p.x, y.getValue()));
          }

          inside = false;
        }

        continue;
      }
      else {
        if (! inside) {
          if (! bxy) {
            if      (x.isValid())
              points1.push_back(CPoint2D(x.getValue(), p.y));
            else if (y.isValid())
              points1.push_back(CPoint2D(p.x, y.getValue()));
          }

          inside = true;
        }
      }

      if (inside)
        points1.push_back(p);
    }

    if (inside) {
      if (! points1.empty()) {
        if (! bxy) {
          const CPoint2D &p = points1.back();

          if      (x.isValid())
            points1.push_back(CPoint2D(x.getValue(), p.y));
          else if (y.isValid())
            points1.push_back(CPoint2D(p.x, y.getValue()));
        }
        else
          points1.push_back(filledCurve_.xyval.getValue());
      }
    }

    fillPolygon(points1, lineStyle.color(CRGBA(1,1,1)));

    drawLines();
  }
  else {
    std::vector<CPoint2D> points1, points2;
    bool                  inside = false;

    for (const auto &point : getPoints2D()) {
      double x, y1, y2;

      if (point.getXYZ(x, y1, y2)) {
        CPoint2D p1(x, y1);
        CPoint2D p2(x, y2);

        bool oldInside = inside;

        inside = ((! filledCurve_.above && ! filledCurve_.below) ||
                  (filledCurve_.above && y1 <= y2) ||
                  (filledCurve_.below && y1 >= y2));

        if (oldInside != inside) {
          if (! points2.empty()) {
            const auto &p11 = points1.back();
            const auto &p21 = points2.back();

            CPoint2D pi;

            CMathGeom2D::IntersectLine(p11, p1, p21, p2, &pi);

            if (inside) {
              points1.push_back(pi);

              for (auto p1 = points2.rbegin(), p2 = points2.rend(); p1 != p2; ++p1)
                points1.push_back(*p1);

              fillPolygon(points1, lineStyle.color(CRGBA(1,1,1)));
            }

            points1.clear();
            points2.clear();

            points1.push_back(pi);
          }
        }

        points1.push_back(p1);
        points2.push_back(p2);
      }
    }

    if (! points1.empty()) {
      for (auto p1 = points2.rbegin(), p2 = points2.rend(); p1 != p2; ++p1)
        points1.push_back(*p1);

      fillPolygon(points1, lineStyle.color(CRGBA(1,1,1)));
    }
  }
}

void
CGnuPlotPlot::
drawFinanceBars(const CBBox2D & /*bbox*/)
{
//double xmin = bbox.getXMin();
//double ymin = bbox.getYMin();
//double xmax = bbox.getYMax();
//double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CRGBA  lc = lineStyle.color(CRGBA(0,0,0));
  double lw = lineStyle.width();

  double sl = group()->pixelWidthToWindowWidth(4);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (point.getReals(reals) && reals.size() >= 5) {
      double date  = reals[0];
      double open  = reals[1];
      double low   = reals[2];
      double high  = reals[3];
      double close = reals[4];

      if (reals.size() >= 6) {
        int ls = 1;

        if (! point.getValue(6, ls))
          ls = 1;

        lc = CGnuPlotStyle::instance()->indexColor(ls);
      }

      drawLine(CPoint2D(date, low), CPoint2D(date, high), lw, lc);

      drawLine(CPoint2D(date, open ), CPoint2D(date - sl, open ), lw, lc);
      drawLine(CPoint2D(date, close), CPoint2D(date + sl, close), lw, lc);
    }
  }
}

void
CGnuPlotPlot::
drawSteps(const CBBox2D &bbox)
{
  double ymin = bbox.getYMin();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  const CRGBA &c = lineStyle.color(CRGBA(1,0,0));

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

        drawLine(p1, p3, lineStyle.width(), c);
        drawLine(p3, p4, lineStyle.width(), c);
        drawLine(p4, p2, lineStyle.width(), c);
      }
      else if (style_ == PlotStyle::STEPS) {
        CPoint2D p3(p2.x, p1.y);

        drawLine(p1, p3, lineStyle.width(), c);
        drawLine(p3, p2, lineStyle.width(), c);
      }
      else if (style_ == PlotStyle::FSTEPS) {
        CPoint2D p3(p1.x, p2.y);

        drawLine(p1, p3, lineStyle.width(), c);
        drawLine(p3, p2, lineStyle.width(), c);
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
drawBinaryImage(const CBBox2D & /*bbox*/)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

  int nd = imageData_.size()/3;
  int ny = nd/imageStyle_.w;

  int i  = 0;
  int di = 3;

  double dx = 0, dy = 0;

  if (imageStyle_.cx.isValid())
    dx = imageStyle_.cx.getValue() - imageStyle_.w/2.0;
  if (imageStyle_.cy.isValid())
    dy = imageStyle_.cy.getValue() - imageStyle_.h/2.0;

  double xo = imageStyle_.xo.getValue(imageStyle_.w/2.0);
  double yo = imageStyle_.yo.getValue(imageStyle_.h/2.0);

#if 0
  double idx = 1, idy = 1;

  if (imageStyle_.dx.isValid())
    idx = imageStyle_.dx.getValue();
  if (imageStyle_.dy.isValid())
    idy = imageStyle_.dy.getValue();
#endif

  bool   rotate = imageStyle_.a.isValid();
  double a      = -imageAngle();

  double ra = Deg2Rad(a);

  if (imageStyle_.flipy) ra = -ra;

  CPoint2D o(xo + dx, yo + dy);

  for (int y = 0; y < ny; ++y) {
    double y1 = (imageStyle_.flipy ? ny - 1 - y : y);

    for (int x = 0; x < imageStyle_.w; ++x, i += di) {
      double x1 = x;

      double r = imageData_[i + 0]/255.0;
      double g = imageData_[i + 1]/255.0;
      double b = imageData_[i + 2]/255.0;

      CRGBA c(r, g, b);

      if (! imageStyle_.usingCols.empty()) {
        if (imageStyle_.usingCols.size() > 1) {
          r = decodeImageUsingColor(0, c);
          g = decodeImageUsingColor(1, c);
          b = decodeImageUsingColor(2, c);

          c = CRGBA(r, g, b);
        }
        else {
          g = decodeImageUsingColor(0, c);

          c = CRGBA(g, g, g);
        }
      }

      CPoint2D p(x1 + dx, y1 + dy);

      if (rotate) {
        CPoint2D p1 = CMathGeom2D::RotatePoint(p, ra, o);

        if (! renderer->clip().inside(p1))
          continue;

        for (int py = 0; py < npy; ++py) {
          for (int px = 0; px < npx; ++px) {
            CPoint2D p1(p.x + px*pw, p.y + py*ph);

            CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

            renderer->drawPoint(p2, c);
          }
        }
      }
      else {
        if (! renderer->clip().inside(p))
          continue;

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        renderer->fillRect(bbox, c);
      }
    }
  }
}

void
CGnuPlotPlot::
drawImage(const CBBox2D & /*bbox*/)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

  double dx = 0, dy = 0;

  if (imageStyle_.cx.isValid())
    dx = imageStyle_.cx.getValue() - imageStyle_.w/2.0;
  if (imageStyle_.cy.isValid())
    dy = imageStyle_.cy.getValue() - imageStyle_.h/2.0;

  double xo = imageStyle_.xo.getValue(imageStyle_.w/2.0);
  double yo = imageStyle_.yo.getValue(imageStyle_.h/2.0);

  bool   rotate = imageStyle_.a.isValid();
  double a      = -imageAngle();

  double ra = Deg2Rad(a);

  if (imageStyle_.flipy) ra = -ra;

  CPoint2D o(xo + dx, yo + dy);

  int ny = getPoints2D().size();

  int y = 0;

  for (const auto &p : getPoints2D()) {
    double y1 = (imageStyle_.flipy ? ny - 1 - y : y);

    std::vector<double> reals;

    (void) p.getReals(reals);

    int x = 0;

    for (const auto &v : reals) {
      double x1 = x;

      CRGBA c = CGnuPlotStyle::instance()->indexColor(int(v));

      CPoint2D p(x1 + dx, y1 + dy);

      if (rotate) {
        CPoint2D p1 = CMathGeom2D::RotatePoint(p, ra, o);

        if (! renderer->clip().inside(p1))
          continue;

        for (int py = 0; py < npy; ++py) {
          for (int px = 0; px < npx; ++px) {
            CPoint2D p1(p.x + px*pw, p.y + py*ph);

            CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

            renderer->drawPoint(p2, c);
          }
        }
      }
      else {
        if (! renderer->clip().inside(p))
          continue;

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        renderer->fillRect(bbox, c);
      }

      ++x;
    }

    ++y;
  }
}

double
CGnuPlotPlot::
decodeImageUsingColor(int col, const CRGBA &c) const
{
  int nc = imageStyle_.usingCols.size();

  if (col < nc) {
    if (imageStyle_.usingCols[col].isInt)
      col = imageStyle_.usingCols[col].ival;
    else {
      bool lookup = true;

      std::string expr = imageStyle_.usingCols[col].str;

      // replace $N variables
      // TODO: easier to define $1 variables
      auto pos = expr.find('$');

      while (pos != std::string::npos) {
        lookup = false;

        int pos1 = ++pos;

        while (isdigit(expr[pos1]))
          ++pos1;

        std::string numStr = expr.substr(pos, pos1 - pos);

        int icol = 0;

        (void) CStrUtil::toInteger(numStr, &icol);

        double r = indexImageColor(icol, c);

        expr = expr.substr(0, pos - 1) + CStrUtil::toString(r) + expr.substr(pos1);

        pos = expr.find('$');
      }

      if (expr.size() > 2 && expr[0] == '(' && expr[expr.size() - 1] == ')') {
        lookup = false;

        expr = expr.substr(1, expr.size() - 2);
      }

      CExprValueP value = CExprInst->evaluateExpression(expr);

      if (! lookup) {
        double r;

        if (value.isValid() && value->getRealValue(r))
          return r;
      }
      else {
        CExprValueP value = CExprInst->evaluateExpression(expr);

        long l;

        if (value.isValid() && value->getIntegerValue(l))
          return indexImageColor(l, c);
      }

      return 1;
    }
  }

  return indexImageColor(col, c);
}

double
CGnuPlotPlot::
indexImageColor(int i, const CRGBA &c) const
{
  switch (i) {
    case 1 : return c.getRed  ();
    case 2 : return c.getGreen();
    case 3 : return c.getBlue ();
    default: return 1;
  }
}

void
CGnuPlotPlot::
drawImpulses(const CBBox2D &bbox)
{
//double xmin = bbox.getXMin();
  double ymin = bbox.getYMin();
//double xmax = bbox.getYMax();
//double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

//double y1 = std::min(0.0, ymin);
  double y2 = std::max(0.0, ymin);

  const CRGBA &c = lineStyle.color(CRGBA(1,0,0));

  for (const auto &point : getPoints2D()) {
    CPoint2D p;

    if (point.getPoint(p)) {
      CPoint2D p1(p.x, y2);

      drawLine(p1, p, lineStyle.width(), c);
    }
  }
}

void
CGnuPlotPlot::
drawLabels(const CBBox2D & /*bbox*/)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  for (const auto &point : getPoints2D()) {
    double x, y;

    if (! point.getX(x) || ! point.getY(y))
      continue;

    std::string str;

    if (! point.getValue(3, str))
      continue;

    renderer->drawText(CPoint2D(x, y), str);
  }
}

void
CGnuPlotPlot::
drawVectors(const CBBox2D & /*bbox*/)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (point.getReals(reals) && reals.size() >= 4) {
      CGnuPlotArrow arrow(app());

      arrow.setStyle(arrowStyle());

      if (reals.size() == 5)
        arrow.setVarValue(reals[4]);

      CPoint2D from(reals[0], reals[1]);

      CPoint2D to = from + CPoint2D(reals[2], reals[3]);

      if (to == from) continue;

      arrow.setFrom(from);
      arrow.setTo  (to);

      arrow.draw(renderer);
    }
  }
}

void
CGnuPlotPlot::
drawXYErrorBars(const CBBox2D & /*bbox*/)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CRGBA c = lineStyle.color(CRGBA(1,0,0));

  CGnuPlotRenderer *renderer = app()->renderer();

  double pw = renderer->pixelWidthToWindowWidth  (4);
  double ph = renderer->pixelHeightToWindowHeight(4);

  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 4) continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    // x y xlow xhigh ylow yhigh
    if      (reals.size() >= 6) {
      xl = reals[2];
      xh = reals[3];
      yl = reals[4];
      yh = reals[5];

      if (reals.size() >= 6) {
        int ls = 1;

        if (! point.getValue(7, ls))
          ls = 1;

        c = CGnuPlotStyle::instance()->indexColor(ls);
      }
    }
    // x y xdelta ydelta
    else if (reals.size() >= 4) {
      double dx = reals[2];
      double dy = reals[3];

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (reals.size() >= 4) {
        int ls = 1;

        if (! point.getValue(5, ls))
          ls = 1;

        c = CGnuPlotStyle::instance()->indexColor(ls);
      }
    }

    drawLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, c);
    drawLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, c);

    drawLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, c);
    drawLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, c);

    drawLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, c);
    drawLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, c);
  }
}

void
CGnuPlotPlot::
drawParallelAxes(const CBBox2D & /*bbox*/)
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  bool isVar = lineStyle.colorSpec().isVariable();

  CGnuPlotRenderer *renderer = app()->renderer();

  typedef std::vector<double>       Reals;
  typedef std::map<int, Reals>      IReals;
  typedef std::pair<double, double> MinMax;
  typedef std::map<int, MinMax>     IMinMax;

  IReals  ireals;
  IMinMax irange;

  // group values by index
  for (const auto &point : getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    int nr = (isVar ? reals.size() - 1 : reals.size());

    for (int i = 0; i < nr; ++i)
      ireals[i].push_back(reals[i]);
  }

  // calc range for index
  for (const auto &ir : ireals) {
    int          i     = ir.first;
    const Reals &reals = ir.second;

    COptReal rmin, rmax;

    for (const auto &r : reals) {
      rmin.updateMin(r);
      rmax.updateMax(r);
    }

    irange[i] = MinMax(rmin.getValue(0), rmax.getValue(0));
  }

  int nr = irange.size();

  //------

  // draw lines between points
  uint np = numPoints2D();

  for (uint i = 0; i < np; ++i) {
    const CGnuPlotPoint &point = getPoint2D(i);

    std::vector<double> reals;

    (void) point.getReals(reals);

    int nr = (isVar ? reals.size() - 1 : reals.size());

    std::vector<double> ireals;
    std::vector<CRGBA>  colors;

    for (int j = 0; j < nr; ++j) {
      const MinMax &mm = irange[j];

      double ymin = mm.first;
      double ymax = mm.second;

      const CGnuPlot::AxisData &paxis = axesData_.paxis[j + 1];

      if (paxis.min.isValid()) ymin = paxis.min.getValue();
      if (paxis.max.isValid()) ymax = paxis.max.getValue();

      double ir;

      if (! paxis.reverse)
        ir = CGnuPlotUtil::map(reals[j], ymin, ymax, 0, 1);
      else
        ir = CGnuPlotUtil::map(reals[j], ymax, ymin, 0, 1);

      ireals.push_back(ir);
    }

    CRGBA c;

    if (isVar)
      c = CGnuPlotStyle::instance()->indexColor(int(reals[nr] + 0.5));
    else
      c = lineStyle.color(CGnuPlotStyle::instance()->indexColor(i));

    for (uint j = 1; j < ireals.size(); ++j)
      drawLine(CPoint2D(j, ireals[j - 1]), CPoint2D(j + 1, ireals[j]), 1.0, c);
  }

  //------

  // draw axes
  CBBox2D clip  = renderer->clip();
  CBBox2D range = renderer->range();

  for (const auto &ir : irange) {
    int           i  = ir.first;
    const MinMax &mm = ir.second;

    double ymin = mm.first;
    double ymax = mm.second;

    const CGnuPlot::AxisData &paxis = axesData_.paxis[i + 1];

    if (paxis.min.isValid()) ymin = paxis.min.getValue();
    if (paxis.max.isValid()) ymax = paxis.max.getValue();

    CGnuPlotAxis *yaxis = group()->getPlotAxis('p', i + 1);

    if (! paxis.reverse)
      yaxis->setRange(ymin, ymax);
    else
      yaxis->setRange(ymax, ymin);

    yaxis->setDrawTickMark(paxis.showTics);
    yaxis->setDrawMinorTickMark(false);
    yaxis->setLabelInside(i > nr/2);

    CBBox2D range1(CBBox2D(range.getXMin(), ymin, range.getXMax(), ymax));

    renderer->setRange(range1);

    renderer->setClip(CBBox2D(clip.getXMin(), std::max(ymin, mm.first),
                              clip.getXMax(), std::min(ymax, mm.second)));

    yaxis->drawAxis(i + 1);
  }

  renderer->setRange(range);
  renderer->setClip(clip);
}

void
CGnuPlotPlot::
drawLines()
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

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

    drawPath(points, lineStyle.width(), lineStyle.color(CRGBA(1,0,0)), lineStyle.dash());
  }

#if 0
  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (! point1.isDiscontinuity() && point1.getPoint(p1) && point2.getPoint(p2))
      drawLine(p1, p2, lineStyle.width(), lineStyle.color(CRGBA(1,0,0)), lineStyle.dash());
  }
#endif
}

void
CGnuPlotPlot::
drawPoints()
{
  const CGnuPlotLineStyle &lineStyle = this->lineStyle();

  CGnuPlotRenderer *renderer = app()->renderer();

  const CRGBA &c = lineStyle.color(CRGBA(1,0,0));

  for (const auto &point : getPoints2D()) {
    CPoint2D p;

    if (point.getPoint(p)) {
      group()->mapLogPoint(p);

      double size = pointSize();

      if (pointStyle().varSize()) {
        if (! point.getValue(3, size))
          size = 1;
      }

      renderer->drawSymbol(p, pointType(), size, c);
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
  setLineStyle(*app()->lineStyle(ind));
}

void
CGnuPlotPlot::
drawTerminal()
{
  static double grid_dashes[]   = {1, 3};
  static uint   num_grid_dashes = 2;

  //---

  CGnuPlotRenderer *renderer = app()->renderer();

  double px1, py1, px2, py2;

  group()->windowToPixel(0.0, 0.0, &px1, &py1);
  group()->windowToPixel(1.0, 1.0, &px2, &py2);

  double pxm = CGnuPlotUtil::avg({px1, px2});
  double pym = CGnuPlotUtil::avg({py1, py2});

  double pw = px2 - px1;
  double ph = py1 - py2;

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  //---

  renderer->setLineDash(CLineDash(grid_dashes, num_grid_dashes));

  renderer->drawLine(CPoint2D(0.5, 0.0), CPoint2D(0.5, 1.0));
  renderer->drawLine(CPoint2D(0.0, 0.5), CPoint2D(1.0, 0.5));

  renderer->setLineDash(CLineDash());

  //---

  renderer->drawHAlignedText(group()->pixelToWindow(CPoint2D(px1 + 4, py2 + 4)),
                             CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_TOP, 0,
                             "terminal test", CRGBA(1,0,0));

  //---

  // symbols on right
  int    nlines = int(ph/font_size);
  double dy     = 1.0/nlines;

  for (int i = 0; i < nlines; ++i) {
    std::string str = CStrUtil::toString(i - 1);

    double w = font->getStringWidth(str);

    CRGBA c = CGnuPlotStyle::instance()->indexColor(i - 1);

    double x, y;

    group()->pixelToWindow(px2 - 48 - w, py2 + font->getCharAscent(), &x, &y);

    renderer->drawText(CPoint2D(x, y - i*dy), str, c);

    double x1, y1, x2, y2;

    group()->pixelToWindow(px2 - 48, py2 + font->getCharAscent()/2, &x1, &y1);
    group()->pixelToWindow(px2 - 24, py2 + font->getCharAscent()/2, &x2, &y2);

    renderer->drawLine(CPoint2D(x1, y1 - i*dy), CPoint2D(x2, y1 - i*dy), 0, c);

    group()->pixelToWindow(px2 - 16, py2 + font->getCharAscent()/2, &x1, &y1);

    renderer->drawSymbol(CPoint2D(x1, y1 - i*dy),
                          CGnuPlotStyle::instance()->indexSymbol(i - 1), 1.0, c);
  }

  //---

  // text in center
  renderer->drawHAlignedText(group()->pixelToWindow(CPoint2D(pxm, pym - 5*font_size)),
                             CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_CENTER, 0, "left justified");
  renderer->drawHAlignedText(group()->pixelToWindow(CPoint2D(pxm, pym - 4*font_size)),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0, "centre+d text");
  renderer->drawHAlignedText(group()->pixelToWindow(CPoint2D(pxm, pym - 3*font_size)),
                             CHALIGN_TYPE_RIGHT, 0, CVALIGN_TYPE_CENTER, 0, "right justified");

  renderer->drawHAlignedText(group()->pixelToWindow(CPoint2D(pxm, pym - font_size)),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "test of character width",
                             CGnuPlotStyle::instance()->indexColor(4));
  renderer->drawHAlignedText(CPoint2D(0.5, 0.5),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "12345678901234567890",
                             CGnuPlotStyle::instance()->indexColor(4));

  double w = font->getStringWidth("12345678901234567890");

  double x1, y1, x2, y2;

  group()->pixelToWindow(pxm - w/2, pym + font_size/2, &x1, &y1);
  group()->pixelToWindow(pxm + w/2, pym - font_size/2, &x2, &y2);

  renderer->drawRect(CBBox2D(x1, y1, x2, y2), CGnuPlotStyle::instance()->indexColor(4));

  //---

  // rotated text on left
  renderer->drawVAlignedText(group()->pixelToWindow(CPoint2D(px1 + 1*font_size, pym)),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             "rotated ce+ntered text", CRGBA(0,1,0));

  renderer->drawRotatedText(group()->pixelToWindow(CPoint2D(px1 + 2*font_size, pym)),
                            "rotated by +45 deg"    , CRGBA(0,1,0),  45);
  renderer->drawRotatedText(group()->pixelToWindow(CPoint2D(px1 + 3*font_size, pym)),
                            "rotated by -45 deg"    , CRGBA(0,1,0), -45);

  //---

  // filled polygons
  double pl = pw/16;

  CPoint2D pp1 = CPoint2D(pxm + pw/4, pym - ph/4);
  CPoint2D pp2 = pp1 + CPoint2D(pl, pl);

  renderer->drawHAlignedText(group()->pixelToWindow(pp1 - CPoint2D(0, pl)), CHALIGN_TYPE_CENTER, 0,
                             CVALIGN_TYPE_BOTTOM, 0, "filled polygons:");

  std::vector<CPoint2D> points1, points2;

  for (int i = 0; i < 6; ++i) {
    double a = Deg2Rad(i*60);

    points1.push_back(group()->pixelToWindow(pp1 + CPoint2D(pl*cos(a), pl*sin(a))));
    points2.push_back(group()->pixelToWindow(pp2 + CPoint2D(pl*cos(a), pl*sin(a))));
  }

  renderer->fillPolygon(points1, CRGBA(0,0,1));
  renderer->fillPolygon(points2, CRGBA(0,1,0,0.5));

  //---

  // arrows
  CPoint2D ac = group()->pixelToWindow(CPoint2D(pxm - 100, pym + 100));
  double   al = 50;

  for (int i = 0; i < 8; ++i) {
    CGnuPlotArrow arrow(app());

    double a = i*Deg2Rad(45);

    double dx = al*cos(a);
    double dy = -al*sin(a);

    CPoint2D ac1 = group()->pixelToWindow(CPoint2D(pxm - 100 + dx, pym + 100 + dy));

    arrow.setFrom(ac);
    arrow.setTo  (ac1);

    arrow.setLength(group()->pixelWidthToWindowWidth(al/5));

    arrow.setAngle(30);

    arrow.setFHead(i == 7);
    arrow.setTHead(i != 3 && i != 7);

    arrow.setFilled(i == 2);
    arrow.setEmpty (i == 1 || i == 4 || i == 5 || i == 6 || i == 7);

    arrow.setStrokeColor(CRGBA(1,0,0));

    arrow.draw(renderer);
  }

  //---

  // pattern fill
  double ptw = pw/30;
  double pth = ph/8;
  double ptb = 4;

  for (int i = 0; i <= 9; ++i) {
    double px = pxm + i*(ptw + ptb);

    CPoint2D p1 = group()->pixelToWindow(CPoint2D(px      , py1 - pth));
    CPoint2D p2 = group()->pixelToWindow(CPoint2D(px + ptw, py1      ));

    renderer->patternRect(CBBox2D(p1, p2), CGnuPlotStyle::instance()->indexPattern(i),
                           CRGBA(0,0,0), CRGBA(1,1,1));

    renderer->drawRect(CBBox2D(p1, p2), CRGBA(0,0,0));

    renderer->drawHAlignedText(group()->pixelToWindow(CPoint2D(px + ptw/2, py1 - pth)),
                               CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0,
                               CStrUtil::strprintf("%d", i));
  }

  renderer->drawHAlignedText(group()->pixelToWindow(
                              CPoint2D(pxm + 4*(ptw + ptb), py1 - pth - font_size)),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0, "pattern fill");

  //---

  // line width bottom left
  double ll  = pw/8;
  double lb1 = 8;
  double lb2 = 8;

  for (int i = 1; i <= 6; ++i) {
    renderer->drawLine(group()->pixelToWindow(CPoint2D(px1 + lb1     , py1 - i*font_size)),
                       group()->pixelToWindow(CPoint2D(px1 + lb1 + ll, py1 - i*font_size)), i);

    renderer->drawHAlignedText(group()->pixelToWindow(
                                CPoint2D(px1 + lb1 + lb2 + ll, py1 - i*font_size)),
                               CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_CENTER, 0,
                               CStrUtil::strprintf("lw %d", i));
  }

  renderer->drawHAlignedText(group()->pixelToWindow(
                                CPoint2D(px1 + lb1 + ll/2, py1 - 7*font_size - 4)),
                             CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP, 0, "linewidth");
}

void
CGnuPlotPlot::
drawPalette()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  double px1, py1, px2, py2;

  group()->windowToPixel(0.0, 0.0, &px1, &py1);
  group()->windowToPixel(1.0, 1.0, &px2, &py2);

  double wx1, wy1, wx2, wy2;

  group()->pixelToWindow(0, py1 + 32, &wx1, &wy1);
  group()->pixelToWindow(0, py1 + 64, &wx2, &wy2);

  bool   first = true;
  double r1, g1, b1, m1, x1;

  for (double i = px1; i <= px2; i += 1.0) {
    double wx, wy;

    group()->pixelToWindow(i, 0, &wx, &wy);

    CRGBA c = palette_.getColor(wx);

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

  renderer->drawRect(CBBox2D(0.0, wy1, 1.0, wy2), CRGBA(0,0,0));

#if 0
  CGnuPlotAxis *xaxis = group()->getPlotAxis('x', 1);
  CGnuPlotAxis *yaxis = group()->getPlotAxis('y', 1);

  xaxis->drawAxis(0.0);
  yaxis->drawAxis(0.0);

  xaxis->drawGrid(0.0, 1.0);
  yaxis->drawGrid(0.0, 1.0);
#endif
}

void
CGnuPlotPlot::
drawPath(const std::vector<CPoint2D> &points, double w, const CRGBA &c, const CLineDash &dash)
{
  // TODO: clip
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->drawPath(points, w, c, dash);
}

void
CGnuPlotPlot::
drawLine(const CPoint2D &p1, const CPoint2D &p2, double w, const CRGBA &c, const CLineDash &dash)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CPoint2D p11 = p1, p21 = p2;

  if (renderer->clipLine(p11, p21))
    renderer->drawLine(p11, p21, w, c, dash);
}

void
CGnuPlotPlot::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  std::vector<CPoint2D> ipoints;

  if (CMathGeom2D::IntersectPolygon(points, renderer->clip(), ipoints))
    renderer->fillPolygon(ipoints, c);
}

void
CGnuPlotPlot::
calcXRange(double *xmin, double *xmax) const
{
  const CGnuPlot::AxisData &xaxis = this->xaxis(xind_);

  if (! xaxis.min.isValid() || ! xaxis.max.isValid()) {
std::cerr << "CGnuPlotPlot::calcXRange" << std::endl;
    double xmin1, xmax1;

    getXRange(&xmin1, &xmax1);

    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    CGnuPlot::AxisData &xaxis = th->xaxis(xind_);

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
  const CGnuPlot::AxisData &xaxis = this->xaxis(xind_);
  const CGnuPlot::AxisData &yaxis = this->yaxis(yind_);

  if (! yaxis.min.isValid() || ! yaxis.max.isValid()) {
std::cerr << "CGnuPlotPlot::calcYRange" << std::endl;
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    CGnuPlot::AxisData &yaxis = th->yaxis(yind_);

    double ymin1, ymax1;

    if (xaxis.min.isValid() && xaxis.max.isValid() && ! is3D()) {
      double xmin1 = xaxis.min.getValue();
      double xmax1 = xaxis.max.getValue();

      for (const auto &point : getPoints2D()) {
        double x, y;

        if (point.getXY(x, y)) {
          if (x < xmin1 || x > xmax1)
            continue;

          yaxis.min.updateMin(y);
          yaxis.max.updateMax(y);
        }
      }
    }
    else {
      getYRange(&ymin1, &ymax1);

      yaxis.min = ymin1;
      yaxis.max = ymax1;
    }
  }

  *ymin = yaxis.min.getValue(-1);
  *ymax = yaxis.max.getValue( 1);
}
