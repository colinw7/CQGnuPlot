#include <CGnuPlotStyleFilledCurves.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotUtil.h>
#include <CMathGeom2D.h>

namespace {

void getLimit(const CGnuPlotFilledCurve &filledCurve, const CBBox2D &bbox,
              COptReal &x, COptReal &y, COptReal &r) {
  double xmin = bbox.getXMin();
  double ymin = bbox.getYMin();
  double xmax = bbox.getXMax();
  double ymax = bbox.getYMax();

  if      (filledCurve.yval.isValid())
    y = filledCurve.yval.getValue();
  else if (filledCurve.xval.isValid())
    x = filledCurve.xval.getValue();
  else if (filledCurve.rval.isValid())
    r = filledCurve.rval.getValue();
  else if (filledCurve.xaxis == 1)
    y = ymin;
  else if (filledCurve.xaxis == 2)
    y = ymax;
  else if (filledCurve.yaxis == 1)
    x = xmin;
  else if (filledCurve.yaxis == 2)
    x = xmax;
}

bool isInside(const CGnuPlotFilledCurve &filledCurve,
              const CPoint2D &p, const COptReal &x, const COptReal &y, const COptReal &r) {
  if      (filledCurve.above) {
    if (x.isValid()) return p.x >= x.getValue();
    if (y.isValid()) return p.y >= y.getValue();

    if (r.isValid()) {
      double r1 = hypot(p.x, p.y);

      return (r1 >= r.getValue());
    }
  }
  else if (filledCurve.below) {
    if (x.isValid()) return p.x <= x.getValue();
    if (y.isValid()) return p.y <= y.getValue();

    if (r.isValid()) {
      double r1 = hypot(p.x, p.y);

      return (r1 < r.getValue());
    }
  }

  return true;
}

bool interpPoint(const CPoint2D &p1, const CPoint2D &p2,
                 const COptReal &x, const COptReal &y, const COptReal &r, CPoint2D &pi) {
  double mi = 0;

  if      (x.isValid()) {
    if (fabs(p2.x - p1.x) > 1E-6)
      mi = (x.getValue() - p1.x)/(p2.x - p1.x);

    double yi = p1.y + mi*(p2.y - p1.y);

    pi = CPoint2D(x.getValue(), yi);
  }
  else if (y.isValid()) {
    if (fabs(p2.y - p1.y) > 1E-6)
      mi = (y.getValue() - p1.y)/(p2.y - p1.y);

    double xi = p1.x + mi*(p2.x - p1.x);

    pi = CPoint2D(xi, y.getValue());
  }
  else if (r.isValid()) {
    double r1 = hypot(p1.x, p1.y);
    double r2 = hypot(p2.x, p2.y);

    if (fabs(r2 - r1) > 1E-6)
      mi = (r.getValue() - r1)/(r2 - r1);

    double xi = p1.x + mi*(p2.x - p1.x);
    double yi = p1.y + mi*(p2.y - p1.y);

    pi = CPoint2D(xi, yi);
  }
  else
    return false;

  return true;
}

void closePoints(std::vector<CPoint2D> &points,
                 const COptReal &x, const COptReal &y, const COptReal &r) {
  if (points.size() < 3) return;

  const CPoint2D &p1 = points[points.size() - 1];
  const CPoint2D &p3 = points[0];

  CPoint2D p2 = (p1 + p3)/2;

  if (p1 == p2)
    return;

  if      (x.isValid() || y.isValid()) {
    points.push_back(p3);
    points.push_back(p2);
  }
  else if (r.isValid()) {
    double r1 = hypot(p1.x, p1.y);
    double r3 = hypot(p3.x, p3.y);
    double a1 = atan2(p1.y, p1.x);
    double a2 = atan2(p2.y, p2.x);
    double a3 = atan2(p3.y, p3.x);

    if (a1 < 0) a1 = 2*M_PI + a1;
    if (a2 < 0) a2 = 2*M_PI + a2;
    if (a3 < 0) a3 = 2*M_PI + a3;

    bool between = (a2 > std::min(a1, a3) && a2 < std::max(a1, a3));

    if (! between)
      a1 += 2*M_PI;

    int n = 50;

    for (int i = 1; i < n; ++i) {
      double r = r1 + i*(r3 - r1)/(n - 1);
      double a = a1 + i*(a3 - a1)/(n - 1);

      double x = r*cos(a);
      double y = r*sin(a);

      points.push_back(CPoint2D(x, y));
    }
  }
}

}

CGnuPlotStyleFilledCurves::
CGnuPlotStyleFilledCurves() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::FILLEDCURVES)
{
}

