#include <CGnuPlotBarObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotBarObject::
CGnuPlotBarObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotBarObject::
inside(const CPoint2D &p) const
{
  return bbox_.inside(p);
}

std::string
CGnuPlotBarObject::
tip() const
{
  return CStrUtil::toString(value_);
}

void
CGnuPlotBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isInitialized()) return;

  if      (fillType_ == FillType::PATTERN)
    renderer->patternRect(bbox_, fillPattern_, fillColor_.getValue(CRGBA(1,0,0)), CRGBA(1,1,1));
  else if (fillType_ == FillType::SOLID)
    renderer->fillRect(bbox_, fillColor_.getValue(CRGBA(1,0,0)));

  if (border_)
    renderer->drawRect(bbox_, lineColor_.getValue(CRGBA(0,0,0)), 1);
}
