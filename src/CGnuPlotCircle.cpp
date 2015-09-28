#include <CGnuPlotCircle.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotCircle::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  center_ = this->getCenter().getPoint2D(renderer);

  // TODO: always round
  xr_ = this->getRadius().getXDistance(renderer);
  yr_ = this->getRadius().getYDistance(renderer);

  CRGBA fc = this->getFillColor().color();

  if (highlighted) {
    fc = fc.getLightRGBA();
  }

  double a1 = arcStart_.getValue(0);
  double a2 = arcEnd_  .getValue(360);

  if (this->getFillColor().isRGB()) {
    if (arcStart_.isValid() || arcEnd_.isValid())
      renderer->fillPieSlice(center_, 0, xr_, a1, a2, fc);
    else
      renderer->fillEllipse(center_, xr_, yr_, 0, fc);
  }

  c_ = this->getStrokeColor().getValue(CRGBA(0,0,0));

  CRGBA  c  = c_;
  double lw = 1;

  if (highlighted) {
    c  = CRGBA(1,0,0);
    lw = 2;
  }

  bbox_ = CBBox2D(center_.x - xr_, center_.y - yr_, center_.x + xr_, center_.y + yr_);

  if (arcStart_.isValid() || arcEnd_.isValid())
    renderer->drawPieSlice(center_, 0, xr_, a1, a2, lw, c);
  else
    renderer->drawEllipse(center_, xr_, yr_, 0, c, lw);
}

bool
CGnuPlotCircle::
inside(const CGnuPlotTypes::InsideData &data) const
{
  const CPoint2D &p = data.window;

  double x = p.x - center_.x;
  double y = p.y - center_.y;

  double x2 = x*x;
  double y2 = y*y;

  double xr2 = xr_*xr_;
  double yr2 = yr_*yr_;

  double f = x2/xr2 + y2/yr2 - 2;

  if (f > 0)
    return false;

  if (arcStart_.isValid() || arcEnd_.isValid()) {
    // check angle
    double a = CAngle::Rad2Deg(atan2(y, x)); while (a < 0) a += 360.0;

    double angle1 = arcStart_.getValue(  0); while (angle1 < 0) angle1 += 360.0;
    double angle2 = arcEnd_  .getValue(360); while (angle2 < 0) angle2 += 360.0;

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
  else
    return true;
}

CGnuPlotTipData
CGnuPlotCircle::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g, %g", center_.x, center_.y));
  tip.setYStr(CStrUtil::strprintf("%g, %g", xr_, yr_));

  tip.setBorderColor(c_);
  tip.setXColor(c_);

  tip.setRect(bbox_);

  return tip;
}

void
CGnuPlotCircle::
print(std::ostream &os) const
{
  os << " circle";

  os << " center " << p_;
  os << " size " << r_;

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  //os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc bgnd "

  os << " fillstyle " << fs_;

  //os << " lt " << lt_.getValue(-1);
}
