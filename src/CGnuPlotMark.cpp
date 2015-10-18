#include <CGnuPlotMark.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPlot.h>

CGnuPlotMark::
CGnuPlotMark(CGnuPlotPlot *plot) :
 plot_(plot)
{
  if (plot)
    init(plot);
}

CGnuPlotMark *
CGnuPlotMark::
dup() const
{
  return new CGnuPlotMark(*this);
}

void
CGnuPlotMark::
init(CGnuPlotPlot *plot)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  size_  = plot->pointSize();
  type_  = plot->pointType();
  color_ = lineStyle.calcColor(plot->group(), CRGBA(0,0,0));
  width_ = lineStyle.calcWidth();
}
