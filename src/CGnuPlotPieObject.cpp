#include <CGnuPlotPieObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotPieObject::
CGnuPlotPieObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotPieObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  const CPoint2D &p = data.window;

  double r = p.distanceTo(c_);

  if (r > r_)
    return false;

  // check angle
  double a = CAngle::Rad2Deg(atan2(p.y - c_.y, p.x - c_.x)); while (a < 0) a += 360.0;

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

  CBBox2D rect(c_ - d, c_ + d);

  if (fillColor_.isValid()) {
    CRGBA c = fillColor();

    tip.setBorderColor(c);
    tip.setXColor(c);
  }

  tip.setRect(rect);

  return tip;
}

void
CGnuPlotPieObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CPoint2D c = c_;

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

  CRGBA fc = fillColor_.getValue(CRGBA(1,0,0));

  if (highlighted) {
    fc = fc.getLightRGBA();
  }

  renderer->fillPieSlice(c, ir, r_, angle1_, angle2_, fc);

  //---

  CRGBA  lc = lineColor_.getValue(CRGBA(1,0,0));
  double lw = 0;

  if (highlighted) {
    lc = CRGBA(1,0,0);
    lw = 2;
  }

  renderer->drawPieSlice(c, ir, r_, angle1_, angle2_, lw, lc);

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

    if (highlighted) {
      tc = CRGBA(1,0,0);
    }

    // aligned ?
    renderer->drawHAlignedText(tp, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0, name_, tc);
  }
}
