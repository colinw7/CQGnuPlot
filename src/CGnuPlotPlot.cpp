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

int CGnuPlotPlot::nextId_ = 1;

CGnuPlotPlot::
CGnuPlotPlot(CGnuPlotGroup *group) :
 group_(group), id_(nextId_++), contour_(this)
{
  setSmooth(app()->getSmooth());

  setFitX(app()->getFitX());
  setFitY(app()->getFitY());
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
  setLineStyle (plot->lineStyle ());
  setPointStyle(plot->pointStyle());

  setAxesData(plot->axesData());

  xind_ = plot->xind();
  yind_ = plot->yind();

  setPalette(plot->palette());
  setFilledCurve(plot->filledCurve());
  setTrianglePattern3D(plot->trianglePattern3D());
  setWhiskerBars(plot->whiskerBars());
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
  if (! getFitX() && ! getFitY())
    return;

  CGnuPlot::AxisData &xaxis = (xind_ == 2 ? axesData_.x2axis : axesData_.xaxis);
  CGnuPlot::AxisData &yaxis = (yind_ == 2 ? axesData_.y2axis : axesData_.yaxis);

  if (! is3D_) {
    if (! xaxis.min.isValid() || ! xaxis.max.isValid()) {
      COptReal xmin, xmax;

      for (const auto &p : getPoints2D()) {
        double x;

        if (p.getX(x)) {
          xmin.updateMin(x);
          xmax.updateMax(x);
        }
      }

      if (! xaxis.min.isValid() && xmin.isValid()) xaxis.min = xmin;
      if (! xaxis.max.isValid() && xmax.isValid()) xaxis.max = xmax;
    }

    COptReal ymin, ymax;

    if (! yaxis.min.isValid() || ! yaxis.max.isValid()) {
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

    if (getFitX()) {
      if (! xaxis.min.isValid() && xmin.isValid()) xaxis.min = xmin;
      if (! xaxis.max.isValid() && xmax.isValid()) xaxis.max = xmax;
    }

    if (getFitY()) {
      if (! yaxis.min.isValid() && ymin.isValid()) yaxis.min = ymin;
      if (! yaxis.max.isValid() && ymax.isValid()) yaxis.max = ymax;
    }
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

  const CGnuPlot::AxisData &xaxis = (xind_ == 2 ? axesData().x2axis : axesData().xaxis);
  const CGnuPlot::AxisData &yaxis = (yind_ == 2 ? axesData().y2axis : axesData().yaxis);

  double xmin = xaxis.min.getValue(0.0);
  double ymin = yaxis.min.getValue(0.0);
  double xmax = xaxis.max.getValue(1.0);
  double ymax = yaxis.max.getValue(1.0);

  clip_ = CBBox2D(xmin, ymin, xmax, ymax);

  //---

  CGnuPlotLineStyleP lineStyle = calcLineStyle();

  uint np = numPoints2D();

  if (getSmooth() == Smooth::BEZIER) {
    const CRGBA &c = lineStyle->color(CRGBA(1,0,0));

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
                           lineStyle->width(), c);
    }
  }
  else if (style_ == PlotStyle::LABELS) {
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
  else {
    if (style_ == PlotStyle::LINES || style_ == PlotStyle::LINES_POINTS)
      drawLines();

    if      (style_ == PlotStyle::DOTS) {
      const CRGBA &c = lineStyle->color(CRGBA(1,0,0));

      for (const auto &point : getPoints2D()) {
        CPoint2D p;

        if (point.getPoint(p))
          renderer->drawPoint(p, c);
      }
    }
    else if (style_ == PlotStyle::CIRCLES) {
      const CRGBA &lc = lineStyle->color(CRGBA(1,0,0));

      CRGBA fc = lc;

      if (fillStyle().transparent())
        fc.setAlpha(fillStyle().density());

      for (const auto &point : getPoints2D()) {
        double x, y, z;

        if (point.getXYZ(x, y, z)) {
          double w = z;
          double h = renderer->pixelHeightToWindowHeight(renderer->windowWidthToPixelWidth(w));

          if (fillStyle().style() == FillType::SOLID)
            renderer->fillEllipse(CPoint2D(x, y), w, h, fc);

          if (fillStyle().border())
            renderer->drawEllipse(CPoint2D(x, y), w, h, lc);
        }
      }
    }

    if (style_ == PlotStyle::POINTS || style_ == PlotStyle::LINES_POINTS) {
      const CRGBA &c = lineStyle->color(CRGBA(1,0,0));

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

    if (style_ == PlotStyle::HISTEPS || style_ == PlotStyle::STEPS ||
        style_ == PlotStyle::FSTEPS) {
      const CRGBA &c = lineStyle->color(CRGBA(1,0,0));

      if      (style_ == PlotStyle::HISTEPS) {
        for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
          const CGnuPlotPoint &point1 = getPoint2D(i1);
          const CGnuPlotPoint &point2 = getPoint2D(i2);

          CPoint2D p1, p2;

          if (point1.getPoint(p1) && point2.getPoint(p2)) {
            double xm = CGnuPlotUtil::avg({p1.x, p2.x});

            CPoint2D p3(xm, p1.y);
            CPoint2D p4(xm, p2.y);

            drawLine(p1, p3, lineStyle->width(), c);
            drawLine(p3, p4, lineStyle->width(), c);
            drawLine(p4, p2, lineStyle->width(), c);
          }
        }
      }
      else if (style_ == PlotStyle::STEPS) {
        for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
          const CGnuPlotPoint &point1 = getPoint2D(i1);
          const CGnuPlotPoint &point2 = getPoint2D(i2);

          CPoint2D p1, p2;

          if (point1.getPoint(p1) && point2.getPoint(p2)) {
            CPoint2D p3(p2.x, p1.y);

            drawLine(p1, p3, lineStyle->width(), c);
            drawLine(p3, p2, lineStyle->width(), c);
          }
        }
      }
      else if (style_ == PlotStyle::FSTEPS) {
        for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
          const CGnuPlotPoint &point1 = getPoint2D(i1);
          const CGnuPlotPoint &point2 = getPoint2D(i2);

          CPoint2D p1, p2;

          if (point1.getPoint(p1) && point2.getPoint(p2)) {
            CPoint2D p3(p1.x, p2.y);

            drawLine(p1, p3, lineStyle->width(), c);
            drawLine(p3, p2, lineStyle->width(), c);
          }
        }
      }

      for (const auto &point : getPoints2D()) {
        CPoint2D p;

        if (point.getPoint(p))
          renderer->drawSymbol(p, SymbolType::FILLED_BOX, 1.0, c);
      }
    }

    if (style_ == PlotStyle::BOXES) {
      double xi = getBoxWidth().getSpacing(getXSpacing());

    //double y1 = std::min(0.0, ymin);
      double y2 = std::max(0.0, ymin);

      CRGBA fc = (fillType() == FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

      const CRGBA &c = lineStyle->color(fc);

      for (const auto &point : getPoints2D()) {
        CPoint2D p;

        double xi1 = xi;

        (void) point.getValue(3, xi1);

        if (point.getPoint(p)) {
          CBBox2D bbox(p.x - xi1/2, y2, p.x + xi1/2, p.y);

          drawBar(p.x, p.y, bbox, fillType(), fillPattern(), c,
                  lineStyle->color(CRGBA(0,0,0)));
        }
      }
    }

    if (style_ == PlotStyle::IMPULSES) {
    //double y1 = std::min(0.0, ymin);
      double y2 = std::max(0.0, ymin);

      const CRGBA &c = lineStyle->color(CRGBA(1,0,0));

      for (const auto &point : getPoints2D()) {
        CPoint2D p;

        if (point.getPoint(p)) {
          CPoint2D p1(p.x, y2);

          drawLine(p1, p, lineStyle->width(), c);
        }
      }
    }

    if (style_ == PlotStyle::FILLEDCURVES) {
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

        fillPolygon(points1, lineStyle->color(CRGBA(1,1,1)));

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

                  fillPolygon(points1, lineStyle->color(CRGBA(1,1,1)));
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

          fillPolygon(points1, lineStyle->color(CRGBA(1,1,1)));
        }
      }
    }

    if (style_ == PlotStyle::CANDLESTICKS) {
      CRGBA c1 = (fillType() == FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

      const CRGBA &c  = lineStyle->color(c1);
      double       lw = lineStyle->width();

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
  }
}

