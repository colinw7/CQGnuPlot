#include <CGnuPlotStylePoints.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPointObject.h>

CGnuPlotStylePoints::
CGnuPlotStylePoints() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::POINTS)
{
}

void
CGnuPlotStylePoints::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle  &lineStyle  = plot->lineStyle();
  const CGnuPlotLabelStyle &labelStyle = plot->labelStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //------

  typedef std::vector<CGnuPlotPointData> PointDatas;

  PointDatas pointDatas;

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

    if (renderer->isPseudo() && ! renderer->isInside(p))
      continue;

    p = group->mapLogPoint(plot->xind(), plot->yind(), 1, p);

    double size1     = pointSize;
    bool   pixelSize = true;

    if ((! isCalcColor && valueNum < reals.size()) ||
        (  isCalcColor && valueNum < reals.size() - 1)) {
      size1     = reals[valueNum++];
      pixelSize = false;
    }

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
    data.setLineWidth  (stroke.width());
    data.setPointString(plot->pointTypeStr());
    data.setErasePoint (false);
    data.setVisible    (true);

    if (pointType == CGnuPlotTypes::SymbolType::STRING)
      data.setColor(tc);
    else
      data.setColor(c1);

    if (size1 > 0)
      data.setSize(size1);
    else
      data.resetSize();

    data.setPixelSize(pixelSize);

    pointDatas.push_back(data);
  }

  //----

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
      double s = data.size().getValue(1);

      if (data.pointType() == CGnuPlotTypes::SymbolType::STRING)
        renderer->drawHAlignedText(data.point(), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                                   data.pointString(), data.color());
      else
        renderer->drawSymbol(data.point(), data.pointType(), s,
                             data.color(), data.lineWidth(), /*pixelSize*/true);
    }

    ++pointNum;
  }

  if (! renderer->isPseudo()) {
    for (const auto &point : plot->pointObjects())
      point->draw(renderer);
  }
}

void
CGnuPlotStylePoints::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  //const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  //bool isCalcColor = lineStyle.color().isCalc();

  const CGnuPlotPlot::Point3DArray &originArray = plot->originArray();

  //---

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  int k = 0;

  for (const auto &ip : plot->getPoints3D()) {
    COptPoint3D op;

    if (k < int(originArray.size()))
      op = originArray[k];

    for (const auto &point : ip.second) {
      CPoint3D p;
      int      ind;

      (void) plot->mapPoint3D(point, p, ind);

      if (op.isValid())
        p += op.getValue();

      renderer->drawSymbol(p, pointType, pointSize, stroke.color(), stroke.width(), true);
    }

    ++k;
  }
}

void
CGnuPlotStylePoints::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  CPoint2D pm = (p1 + p2)/2;

  CRGBA lc = stroke.color();

  if (! plot->isDisplayed())
    lc.setAlpha(0.5);

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
