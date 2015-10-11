#include <CGnuPlotStyleLinesPoints.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPathObject.h>
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

  const CGnuPlotLineStyle  &lineStyle  = plot->lineStyle();
  const CGnuPlotLabelStyle &labelStyle = plot->labelStyle();

  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  //------

  typedef std::vector<CPoint2D> Points;
  typedef std::vector<Points>   PointsArray;

  PointsArray pointsArray;

  uint i  = 0;
  uint np = plot->numPoints2D();

  while (i < np) {
    CPoint2D p1;

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
    pointsArray.push_back(points);
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    plot->updatePathCacheSize(pointsArray.size());

    int j = 0;

    for (const auto &points : pointsArray) {
      CGnuPlotPathObject *path = plot->pathObjects()[j];

      path->setPoints(points);

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

  //------

  typedef std::vector<CGnuPlotPointData> PointDatas;

  PointDatas pointDatas;

  int  pi         = lineStyle.calcPointInterval();
  int  pi1        = abs(pi);
  bool erasePoint = (pi < 0);

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = lineStyle.calcPointType();

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    CPoint2D p(0, 0);

    uint valueNum = 0;

    if      (reals.size() == 1) {
      p.x = pointDatas.size();
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

    CRGBA c1 = stroke.color();

    if (isCalcColor && valueNum < reals.size()) {
      double z = reals[valueNum++];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        c1 = lineStyle.calcColor(plot, z);
    }

    CRGBA tc = c1;

    if (labelStyle.textColor().isValid())
      tc = labelStyle.textColor().getValue().calcColor(group);

    CGnuPlotPointData data;

    data.setPoint      (p);
    data.setPointType  (pointType);
    data.setSize       (size1);
    data.setLineWidth  (stroke.width());
    data.setPointString(plot->pointTypeStr());
    data.setErasePoint (erasePoint);

    if (pointType == CGnuPlotTypes::SymbolType::STRING)
      data.setColor(tc);
    else
      data.setColor(c1);

    if (pi1 == 0) {
      data.setVisible(true);

      pi1 = abs(pi);
    }
    else
      data.setVisible(false);

    pointDatas.push_back(data);

    if (pi1 > 0)
      --pi1;
  }

  //----

  CRGBA bg = fill.background();

  if (! renderer->isPseudo() && lineStyle.isTipPoints())
    plot->updatePointCacheSize(pointDatas.size());

  int pointNum = 0;

  for (const auto &data : pointDatas) {
    if (! renderer->isPseudo() && lineStyle.isTipPoints()) {
      CGnuPlotPointObject *point = plot->pointObjects()[pointNum];

      if (! point->testAndSetUsed()) {
        point->setData(data);
      }
      else {
        point->setPoint(data.point());
      }
    }
    else {
      if (data.isVisible()) {
        double s = data.size().getValue(1);

        if (data.isErasePoint()) {
          double pw = renderer->pixelWidthToWindowWidth  (8*s);
          double ph = renderer->pixelHeightToWindowHeight(8*s);

          CPoint2D d(pw/2, ph/2);

          renderer->fillEllipse(CBBox2D(data.point() - d, data.point() + d), bg);
        }

        if (data.pointType() == CGnuPlotTypes::SymbolType::STRING)
          renderer->drawHAlignedText(data.point(), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                                     data.pointString(), data.color());
        else
          renderer->drawSymbol(data.point(), data.pointType(), s,
                               data.color(), data.lineWidth(), /*pixelSize*/true);
      }
    }

    ++pointNum;
  }

  if (! renderer->isPseudo()) {
    for (const auto &path : plot->pathObjects())
      path->draw(renderer);

    for (const auto &point : plot->pointObjects())
      point->draw(renderer);
  }
}

void
CGnuPlotStyleLinesPoints::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  //const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  //bool isCalcColor = lineStyle.color().isCalc();

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
      renderer->drawPath(points, stroke.width(), stroke.color(), stroke.lineDash());
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

      renderer->drawSymbol(p, plot->pointType(), size, stroke.color(), stroke.width(), true);
    }
  }
}

void
CGnuPlotStyleLinesPoints::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  CPoint2D pm = (p1 + p2)/2;

  CRGBA lc = stroke.color();

  if (! plot->isDisplayed())
    lc.setAlpha(0.5);

  renderer->drawLine(p1, p2, stroke.width(), lc);

  if (plot->pointType() == CGnuPlotTypes::SymbolType::STRING) {
    const CGnuPlotLabelStyle &labelStyle = plot->labelStyle();

    CRGBA tc = lc;

    if (labelStyle.textColor().isValid())
      tc = labelStyle.textColor().getValue().calcColor(plot->group());

    renderer->drawHAlignedText(pm, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                               plot->pointTypeStr(), tc);
  }
  else
    renderer->drawSymbol(pm, plot->pointType(), plot->pointSize(), lc, stroke.width(), true);
}

CBBox2D
CGnuPlotStyleLinesPoints::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
