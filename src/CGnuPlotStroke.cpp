#include <CGnuPlotStroke.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPlot.h>

CGnuPlotStroke::
CGnuPlotStroke(CGnuPlotPlot *plot) :
 plot_(plot)
{
  if (plot)
    init(plot);
}

CGnuPlotStroke *
CGnuPlotStroke::
dup() const
{
  return new CGnuPlotStroke(*this);
}

void
CGnuPlotStroke::
init(CGnuPlotPlot *plot)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  enabled_ = fillStyle.hasBorder();

  //CRGBA c = (fillStyle.style() == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));
  CRGBA c = CRGBA(0,0,0);

  color_ = lineStyle.calcColor(plot->group(), c);
  width_ = lineStyle.calcWidth();
  dash_  = lineStyle.calcDash();

  if (fillStyle.borderColor().isValid())
    color_ = fillStyle.borderColor().getValue().calcColor(plot->group());
}
