#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotBubbleObject::
CGnuPlotBubbleObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
}

bool
CGnuPlotBubbleObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  double r = mouseEvent.window().distanceTo(c_);

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

  tip.setBBox(bbox_);

  tip.setBorderColor(fill_->color());
  tip.setXColor     (fill_->color());

  return tip;
}

void
CGnuPlotBubbleObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    fill   = fill_  ->dup();
    stroke = stroke_->dup();

    if (fill->type() == CGnuPlotTypes::FillType::PATTERN)
      fill->setBackground(fill->background().getLightRGBA());
    else
      fill->setColor(fill->color().getLightRGBA());

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  renderer->fillEllipse  (c_, xr_, yr_, 0, *fill  );
  renderer->strokeEllipse(c_, xr_, yr_, 0, *stroke);

  double s = renderer->fontSize();

  renderer->setFontSize(6);

  CRGBA tc(0,0,0);

  if (fill->type() == CGnuPlotTypes::FillType::SOLID)
    tc = fill->color().bwContrast();

  renderer->drawHAlignedText(c_, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0), name_, tc);

  renderer->setFontSize(s);

  //---

  CPoint2D d(xr_, yr_);

  bbox_ = CBBox2D(c_ - d, c_ + d);
}
