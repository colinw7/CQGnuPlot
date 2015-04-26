#include <CGnuPlotStyleImpulses.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleImpulses::
CGnuPlotStyleImpulses() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::IMPULSES)
{
}

void
CGnuPlotStyleImpulses::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  double ymin = plot->getBBox().getYMin();

  double y2 = std::max(0.0, ymin);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    CPoint2D p1(reals[0], reals[1]);
    CPoint2D p2(reals[0], y2);

    CRGBA lc1 = lc;

    if (reals.size() >= 3 && isCalcColor) {
      double x = reals[2];

      lc1 = lineStyle.color().calcColor(plot, x);
    }

    renderer->drawClipLine(p1, p2, lineStyle.width(), lc1);
  }
}

void
CGnuPlotStyleImpulses::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  renderer->drawLine(p1, p2, lineStyle.width(), lineStyle.calcColor(CRGBA(1,0,0)));
}

CBBox2D
CGnuPlotStyleImpulses::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
