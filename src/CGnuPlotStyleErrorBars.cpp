#include <CGnuPlotStyleErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotErrorBarData.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleErrorBars::
CGnuPlotStyleErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ERRORBARS)
{
}

void
CGnuPlotStyleErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //------

  // calc bar size
  double bs = plot->barsSize();

  double bw = 0;

  if (! renderer->isPseudo())
    bw = renderer->pixelWidthToWindowWidth(4*bs);

  //------

  typedef std::vector<CGnuPlotErrorBarData> BarDatas;
  typedef std::map<std::string,int>         IndexMap;

  BarDatas barDatas;
  IndexMap indexMap;

  for (const auto &point : plot->getPoints2D()) {
    double x = 0;

    if (! point.getValue(1, x)) {
      std::string str;

      if (point.getValue(1, str)) {
        auto p = indexMap.find(str);

        if (p == indexMap.end()) {
          int ind = 0;

          auto p1 = indexMap.rbegin();

          if (p1 != indexMap.rend())
            ind = (*p1).second;

          p = indexMap.insert(p, IndexMap::value_type(str, ind));
        }

        x = (*p).second;
      }
    }

    double y = 0;

    (void) point.getValue(2, y);

    //---

    double yl = y;
    double yh = y;

    CRGBA lc = stroke.color();

    // x y ylow yhigh
    if      ((! isCalcColor && point.getNumValues() == 4) ||
             (  isCalcColor && point.getNumValues() == 5)) {
      (void) point.getValue(3, yl);
      (void) point.getValue(4, yh);

      if (isCalcColor) {
        double z;

        (void) point.getValue(5, z);

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }
    // x y ydelta
    else if (point.getNumValues() >= 3) {
      double dy;

      (void) point.getValue(3, dy);

      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double z;

        (void) point.getValue(4, z);

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }

    //---

    CPoint2D p (x, y );
    CPoint2D pl(x, yl);
    CPoint2D ph(x, yh);

    p  = group->mapLogPoint(plot->xind(), plot->yind(), 1, p );
    pl = group->mapLogPoint(plot->xind(), plot->yind(), 1, pl);
    ph = group->mapLogPoint(plot->xind(), plot->yind(), 1, ph);

    //---

    CGnuPlotErrorBarData barData;

    barData.setX(p.x);
    barData.setY(p.y);

    barData.setYDirection(true);

    barData.setYLow (pl.y);
    barData.setYHigh(ph.y);

    barData.setBarWidth (bw);
    barData.setPointSize(pointSize);
    barData.setPointType(pointType);
    barData.setLineColor(lc);
    barData.setLineWidth(1);

    barDatas.push_back(barData);
  }

  if (! renderer->isPseudo())
    plot->updateErrorBarCacheSize(barDatas.size());

  int i = 0;

  for (const auto &barData : barDatas) {
    if (! renderer->isPseudo()) {
      CGnuPlotErrorBarObject *bar = plot->errorBarObjects()[i];

      bar->setX(barData.x());
      bar->setY(barData.y());

      bar->setYDirection(true);

      bar->setYLow (barData.yLow ());
      bar->setYHigh(barData.yHigh());

      if (! bar->testAndSetUsed()) {
        bar->setBarWidth(barData.barWidth());

        CGnuPlotStrokeP stroke1(plot->createStroke());

        stroke1->setColor(barData.lineColor());
        stroke1->setWidth(barData.lineWidth());

        bar->setStroke(stroke1);

        CGnuPlotMarkP mark(plot->createMark());

        mark->setType(barData.pointType());
        mark->setSize(barData.pointSize());

        bar->setMark(mark);
      }
    }
    else {
      barData.draw(renderer);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->errorBarObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleErrorBars::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //----

  renderer->drawLine(p1, p2, stroke.color(), stroke.width(), stroke.lineDash());

  double ph = renderer->pixelHeightToWindowHeight(4);

  CPoint2D dy(0, ph);

  renderer->drawLine(p1 - dy, p1 + dy, stroke.color(), stroke.width(), stroke.lineDash());
  renderer->drawLine(p2 - dy, p2 + dy, stroke.color(), stroke.width(), stroke.lineDash());

  CPoint2D pm = (p1 + p2)/2;

  renderer->drawSymbol(pm, pointType, pointSize, stroke.color(),
                       stroke.width(), /*pixelSize*/true);
}
