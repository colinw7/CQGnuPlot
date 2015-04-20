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

  renderer->fillPieSlice(c, 0, r_, angle1_, angle2_, fillColor_.getValue(CRGBA(1,0,0)));
  renderer->drawPieSlice(c, 0, r_, angle1_, angle2_, 0, lineColor_.getValue(CRGBA(1,0,0)));

  double tangle = CAngle::Deg2Rad((angle1_ + angle2_)/2.0);

  double x = c.x + 0.5*r_*cos(tangle);
  double y = c.y + 0.5*r_*sin(tangle);

  // aligned ?
  renderer->drawHAlignedText(CPoint2D(x, y), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             name_, CRGBA(0,0,0));
}
