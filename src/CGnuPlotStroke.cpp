#include <CGnuPlotStroke.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPlot.h>

CGnuPlotStroke::
CGnuPlotStroke(CGnuPlotPlot *plot)
{
  if (plot)
    init(plot);
}

void
CGnuPlotStroke::
init(CGnuPlotPlot *plot)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  enabled_ = fillStyle.hasBorder();

  color_ = lineStyle.calcColor(plot->group(), CRGBA(0, 0, 0));
  width_ = lineStyle.calcWidth();
  dash_  = lineStyle.calcDash();

  if (fillStyle.borderColor().isValid())
    color_ = fillStyle.borderColor().getValue().calcColor(plot->group());
}
