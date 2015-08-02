#include <CGnuPlotEllipse.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotEllipse::
draw(CGnuPlotRenderer *renderer) const
{
  const CGnuPlotEllipse *e = this;

  CPoint2D center = e->getCenter().getPoint2D(renderer);

  if (e->getFillColor().isRGB())
    renderer->fillEllipse(center, e->getRX(), e->getRY(), 0, e->getFillColor().color());

  CRGBA lc;

  if      (e->getStrokeColor().isValid()) {
    lc = e->getStrokeColor().getValue();

    renderer->drawEllipse(center, e->getRX(), e->getRY(), angle_, lc, 1);
  }
  else if (getFillStyle().calcColor(group_, lc)) {
    renderer->drawEllipse(center, e->getRX(), e->getRY(), angle_, lc, 1);
  }
}

bool
CGnuPlotEllipse::
inside(const CPoint2D &) const
{
  return false;
}

void
CGnuPlotEllipse::
print(std::ostream &os) const
{
  os << " ellipse";

  os << " center " << p_;
  os << " size " << 2*rx_ << ", " << 2*ry_;
  os << " angle " << angle_;

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  //os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc bgnd "

  os << " fillstyle " << fs_;

  //os << " lt " << lt_.getValue(-1);
}
