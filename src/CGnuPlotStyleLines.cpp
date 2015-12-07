#include <CGnuPlotStyleLines.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPathObject.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleLines::
CGnuPlotStyleLines() :
 CGnuPlotStylePointsBase(CGnuPlot::PlotStyle::LINES)
{
}

void
CGnuPlotStyleLines::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  CGnuPlotStroke stroke(plot);

  double                            idx         = plot->delta().dx;
  double                            idt         = plot->delta().dt;
  const CGnuPlotRotate             &rotate      = plot->rotate();
  const CGnuPlotPlot::Point3DArray &originArray = plot->originArray();

  //------

  uint np = plot->numPoints2D();

  CPoint2D p1;

  uint i = 0;

  typedef std::vector<CPoint2D> Points;
  typedef std::vector<Points>   PointsArray;

  PointsArray pointsArray;

  while (i < np) {
    // get first point
    for ( ; i < np; ++i) {
      const CGnuPlotPoint &point1 = plot->getPoint2D(i);

      if (plot->isPolar()) {
        if (! point1.getPoint(i*idt, p1, CGnuPlotPoint::GetOpts().setForce(true)))
          continue;
      }
      else {
        if (! point1.getPoint(i*idx, p1, CGnuPlotPoint::GetOpts().setForce(true)))
          continue;
      }

      if (renderer->isPseudo() && ! renderer->isInside(p1))
        continue;

      break;
    }

    COptPoint2D op;

    if (pointsArray.size() < originArray.size()) {
      CPoint3D o = originArray[pointsArray.size()];

      op = CPoint2D(o.x, o.y);
    }

    ++i;

    Points points;

    if (plot->isPolar()) {
      bool inside;

      p1 = plot->convertPolarAxisPoint(p1, inside);
    }
    else {
      p1 = rotate.apply(p1);

      if (op.isValid())
        p1 += op.getValue();
    }

    p1 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);

    points.push_back(p1);

    // add points until discontinuity
    for ( ; i < np; ++i) {
      CPoint2D p2;

      const CGnuPlotPoint &point2 = plot->getPoint2D(i);

      if (plot->isPolar()) {
        if (! point2.getPoint(i*idt, p2, CGnuPlotPoint::GetOpts().setForce(true)) ||
            point2.isDiscontinuity())
          break;
      }
      else {
        if (! point2.getPoint(i*idx, p2, CGnuPlotPoint::GetOpts().setForce(true)) ||
            point2.isDiscontinuity())
          break;
      }

      if (renderer->isPseudo() && ! renderer->isInside(p1))
        break;

      if (plot->isPolar()) {
        bool inside;

        p2 = plot->convertPolarAxisPoint(p2, inside);
      }
      else {
        p2 = rotate.apply(p2);

        if (op.isValid())
          p2 += op.getValue();
      }

      p2 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);

      points.push_back(p2);

      p1 = p2;
    }

    // TODO: clip
    pointsArray.push_back(points);
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    plot->updatePathCacheSize(pointsArray.size());

    int j = 0;

    for (const auto &points : pointsArray) {
      CGnuPlotPathObject *path = plot->pathObjects()[j];

      path->setPoints2D(points);

      if (! path->testAndSetUsed()) {
        CGnuPlotStrokeP stroke1(path->stroke()->dup());

        stroke1->setEnabled (true);
        stroke1->setColor   (stroke.color());
        stroke1->setWidth   (stroke.width());
        stroke1->setLineDash(stroke.lineDash());

        path->setStroke(stroke1);
      }

      ++j;
    }

    for (const auto &path : plot->pathObjects())
      path->draw(renderer);
  }
  else {
    for (const auto &points : pointsArray) {
      renderer->strokeClippedPath(points, stroke);
    }
  }
}

void
CGnuPlotStyleLines::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  CGnuPlotStroke stroke(plot);

  CRGBA c = stroke.color();

  //---

  bool grid = true;
  int  n    = -1;

  // draw grid of lines
  typedef std::vector<CPoint3D> Points;

  typedef std::map<int,Points> XPoints;

  XPoints xpoints;

  for (const auto &ip : plot->getPoints3D()) {
    uint np = ip.second.size();

    CPoint3D p1;
    int      ind1;

    uint i = 0, j = 0;

    while (i < np) {
      // find first point
      for ( ; i < np; ++i, ++j) {
        const CGnuPlotPoint &point1 = ip.second[i];

        if (plot->mapPoint3D(point1, p1, ind1))
          break;
      }

      Points points;

      p1 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);

      xpoints[j].push_back(p1);

      points.push_back(p1);

      ++i; ++j;

      // get next continuous points
      for ( ; i < np; ++i, ++j) {
        CPoint3D p2;
        int      ind2;

        const CGnuPlotPoint &point2 = ip.second[i];

        if (! plot->mapPoint3D(point2, p2, ind2) || point2.isDiscontinuity())
          break;

        p2 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);

        xpoints[j].push_back(p2);

        points.push_back(p2);

        p1 = p2;
      }

      // TODO: clip
      double lw = stroke.width();

      if (group->isHidden3D())
        renderer->drawHiddenClippedPath(points, lw, c, stroke.lineDash());
      else
        renderer->drawClippedPath(points, lw, c, stroke.lineDash());

      if (n < 0)
        n = points.size();

      if (grid && n != int(points.size()))
        grid = false;

      j = 0;
    }
  }

  if (grid) {
    for (const auto &ip : xpoints) {
      // TODO: clip
      double lw = stroke.width();

      if (group->isHidden3D())
        renderer->drawHiddenClippedPath(ip.second, lw, c, stroke.lineDash());
      else
        renderer->drawClippedPath(ip.second, lw, c, stroke.lineDash());
    }
  }
}

void
CGnuPlotStyleLines::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  CRGBA  c  = stroke.color();
  double lw = stroke.width();

  renderer->drawLine(p1, p2, lw, c, stroke.lineDash());
}
