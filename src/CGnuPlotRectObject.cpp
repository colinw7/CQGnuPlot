#include <CGnuPlotRectObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotRectObject::
CGnuPlotRectObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
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
                  bbox_.getXMin(), bbox_.getYMin(), bbox_.getXMax(), bbox_.getYMax()));

    if (text_ != "")
      tip.setYStr(text_);
  }

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  tip.setBBox(bbox_);

  return tip;
}

void
CGnuPlotRectObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    fill   = fill_  ->dup();
    stroke = stroke_->dup();

    fill->setColor(fill->color().getLightRGBA());

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  renderer->fillRect  (bbox_, *fill  );
  renderer->strokeRect(bbox_, *stroke);

  if (text_ != "") {
    double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
    double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    CRGBA tc(0,0,0);

    if (fill->type() == CGnuPlotTypes::FillType::SOLID)
      tc = fill->color().bwContrast();

    renderer->drawText(bbox_.getCenter() - CPoint2D(tw/2, th/2), text_, tc);
  }
}
