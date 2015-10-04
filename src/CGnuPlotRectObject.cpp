#include <CGnuPlotRectObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotRectObject::
CGnuPlotRectObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotRectObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return rect_.inside(data.window);
}

CGnuPlotTipData
CGnuPlotRectObject::
tip() const
{
  CGnuPlotTipData tip;

  if      (tipText_ != "") {
    tip.setXStr(tipText_);
  }
  else {
    tip.setXStr(CStrUtil::strprintf("%g, %g -> %g, %g",
                  rect_.getXMin(), rect_.getYMin(), rect_.getXMax(), rect_.getYMax()));

    if (text_ != "")
      tip.setYStr(text_);
  }

  tip.setBorderColor(lc_);
  tip.setXColor(lc_);

  tip.setRect(rect_);

  return tip;
}

void
CGnuPlotRectObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  if (fillColor_.isValid()) {
    CRGBA fc = fillColor_.getValue();

    lc_ = fc;

    if (highlighted) {
      fc = fc.getLightRGBA();
    }

    renderer->fillRect(rect_, fc);
  }

  if (lineColor_.isValid()) {
    CRGBA  lc = lineColor_.getValue();
    double lw = lineWidth_;

    lc_ = lc;

    if (highlighted) {
      lc = CRGBA(1,0,0);
      lw = 2;
    }

    renderer->drawRect(rect_, lc, lw);
  }

  if (text_ != "") {
    double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
    double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    CRGBA tc = CRGBA(0,0,0);

    if (highlighted) {
      tc = CRGBA(1,0,0);
    }

    renderer->drawText(rect_.getCenter() - CPoint2D(tw/2, th/2), text_, tc);
  }
}
