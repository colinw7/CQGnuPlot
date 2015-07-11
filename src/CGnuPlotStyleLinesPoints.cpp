#include <CGnuPlotStyleLinesPoints.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleLinesPoints::
CGnuPlotStyleLinesPoints() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::LINES_POINTS)
{
}

void
CGnuPlotStyleLinesPoints::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA c = lineStyle.calcColor(group, CRGBA(1,0,0));

  CRGBA bg = group->window()->backgroundColor();

  int pi = lineStyle.calcPointInterval();

  bool erasePoint = (pi < 0);

  int pi1 = abs(pi);

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
      renderer->drawClipLine(p1, p2, lw, lineStyle.calcColor(CRGBA(1,0,0)), lineStyle.calcDash());
  }
#endif

  //------

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  uint pointNum = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    CPoint2D p(0, 0);

    uint valueNum = 0;

    if      (reals.size() == 1) {
      p.x = pointNum;
      p.y = reals[valueNum++];
    }
    else if (reals.size() >= 2) {
      p.x = reals[valueNum++];
      p.y = reals[valueNum++];
    }

    group->mapLogPoint(plot->xind(), plot->yind(), 1, p);

    double size1 = pointSize;

    if ((! isCalcColor && valueNum < reals.size()) ||
        (  isCalcColor && valueNum < reals.size() - 1))
      size1 = reals[valueNum++];

    CRGBA c1 = c;

    if (isCalcColor && valueNum < reals.size()) {
      double x = reals[valueNum++];

      c1 = lineStyle.calcColor(plot, x);
    }

    if (pi1 == 0) {
      if (erasePoint) {
        double pw = renderer->pixelWidthToWindowWidth  (8*size1);
        double ph = renderer->pixelHeightToWindowHeight(8*size1);

        renderer->fillRect(CBBox2D(p - CPoint2D(pw/2, ph/2), p + CPoint2D(pw/2, ph/2)), bg);
      }

      if (pointType == CGnuPlotTypes::SymbolType::STRING)
        renderer->drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                                   plot->pointTypeStr(), c1);
      else
        renderer->drawSymbol(p, pointType, size1, c1);

      pi1 = abs(pi);
    }

    if (pi1 > 0)
      --pi1;
  }
}

void
CGnuPlotStyleLinesPoints::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  //bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  //------

  for (const auto &ip : plot->getPoints3D()) {
    uint np = ip.second.size();

    CPoint3D p1;

    uint i = 0;

    typedef std::vector<CPoint3D> Points;

    while (i < np) {
      // find first point
      for ( ; i < np; ++i) {
        const CGnuPlotPoint &point1 = ip.second[i];

        if (plot->mapPoint3D(point1, p1))
          break;
      }

      ++i;

      Points points;

      points.push_back(p1);

      // get next continuous points
      for ( ; i < np; ++i) {
        CPoint3D p2;

        const CGnuPlotPoint &point2 = ip.second[i];

        if (! plot->mapPoint3D(point2, p2) || point2.isDiscontinuity())
          break;

        points.push_back(p2);

        p1 = p2;
      }

      // TODO: clip
      double lw = lineStyle.calcWidth();

      renderer->drawPath(points, lw, c, lineStyle.calcDash());
    }
  }

  //------

  double size = plot->pointSize();

  uint pointNum = 0;

  for (const auto &ip : plot->getPoints3D()) {
    for (const auto &point : ip.second) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      CPoint3D p(0, 0, 0);

      uint valueNum = 0;

      if      (reals.size() == 2) {
        p.x = reals[valueNum++];
        p.y = reals[valueNum++];
        p.z = pointNum;
      }
      else if (reals.size() >= 3) {
        p.x = reals[valueNum++];
        p.y = reals[valueNum++];
        p.z = reals[valueNum++];
      }

      renderer->drawSymbol(p, plot->pointType(), size, c);
    }
  }
}

void
CGnuPlotStyleLinesPoints::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CPoint2D pm = (p1 + p2)/2;

  double lw = lineStyle.calcWidth();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  renderer->drawLine(p1, p2, lw, c);

  renderer->drawSymbol(pm, plot->pointType(), plot->pointSize(), c);
}

CBBox2D
CGnuPlotStyleLinesPoints::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
