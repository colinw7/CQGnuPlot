#include <CGnuPlotFill.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>

CGnuPlotFill::
CGnuPlotFill(CGnuPlotPlot *plot)
{
  if (plot)
    init(plot);
}

void
CGnuPlotFill::
init(CGnuPlotPlot *plot)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  color_ = lineStyle.calcColor(plot->group(), CRGBA(1, 1, 1));

  if (fillStyle.isTransparent())
    color_.setAlpha(fillStyle.density());

  bg_      = plot->group()->window()->backgroundColor();
  type_    = fillStyle.style();
  pattern_ = fillStyle.pattern();
}