void
CGnuPlotPlot::
drawLines()
{
  CGnuPlotLineStyleP lineStyle = calcLineStyle();

  uint np = numPoints2D();

  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (point1.getPoint(p1) && point2.getPoint(p2))
      drawLine(p1, p2, lineStyle->width(), lineStyle->color(CRGBA(1,0,0)));
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

CGnuPlotLineStyleP
CGnuPlotPlot::
lineStyle() const
{
  if (! lineStyle_) {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    th->lineStyle_ = app()->lineStyle();
  }

  return lineStyle_;
}

CGnuPlotLineStyleP
CGnuPlotPlot::
calcLineStyle() const
{
  //if (lineStyleNum().isValid())
  //  return app()->lineStyle(lineStyleNum().getValue());

  return lineStyle();
}

void
CGnuPlotPlot::
setLineStyleId(int ind)
{
  setLineStyle(app()->lineStyle(ind));
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
    CGnuPlotArrow arrow;

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
  CGnuPlotAxis xaxis(this, CORIENTATION_HORIZONTAL);
  CGnuPlotAxis yaxis(this, CORIENTATION_VERTICAL);

  xaxis.setFont(renderer->getFont());
  yaxis.setFont(renderer->getFont());

  xaxis.drawAxis(0.0);
  yaxis.drawAxis(0.0);

  xaxis.drawGrid(0.0, 1.0);
  yaxis.drawGrid(0.0, 1.0);
#endif
}

void
CGnuPlotPlot::
drawLine(const CPoint2D &p1, const CPoint2D &p2, double w, const CRGBA &c)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  double x1 = p1.x, y1 = p1.y;
  double x2 = p2.x, y2 = p2.y;

  if (CMathGeom2D::clipLine(clip_.getXMin(), clip_.getYMin(), clip_.getXMax(), clip_.getYMax(),
                            &x1, &y1, &x2, &y2))
    renderer->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2), w, c);
}

