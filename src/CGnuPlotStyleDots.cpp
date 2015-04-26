#include <CGnuPlotStyleDots.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleDots::
CGnuPlotStyleDots() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::DOTS)
{
}

void
CGnuPlotStyleDots::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  const CRGBA &c = lineStyle.calcColor(CRGBA(1,0,0));

  for (const auto &point : plot->getPoints2D()) {
    CPoint2D p;

    if (point.getPoint(p))
      renderer->drawPoint(p, c);
  }
}

CBBox2D
CGnuPlotStyleDots::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
