#include <CGnuPlotStyleLines.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleLines::
CGnuPlotStyleLines() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::LINES)
{
}

void
CGnuPlotStyleLines::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(group, CRGBA(1,0,0));

  //------

  uint np = plot->numPoints2D();

  CPoint2D p1;

  uint i = 0;

  typedef std::vector<CPoint2D> Points;

  while (i < np) {
    // get first point
    for ( ; i < np; ++i) {
      const CGnuPlotPoint &point1 = plot->getPoint2D(i);

      if (point1.getPoint(p1))
        break;
    }

    ++i;

    Points points;

    if (plot->isPolar()) {
      bool inside;

      p1 = group->convertPolarAxisPoint(p1, inside);
    }

    group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);

    points.push_back(p1);

    // add points until discontinuity
    for ( ; i < np; ++i) {
      CPoint2D p2;

      const CGnuPlotPoint &point2 = plot->getPoint2D(i);

      if (! point2.getPoint(p2) || point2.isDiscontinuity())
        break;

      if (plot->isPolar()) {
        bool inside;

        p2 = group->convertPolarAxisPoint(p2, inside);
      }

      group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);

      points.push_back(p2);

      p1 = p2;
    }

    // TODO: clip
    double lw = lineStyle.calcWidth();

    renderer->drawPath(points, lw, c, lineStyle.calcDash());
  }

#if 0
  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (! point1.isDiscontinuity() && point1.getPoint(p1) && point2.getPoint(p2))
      renderer->drawClipLine(p1, p2, lw, c), lineStyle.calcDash());
  }
#endif
}

void
CGnuPlotStyleLines::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  CGnuPlotGroup *group = plot->group();

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

    uint i = 0, j = 0;

    while (i < np) {
      // find first point
      for ( ; i < np; ++i, ++j) {
        const CGnuPlotPoint &point1 = ip.second[i];

        if (plot->mapPoint3D(point1, p1))
          break;
      }

      Points points;

      group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);

      xpoints[j].push_back(p1);

      points.push_back(p1);

      ++i; ++j;

      // get next continuous points
      for ( ; i < np; ++i, ++j) {
        CPoint3D p2;

        const CGnuPlotPoint &point2 = ip.second[i];

        if (! plot->mapPoint3D(point2, p2) || point2.isDiscontinuity())
          break;

        group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);

        xpoints[j].push_back(p2);

        points.push_back(p2);

        p1 = p2;
      }

      // TODO: clip
      double lw = lineStyle.calcWidth();

      renderer->drawPath(points, lw, c, lineStyle.calcDash());

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
      double lw = lineStyle.calcWidth();

      renderer->drawPath(ip.second, lw, c, lineStyle.calcDash());
    }
  }
}

void
CGnuPlotStyleLines::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle= plot->lineStyle();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double lw = lineStyle.calcWidth();

  renderer->drawLine(p1, p2, lw, c, lineStyle.calcDash());
}

CBBox2D
CGnuPlotStyleLines::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
