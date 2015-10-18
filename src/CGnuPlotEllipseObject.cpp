#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>

CGnuPlotEllipseObject::
CGnuPlotEllipseObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
}

void
CGnuPlotEllipseObject::
setCenter(const CPoint2D &center)
{
  center_ = center;

  update();
}

void
CGnuPlotEllipseObject::
setSize(const CSize2D &size)
{
  size_ = size;

  update();
}

void
CGnuPlotEllipseObject::
update()
{
  rect_ = CBBox2D();

  double w = size_.getWidth ();
  double h = size_.getHeight();

  rect_ += center_ - CPoint2D(w, h);
  rect_ += center_ + CPoint2D(w, h);
}

bool
CGnuPlotEllipseObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  if (! rect_.inside(data.window))
    return false;

  // inside ellipse
  return true;
}

CGnuPlotTipData
CGnuPlotEllipseObject::
tip() const
{
  CGnuPlotTipData tip;

  double w = size_.getWidth ();
  double h = size_.getHeight();

  tip.setXStr(CStrUtil::strprintf("%g, %g", center_.x, center_.y));
  tip.setYStr(CStrUtil::strprintf("%g, %g", w, h));

  if      (tipText_ != "")
    tip.setYStr(tipText_);
  else if (text_ != "")
    tip.setYStr(text_);

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  tip.setRect(rect_);

  return tip;
}

void
CGnuPlotEllipseObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    fill   = fill_  ->dup();
    stroke = stroke_->dup();

    fill->setColor(fill->color().getLightRGBA());

    stroke->setEnabled(true);
    stroke->setColor  (CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  double w = size_.getWidth ();
  double h = size_.getHeight();

  renderer->fillEllipse  (center_, w, h, angle_, *fill  );
  renderer->strokeEllipse(center_, w, h, angle_, *stroke);

  if (text_ != "") {
    CRGBA tc(0,0,0);

    if (fill->type() == CGnuPlotTypes::FillType::SOLID)
      tc = fill->color().bwContrast();

    double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
    double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    renderer->drawText(center_ - CPoint2D(tw/2, th/2), text_, tc);
  }
}
