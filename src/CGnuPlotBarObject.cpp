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

CGnuPlotTipData
CGnuPlotBarObject::
tip() const
{
  CGnuPlotTipData tip;

  if (x_str_ != "")
    tip.setXStr(x_str_);
  else
    tip.setXStr(CStrUtil::strprintf("%g", x_));

  if (y_str_ != "")
    tip.setYStr(y_str_);
  else
    tip.setYStr(CStrUtil::strprintf("%g", y_));

  tip.setRect(bbox_);

  tip.setXColor(CRGBA(0,0,0));
  tip.setYColor(CRGBA(0,0,0));

  if (fillType_ == FillType::SOLID) {
    tip.setBorderColor(fillColor_.getValue(CRGBA(1,0,0)));
    tip.setXColor     (fillColor_.getValue(CRGBA(1,0,0)));
  }

  return tip;
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
    renderer->drawRect(bbox_, lineColor_.getValue(CRGBA(0,0,0)), width_);
}
