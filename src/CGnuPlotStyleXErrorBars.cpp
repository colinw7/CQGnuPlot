#include <CGnuPlotStyleXErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotErrorBarData.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleXErrorBars::
CGnuPlotStyleXErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::XERRORBARS)
{
}

void
CGnuPlotStyleXErrorBars::
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

  double bh = 0;

  if (! renderer->isPseudo())
    bh = renderer->pixelHeightToWindowHeight(4*bs);

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

    double xl = x;
    double xh = x;

    CRGBA lc = stroke.color();

    // x y xlow xhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      xl = reals[2];
      xh = reals[3];

      if (isCalcColor) {
        double z = reals[4];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }
    // x y xdelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dx = reals[2];

      xl = x - dx;
      xh = x + dx;

      if (isCalcColor) {
        double z = reals[3];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }

    //---

    CPoint2D p (x , y);
    CPoint2D pl(xl, y);
    CPoint2D ph(xh, y);

    p  = group->mapLogPoint(plot->xind(), plot->yind(), 1, p );
    pl = group->mapLogPoint(plot->xind(), plot->yind(), 1, pl);
    ph = group->mapLogPoint(plot->xind(), plot->yind(), 1, ph);

    //---

    CGnuPlotErrorBarData barData;

    barData.setX(p.x);
    barData.setY(p.y);

    barData.setXDirection(true);

    barData.setXLow (pl.x);
    barData.setXHigh(ph.x);

    barData.setBarHeight(bh);
    barData.setPointSize(pointSize);
    barData.setPointType(pointType);
    barData.setLineColor(lc);
    barData.setLineWidth(1);

    barDatas.push_back(barData);
  }

  if (! renderer->isPseudo())
    plot->updateErrorBarCacheSize(int(barDatas.size()));

  int i = 0;

  for (const auto &barData : barDatas) {
    if (! renderer->isPseudo()) {
      CGnuPlotErrorBarObject *bar = plot->errorBarObjects()[i];

      bar->setX(barData.x());
      bar->setY(barData.y());

      bar->setXDirection(true);

      bar->setXLow (barData.xLow ());
      bar->setXHigh(barData.xHigh());

      if (! bar->testAndSetUsed()) {
        bar->setBarHeight(barData.barHeight());

        CGnuPlotStrokeP stroke1(plot->createStroke());

        stroke1->setColor(barData.lineColor());
        stroke1->setWidth(barData.lineWidth());

        CGnuPlotMarkP mark(plot->createMark());

        mark->setType(barData.pointType());
        mark->setSize(barData.pointSize());

        bar->setStroke(stroke1);
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
CGnuPlotStyleXErrorBars::
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
