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
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
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

  bool highlighted = (isHighlighted() || isSelected());

  if      (fillType_ == FillType::PATTERN) {
    CRGBA fc = fillColor_.getValue(CRGBA(1,0,0));

    if (highlighted) {
      double g = fc.getGray();

      if (g < 0.5)
        fc = CRGBA(1, 1, 1);
      else
        fc = CRGBA(0, 0, 0);
    }

    renderer->patternRect(bbox_, fillPattern_, fc, CRGBA(1,1,1));
  }
  else if (fillType_ == FillType::SOLID) {
    CRGBA fc  = fillColor().getValue(CRGBA(1,1,1));

    if (highlighted)
      fc = fc.getLightRGBA();

    renderer->fillRect(bbox_, fc);
  }

  if (border_) {
    CRGBA  lc = lineColor_.getValue(CRGBA(0,0,0));
    double lw = width_;

    if (highlighted) {
      lc = CRGBA(1,0,0);
      lw = 2;
    }

    renderer->drawRect(bbox_, lc, lw);
  }
}
