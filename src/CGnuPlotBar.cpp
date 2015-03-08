#include <CGnuPlotBar.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotBar::
CGnuPlotBar(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotBar::
inside(const CPoint2D &p) const
{
  return bbox_.inside(p);
}

void
CGnuPlotBar::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isInitialized()) return;

  if      (fillType_ == FillType::PATTERN)
    renderer->patternRect(bbox_, fillPattern_, fillColor_.getValue(CRGBA(1,0,0)));
  else if (fillType_ == FillType::SOLID)
    renderer->fillRect(bbox_, fillColor_.getValue(CRGBA(1,0,0)));

  if (border_)
    renderer->drawRect(bbox_, lineColor_.getValue(CRGBA(0,0,0)));
}
