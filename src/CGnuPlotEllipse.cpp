#include <CGnuPlotEllipse.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotEllipse::
draw(CGnuPlotRenderer *renderer) const
{
  const CGnuPlotEllipse *e = this;

  if (e->getFillColor().isRGB())
    renderer->fillEllipse(e->getCenter(), e->getRX(), e->getRY(), 0, e->getFillColor().color());

  renderer->drawEllipse(e->getCenter(), e->getRX(), e->getRY(), 0, e->getStrokeColor());
}

bool
CGnuPlotEllipse::
inside(const CPoint2D &) const
{
  return false;
}

void
CGnuPlotEllipse::
print(std::ostream &) const
{
  //TODO
}