void
CGnuPlotStyleFilledCurves::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotFill fill(plot);

  //---

  bool fillbetween = false;

  const CGnuPlotFilledCurve &filledCurve = plot->filledCurve();

  if (! filledCurve.xaxis && ! filledCurve.yaxis && ! filledCurve.xyval.isValid()) {
    if (! plot->getPoints2D().empty() && plot->getPoints2D()[0].getNumValues() > 2)
      fillbetween = true;
  }

  //---

  if (! fillbetween) {
    PointsArray pointsVector;

    uint np = plot->numPoints2D();

    CPoint2D p1;

    uint i = 0;

    while (i < np) {
      // get first point
      for ( ; i < np; ++i) {
        const CGnuPlotPoint &point1 = plot->getPoint2D(i);

        if (! point1.getPoint(p1))
          continue;

        if (renderer->isPseudo() && ! renderer->isInside(p1))
          continue;

        break;
      }

      ++i;

      Points points;

      points.push_back(p1);

      // add points until discontinuity
      for ( ; i < np; ++i) {
        CPoint2D p2;

        const CGnuPlotPoint &point2 = plot->getPoint2D(i);

        if (! point2.getPoint(p2) || point2.isDiscontinuity())
          break;

        if (renderer->isPseudo() && ! renderer->isInside(p1))
          break;

        points.push_back(p2);

        p1 = p2;
      }

      if (! points.empty())
        pointsVector.push_back(points);
    }

    PointsArray pointsArray;

    for (auto &points : pointsVector)
      addPolygons(plot, points, pointsArray);

    if (! renderer->isPseudo() && plot->isCacheActive()) {
      plot->updatePolygonCacheSize(pointsArray.size());
    }

    int pi = 0;

    for (auto &points : pointsArray) {
      drawPolygon(plot, renderer, pi, points);

      ++pi;
    }

    if (! renderer->isPseudo() && plot->isCacheActive()) {
      for (const auto &polygon : plot->polygonObjects())
        polygon->draw(renderer);
    }
  }
  else {
    // TODO: handle disconnected points ?
    std::vector<CPoint2D> points1, points2;
    bool                  inside = false;

    for (const auto &point : plot->getPoints2D()) {
      double x, y1, y2;

      if (! point.getXYZ(x, y1, y2))
        continue;

      CPoint2D p1(x, y1);
      CPoint2D p2(x, y2);

      if (renderer->isPseudo() && ! renderer->isInside(p1))
        continue;

      bool oldInside = inside;

      inside = ((! filledCurve.above && ! filledCurve.below) ||
                (filledCurve.above && y1 <= y2) ||
                (filledCurve.below && y1 >= y2));

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

            renderer->fillClippedPolygon(points1, fill);
          }

          points1.clear();
          points2.clear();

          points1.push_back(pi);
        }
      }

      points1.push_back(p1);
      points2.push_back(p2);
    }

    if (! points1.empty()) {
      for (auto p1 = points2.rbegin(), p2 = points2.rend(); p1 != p2; ++p1)
        points1.push_back(*p1);

      renderer->fillClippedPolygon(points1, fill);
    }
  }
}

void
CGnuPlotStyleFilledCurves::
addPolygons(CGnuPlotPlot *plot, const Points &points, PointsArray &pointsArray)
{
  const CGnuPlotFilledCurve &filledCurve = plot->filledCurve();

  const CBBox2D &bbox = plot->bbox();

  COptReal x, y, r;

  getLimit(filledCurve, bbox, x, y, r);

  bool bxy = filledCurve.xyval.isValid();

  bool inside = false;

  Points   points1;
  CPoint2D lastPoint;
  bool     lastPointValid = false;

  for (const auto &p : points) {
    bool inside1 = isInside(filledCurve, p, x, y, r);

    if (! inside1) {
      if (inside) {
        CPoint2D pi;

        if (interpPoint(lastPoint, p, x, y, r, pi))
          points1.push_back(pi);

        closePoints(points1, x, y, r);

        pointsArray.push_back(points1);

        points1.clear();

        inside = false;
      }
    }
    else {
      if (! inside) {
        if (! bxy) {
          if (! lastPointValid)
            lastPoint = p;

          CPoint2D pi;

          if (interpPoint(lastPoint, p, x, y, r, pi))
            points1.push_back(pi);
        }

        inside = true;
      }
    }

    if (inside)
      points1.push_back(p);

    lastPoint      = p;
    lastPointValid = true;
  }

  if (inside) {
    if (! points1.empty()) {
      if (! bxy) {
        CPoint2D pi;

        if (interpPoint(lastPoint, lastPoint, x, y, r, pi))
          points1.push_back(pi);
      }
      else
        points1.push_back(filledCurve.xyval.getValue());

      closePoints(points1, x, y, r);

      pointsArray.push_back(points1);
    }
  }
}

void
CGnuPlotStyleFilledCurves::
drawPolygon(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, int i, Points &points)
{
  if (! renderer->isPseudo() && plot->isCacheActive()) {
    CGnuPlotPolygonObject *polygon = plot->polygonObjects()[i];

    polygon->setPoints(points);

    if (! polygon->testAndSetUsed()) {
      CGnuPlotFillP   fill  (polygon->fill  ()->dup());
      CGnuPlotStrokeP stroke(polygon->stroke()->dup());

      polygon->setFill  (fill  );
      polygon->setStroke(stroke);
    }

    polygon->setClipped(true);
  }
  else {
    // fill and stroke polygon
    CGnuPlotFill   fill  (plot);
    CGnuPlotStroke stroke(plot);

    renderer->fillClippedPolygon  (points, fill);
    renderer->strokeClippedPolygon(points, stroke);
  }
}

void
CGnuPlotStyleFilledCurves::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotFill   fill(plot);
  CGnuPlotStroke stroke(plot);

  double h = renderer->pixelHeightToWindowHeight(4);

  CBBox2D cbbox(p1.x, p1.y - h, p2.x, p1.y + h);

  // fill and stroke rectangle
  renderer->fillRect  (cbbox, fill);
  renderer->strokeRect(cbbox, stroke);
}
