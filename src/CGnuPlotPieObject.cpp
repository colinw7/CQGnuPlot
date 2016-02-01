#include <CGnuPlotPieObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotPieObject::
CGnuPlotPieObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
}

bool
CGnuPlotPieObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  const CPoint2D &p = mouseEvent.window();

  double r = p.distanceTo(center());

  if (r > r_)
    return false;

  // check angle
  double a = CAngle::Rad2Deg(atan2(p.y - center().y, p.x - center().x)); while (a < 0) a += 360.0;

  double angle1 = angle1_; while (angle1 < 0) angle1 += 360.0;
  double angle2 = angle2_; while (angle2 < 0) angle2 += 360.0;

  if (angle1 > angle2) {
    // crosses zero
    if (a >= 0 && a <= angle2)
      return true;

    if (a <= 360 && a >= angle1)
      return true;
  }
  else {
    if (a >= angle1 && a <= angle2)
      return true;
  }

  return false;
}

bool
CGnuPlotPieObject::
keyInside(const CPoint2D &p) const
{
  return keyRect_.inside(p);
}

CGnuPlotTipData
CGnuPlotPieObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(name_);
  tip.setYStr(CStrUtil::strprintf("%g", value_));

  CPoint2D d(r_, r_);

  CBBox2D rect(center() - d, center() + d);

  tip.setBorderColor(fill_->color());
  tip.setXColor     (fill_->color());

  tip.setBBox(rect);

  return tip;
}

void
CGnuPlotPieObject::
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

    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  CPoint2D c = center();

  bool exploded = exploded_;

  if (isSelected())
    exploded = true;

  if (exploded) {
    double angle = CAngle::Deg2Rad((angle1_ + angle2_)/2.0);

    double dx = 0.1*r_*cos(angle);
    double dy = 0.1*r_*sin(angle);

    c.x += dx;
    c.y += dy;
  }

  //---

  double ir = innerRadius_*r_;

  renderer->fillPieSlice  (c, ir, r_, angle1_, angle2_, *fill  );
  renderer->strokePieSlice(c, ir, r_, angle1_, angle2_, *stroke);

  //---

  if (name_ != "") {
    double tangle = CAngle::Deg2Rad((angle1_ + angle2_)/2.0);

    double lr = labelRadius_*r_;

    if (lr < 0.01)
      lr = 0.01;

    double x = c.x + lr*cos(tangle);
    double y = c.y + lr*sin(tangle);

    CPoint2D tp(x, y);

    CRGBA tc(0,0,0);

    if (fill->type() == CGnuPlotTypes::FillType::SOLID)
      tc = fill->color().bwContrast();

    // aligned ?
    renderer->drawHAlignedText(tp, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                               VAlignPos(CVALIGN_TYPE_CENTER, 0), name_, tc);
  }

  //---

  CPoint2D pr(r_, r_);

  bbox_ = CBBox2D(c - pr, c + pr);
}
