#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotBubbleObject::
CGnuPlotBubbleObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotBubbleObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  double r = data.window.distanceTo(c_);

  if (r > xr_ || r > yr_)
    return false;

  return true;
}

CGnuPlotTipData
CGnuPlotBubbleObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(name_);
  tip.setYStr(CStrUtil::strprintf("%g", value_));

  CPoint2D d(xr_, yr_);

  CBBox2D rect(c_ - d, c_ + d);

  tip.setRect(rect);

  CRGBA c = color_.getValue(CRGBA(0,0,0));

  tip.setBorderColor(c);
  tip.setXColor(c);

  return tip;
}

void
CGnuPlotBubbleObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CRGBA fc = color_.getValue(CRGBA(1,1,1));

  if (highlighted)
    fc = fc.getLightRGBA();

  renderer->fillEllipse(c_, xr_, yr_, 0, fc);

  double s = renderer->fontSize();

  renderer->setFontSize(6);

  CRGBA tc(0, 0, 0);

  if (highlighted)
    tc = CRGBA(1, 1, 1);

  renderer->drawHAlignedText(c_, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0, name_, tc);

  renderer->setFontSize(s);
}
