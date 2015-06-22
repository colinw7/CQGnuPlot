#include <CGnuPlotCircle.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotCircle::
draw(CGnuPlotRenderer *renderer) const
{
  const CGnuPlotCircle *e = this;

  c_ = e->getCenter().getPoint2D(renderer);

  // TODO: always round
  xr_ = e->getRadius().getXDistance(renderer);
  yr_ = e->getRadius().getYDistance(renderer);

  CRGBA fc = e->getFillColor().color();

  double a1 = arcStart_.getValue(0);
  double a2 = arcEnd_  .getValue(360);

  if (e->getFillColor().isRGB()) {
    if (arcStart_.isValid() || arcEnd_.isValid())
      renderer->fillPieSlice(c_, 0, xr_, a1, a2, fc);
    else
      renderer->fillEllipse(c_, xr_, yr_, 0, fc);
  }

  CRGBA lc = e->getStrokeColor().getValue(CRGBA(0,0,0));

  if (arcStart_.isValid() || arcEnd_.isValid())
    renderer->drawPieSlice(c_, 0, xr_, a1, a2, 1, lc);
  else
    renderer->drawEllipse(c_, xr_, yr_, 0, lc, 1);
}

bool
CGnuPlotCircle::
inside(const CPoint2D &p) const
{
  double x = p.x - c_.x;
  double y = p.y - c_.y;

  double x2 = x*x;
  double y2 = y*y;

  double xr2 = xr_*xr_;
  double yr2 = yr_*yr_;

  double f = x2/xr2 + y2/yr2 - 1;

  if (f > 0)
    return false;

  if (arcStart_.isValid() || arcEnd_.isValid()) {
    double a1 = arcStart_.getValue(0);
    double a2 = arcEnd_  .getValue(360);

    double a = CAngle::Rad2Deg(atan2(y, x));

    if (a < a1 || a > a2)
      return false;
  }

  return true;
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
