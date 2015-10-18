#include <CGnuPlotStyleYErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotErrorBarData.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleYErrorBars::
CGnuPlotStyleYErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::YERRORBARS)
{
}

void
CGnuPlotStyleYErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //---

  // calc bar size
  double bs = plot->barsSize();

  double bw = 0;

  if (! renderer->isPseudo())
    bw = renderer->pixelWidthToWindowWidth(4*bs);

  //---

  typedef std::vector<CGnuPlotErrorBarData> BarDatas;

  BarDatas barDatas;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    // TODO: index map (see CGnuPlotStyleErrorBars)

    double x = reals[0];
    double y = reals[1];

    double yl = y;
    double yh = y;

    CRGBA lc = stroke.color();

    // x y ylow yhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      yl = reals[2];
      yh = reals[3];

      if (isCalcColor) {
        double z = reals[4];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }
    // x y ydelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dy = reals[2];

      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double z = reals[3];

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

        CGnuPlotStrokeP stroke(plot->createStroke());

        stroke->setColor(barData.lineColor());
        stroke->setWidth(barData.lineWidth());

        CGnuPlotMarkP mark(plot->createMark());

        mark->setType(barData.pointType());
        mark->setSize(barData.pointSize());

        bar->setStroke(stroke);
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
CGnuPlotStyleYErrorBars::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //----

  renderer->drawLine(p1, p2, stroke.width(), stroke.color(), stroke.lineDash());

  double ph = renderer->pixelHeightToWindowHeight(4);

  CPoint2D dy(0, ph);

  renderer->drawLine(p1 - dy, p1 + dy, stroke.width(), stroke.color(), stroke.lineDash());
  renderer->drawLine(p2 - dy, p2 + dy, stroke.width(), stroke.color(), stroke.lineDash());

  CPoint2D pm = (p1 + p2)/2;

  renderer->drawSymbol(pm, pointType, pointSize, stroke.color(),
                       stroke.width(), /*pixelSize*/true);
}

CBBox2D
CGnuPlotStyleYErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
