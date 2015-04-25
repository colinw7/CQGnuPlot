#include <CGnuPlotStyleLabels.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleLabels::
CGnuPlotStyleLabels() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::LABELS)
{
}

void
CGnuPlotStyleLabels::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  bool enhanced = plot->app()->device()->isEnhanced();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 3)
      continue;

    CPoint2D p(reals[0], reals[1]);

    std::string str;

    if (! point.getValue(3, str))
      continue;

    CRGBA lc1 = lc;

    if (isCalcColor && reals.size() > 3) {
      double x = reals[3];

      lc1 = lineStyle.color().calcColor(plot, x);
    }

    //----

    if (enhanced) {
      CGnuPlotText text(str);

      CBBox2D bbox = text.calcBBox(renderer).moveBy(p);

      bbox.setYMax(p.y);

      text.draw(renderer, bbox, CHALIGN_TYPE_LEFT, lc1);
    }
    else
      renderer->drawText(p, str, lc1);
  }
}

CBBox2D
CGnuPlotStyleLabels::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