void
CGnuPlotPlot::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  std::vector<CPoint2D> ipoints;

  if (CMathGeom2D::IntersectPolygon(points, clip_, ipoints))
    renderer->fillPolygon(ipoints, c);
}

void
CGnuPlotPlot::
calcXRange(double *xmin, double *xmax) const
{
  const CGnuPlot::AxisData &xaxis = (xind_ == 2 ? axesData().x2axis : axesData().xaxis);

  if (! xaxis.min.isValid() || ! xaxis.max.isValid()) {
    double xmin1, xmax1;

    getXRange(&xmin1, &xmax1);

    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    CGnuPlot::AxisData &xaxis = (xind_ == 2 ? th->axesData_.x2axis : th->axesData_.xaxis);

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
  const CGnuPlot::AxisData &xaxis = (xind_ == 2 ? axesData().x2axis : axesData().xaxis);
  const CGnuPlot::AxisData &yaxis = (yind_ == 2 ? axesData().y2axis : axesData().yaxis);

  if (! yaxis.min.isValid() || ! yaxis.max.isValid()) {
    CGnuPlotPlot *th = const_cast<CGnuPlotPlot *>(this);

    CGnuPlot::AxisData &yaxis = (yind_ == 2 ? th->axesData_.y2axis : th->axesData_.yaxis);

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
