#include <CGnuPlotStyleFilledCurves.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotUtil.h>
#include <CMathGeom2D.h>

CGnuPlotStyleFilledCurves::
CGnuPlotStyleFilledCurves() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::FILLEDCURVES)
{
}

void
CGnuPlotStyleFilledCurves::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  //const CBBox2D &bbox = plot->getBBox();

  //double xmin = bbox.getXMin();
  //double ymin = bbox.getYMin();
  //double xmax = bbox.getYMax();
  //double ymax = bbox.getYMax();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  //const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  //---

  bool fillbetween = false;

  const CGnuPlot::FilledCurve &filledCurve = plot->filledCurve();

  if (! filledCurve.xaxis && ! filledCurve.yaxis && ! filledCurve.xyval.isValid()) {
    if (! plot->getPoints2D().empty() && plot->getPoints2D()[0].getNumValues() > 2)
      fillbetween = true;
  }

  //---

  if (! fillbetween) {
    typedef std::vector<CPoint2D> Points;
    typedef std::vector<Points>   PointsVector;

    PointsVector pointsVector;

    uint np = plot->numPoints2D();

    CPoint2D p1;

    uint i = 0;

    while (i < np) {
      for ( ; i < np; ++i) {
        const CGnuPlotPoint &point1 = plot->getPoint2D(i);

        if (point1.getPoint(p1))
          break;
      }

      ++i;

      Points points;

      points.push_back(p1);

      for ( ; i < np; ++i) {
        CPoint2D p2;

        const CGnuPlotPoint &point2 = plot->getPoint2D(i);

        if (! point2.getPoint(p2) || point2.isDiscontinuity())
          break;

        points.push_back(p2);

        p1 = p2;
      }

      if (! points.empty())
        pointsVector.push_back(points);
    }

    if (! renderer->isPseudo() && plot->isCacheActive()) {
      plot->updatePolygonCacheSize(pointsVector.size());
    }

    int pi = 0;

    for (auto &points : pointsVector) {
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

      if (point.getXYZ(x, y1, y2)) {
        CPoint2D p1(x, y1);
        CPoint2D p2(x, y2);

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

              renderer->fillClippedPolygon(points1, lineStyle.calcColor(CRGBA(1,1,1)));
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

      renderer->fillClippedPolygon(points1, lineStyle.calcColor(CRGBA(1,1,1)));
    }
  }
}

void
CGnuPlotStyleFilledCurves::
drawPolygon(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, int i, std::vector<CPoint2D> &points)
{
  if (points[0] != points[points.size() - 1])
    points.push_back(points[0]);

  const CBBox2D &bbox = plot->getBBox();

  double xmin = bbox.getXMin();
  double ymin = bbox.getYMin();
  double xmax = bbox.getYMax();
  double ymax = bbox.getYMax();

  bool inside = false;

  const CGnuPlot::FilledCurve &filledCurve = plot->filledCurve();
  const CGnuPlotLineStyle     &lineStyle   = plot->lineStyle();
  const CGnuPlotFillStyle     &fillStyle   = plot->fillStyle();

  COptReal x, y;

  if      (filledCurve.yval.isValid())
    y = filledCurve.yval.getValue();
  else if (filledCurve.xaxis == 1)
    y = ymin;
  else if (filledCurve.xaxis == 2)
    y = ymax;
  else if (filledCurve.yaxis == 1)
    x = xmin;
  else if (filledCurve.yaxis == 2)
    x = xmax;

  bool bxy = filledCurve.xyval.isValid();

  std::vector<CPoint2D> points1;

  for (const auto &p : points) {
    if ((filledCurve.above && ((x.isValid() && x.getValue() > p.x) ||
                               (y.isValid() && y.getValue() > p.y))) ||
        (filledCurve.below && ((x.isValid() && x.getValue() < p.x) ||
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
        points1.push_back(filledCurve.xyval.getValue());
    }
  }

  //-----

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    CGnuPlotPolygonObject *polygon = plot->polygonObjects()[i];

    polygon->setPoints(points1);

    if (! polygon->isModified()) {
      polygon->resetFillColor();
      polygon->resetLineColor();
    }

    if (fillStyle.style() == CGnuPlotTypes::FillType::PATTERN) {
      // TODO
    }
    else if (fillStyle.style() == CGnuPlotTypes::FillType::SOLID) {
      CRGBA fc = lineStyle.calcColor(CRGBA(1,1,1));

      if (fillStyle.isTransparent())
        fc.setAlpha(fillStyle.density());

      if (! polygon->isModified())
        polygon->setFillColor(fc);
    }

    if (fillStyle.hasBorder()) {
      // border line type ?
      CRGBA lc = CRGBA(0,0,0);

      if (fillStyle.borderColor().isValid())
        lc = fillStyle.borderColor().getValue().calcColor(plot);

      if (! polygon->isModified()) {
        polygon->setLineWidth(lineStyle.width());
        polygon->setLineColor(lc);
      }
    }

    polygon->setClipped(true);
  }
  else {
    // fill polygon
    if (fillStyle.style() == CGnuPlotTypes::FillType::PATTERN) {
      CRGBA fg = lineStyle.calcColor(CRGBA(1,1,1));
      CRGBA bg = plot->window()->backgroundColor();

      renderer->patternClippedPolygon(points1, fillStyle.pattern(), fg, bg);
    }
    else if (fillStyle.style() == CGnuPlotTypes::FillType::SOLID) {
      CRGBA fc = lineStyle.calcColor(CRGBA(1,1,1));

      if (fillStyle.isTransparent())
        fc.setAlpha(fillStyle.density());

      renderer->fillClippedPolygon(points1, fc);
    }

    //-----

    // draw border
    if (fillStyle.hasBorder()) {
      // border line type ?
      CRGBA lc = CRGBA(0,0,0);

      if (fillStyle.borderColor().isValid())
        lc = fillStyle.borderColor().getValue().calcColor(plot);

      renderer->drawClippedPolygon(points1, lineStyle.width(), lc);
    }
  }
}

void
CGnuPlotStyleFilledCurves::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  double h = renderer->pixelHeightToWindowHeight(4);

  CBBox2D cbbox(p1.x, p1.y - h, p2.x, p1.y + h);

  renderer->fillRect(cbbox, lineStyle.calcColor(CRGBA(1,1,1)));
}

CBBox2D
CGnuPlotStyleFilledCurves::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
