#include <CGnuPlotPolygon.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotPolygon::
draw() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  const CGnuPlotPolygon *poly = this;

  if (poly->getFillColor().isRGB())
    renderer->fillPolygon(poly->getPoints(), poly->getFillColor().color());

  renderer->drawPolygon(poly->getPoints(), getLineWidth().getValue(0), poly->getStrokeColor());
}

bool
CGnuPlotPolygon::
inside(const CPoint2D &) const
{
  return false;
}

void
CGnuPlotPolygon::
print(std::ostream &) const
{
  //TODO
}
