#include <CGnuPlotCircle.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotCircle::
draw(CGnuPlotRenderer *renderer) const
{
  const CGnuPlotCircle *e = this;

  CPoint2D center = e->getCenter().getPoint(renderer);

  double xr = e->getRadius().getXDistance(renderer);
  double yr = e->getRadius().getYDistance(renderer);

  if (e->getFillColor().isRGB()) {
    if (arcStart_.isValid() || arcEnd_.isValid())
      renderer->fillPieSlice(center, 0, xr, arcStart_.getValue(0), arcEnd_.getValue(360),
                             e->getFillColor().color());
    else
      renderer->fillEllipse(center, xr, yr, 0, e->getFillColor().color());
  }

  CRGBA lc = e->getStrokeColor().getValue(CRGBA(0,0,0));

  if (arcStart_.isValid() || arcEnd_.isValid())
    renderer->drawPieSlice(center, 0, xr, arcStart_.getValue(0), arcEnd_.getValue(360),
                           1, lc);
  else
    renderer->drawEllipse(center, xr, yr, 0, lc, 1);
}

bool
CGnuPlotCircle::
inside(const CPoint2D &) const
{
  return false;
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
