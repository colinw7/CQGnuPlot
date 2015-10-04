#include <CGnuPlotStyleLinesPoints.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPointObject.h>

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

  int       pi = lineStyle.calcPointInterval();
  double    lw = lineStyle.calcWidth();
  CLineDash ld = lineStyle.calcDash();

  bool erasePoint = (pi < 0);

  int pi1 = abs(pi);

  //------

  uint np = plot->numPoints2D();

  if (! renderer->isPseudo() && lineStyle.isTipPoints())
    plot->updatePointCacheSize(np);

  //------

  CPoint2D p1;

  uint i = 0;

  typedef std::vector<CPoint2D> Points;

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

    if (plot->isPolar()) {
      bool inside;

      p1 = group->convertPolarAxisPoint(p1, inside);
    }

    p1 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);

    points.push_back(p1);

    // add points until discontinuity
    for ( ; i < np; ++i) {
      CPoint2D p2;

      const CGnuPlotPoint &point2 = plot->getPoint2D(i);

      if (! point2.getPoint(p2) || point2.isDiscontinuity())
        break;

      if (renderer->isPseudo() && ! renderer->isInside(p1))
        break;

      if (plot->isPolar()) {
        bool inside;

        p2 = group->convertPolarAxisPoint(p2, inside);
      }

      p2 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);

      points.push_back(p2);

      p1 = p2;
    }

    // TODO: clip
    renderer->drawPath(points, lw, c, ld);
  }

#if 0
  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = getPoint2D(i1);
    const CGnuPlotPoint &point2 = getPoint2D(i2);

    CPoint2D p1, p2;

    if (! point1.isDiscontinuity() && point1.getPoint(p1) && point2.getPoint(p2))
      renderer->drawClipLine(p1, p2, lw, c, ld));
  }
#endif

  //------

  double pointSize = plot->pointSize();

//CGnuPlotTypes::SymbolType pointType = plot->pointType();
  CGnuPlotTypes::SymbolType pointType = lineStyle.calcPointType();

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

    p = group->mapLogPoint(plot->xind(), plot->yind(), 1, p);

    double size1 = pointSize;

    if ((! isCalcColor && valueNum < reals.size()) ||
        (  isCalcColor && valueNum < reals.size() - 1))
      size1 = reals[valueNum++];

    CRGBA c1 = c;

    if (isCalcColor && valueNum < reals.size()) {
      double z = reals[valueNum++];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        c1 = lineStyle.calcColor(plot, z);
    }

    if (! renderer->isPseudo() && lineStyle.isTipPoints()) {
      CGnuPlotPointObject *point = plot->pointObjects()[pointNum];

      point->setPoint      (p);
      point->setPointType  (pointType);
      point->setSize       (size1);
      point->setColor      (c1);
      point->setLineWidth  (lw);
      point->setPointString(plot->pointTypeStr());
      point->setErasePoint (erasePoint);
    }
    else {
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
          renderer->drawSymbol(p, pointType, size1, c1, lw, true);

        pi1 = abs(pi);
      }

      if (pi1 > 0)
        --pi1;
    }

    ++pointNum;
  }

  if (! renderer->isPseudo()) {
    for (const auto &point : plot->pointObjects())
      point->draw(renderer);
  }
}

void
CGnuPlotStyleLinesPoints::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  //bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double lw = lineStyle.calcWidth();

  //------

  for (const auto &ip : plot->getPoints3D()) {
    uint np = ip.second.size();

    CPoint3D p1;
    int      ind1;

    uint i = 0;

    typedef std::vector<CPoint3D> Points;

    while (i < np) {
      // find first point
      for ( ; i < np; ++i) {
        const CGnuPlotPoint &point1 = ip.second[i];

        if (plot->mapPoint3D(point1, p1, ind1))
          break;
      }

      ++i;

      Points points;

      points.push_back(p1);

      // get next continuous points
      for ( ; i < np; ++i) {
        CPoint3D p2;
        int      ind2;

        const CGnuPlotPoint &point2 = ip.second[i];

        if (! plot->mapPoint3D(point2, p2, ind2) || point2.isDiscontinuity())
          break;

        points.push_back(p2);

        p1 = p2;
      }

      // TODO: clip
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

      renderer->drawSymbol(p, plot->pointType(), size, c, lw, true);
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

  if (! plot->isDisplayed())
    c.setAlpha(0.5);

  renderer->drawLine(p1, p2, lw, c);

  renderer->drawSymbol(pm, plot->pointType(), plot->pointSize(), c, lw, true);
}

CBBox2D
CGnuPlotStyleLinesPoints::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
