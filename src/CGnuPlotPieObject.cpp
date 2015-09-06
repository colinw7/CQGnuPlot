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
inside(const CPoint2D &p) const
{
  double r = p.distanceTo(c_);

  if (r > r_)
    return false;

  // check angle
  double a = CAngle::Rad2Deg(atan2(p.y - c_.y, p.x - c_.x)); while (a < 0) a += 360.0;

  double angle1 = angle1_; while (angle1 < 0) angle1 += 360.0;
  double angle2 = angle2_; while (angle2 < 0) angle2 += 360.0;

  if (angle2 > angle1) {
    // crosses zero
    if (a >= 0 && a <= angle1)
      return true;

    if (a <= 360 && a >= angle2)
      return true;
  }
  else {
    if (a >= angle2 && a <= angle1)
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
  CPoint2D c = c_;

  if (exploded_) {
    double angle = CAngle::Deg2Rad((angle1_ + angle2_)/2.0);

    double x = c_.x + 0.1*r_*cos(angle);
    double y = c_.y + 0.1*r_*sin(angle);

    c.x += x;
    c.y += y;
  }

  double ir = innerRadius_*r_;

  renderer->fillPieSlice(c, ir, r_, angle1_, angle2_, fillColor_.getValue(CRGBA(1,0,0)));
  renderer->drawPieSlice(c, ir, r_, angle1_, angle2_, 0, lineColor_.getValue(CRGBA(1,0,0)));

  double tangle = CAngle::Deg2Rad((angle1_ + angle2_)/2.0);

  double lr = labelRadius_*r_;

  if (lr < 0.01)
    lr = 0.01;

  double x = c.x + lr*cos(tangle);
  double y = c.y + lr*sin(tangle);

  CPoint2D tp(x, y);

  CRGBA tc(0,0,0);

  // aligned ?
  renderer->drawHAlignedText(tp, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0, name_, tc);
}
