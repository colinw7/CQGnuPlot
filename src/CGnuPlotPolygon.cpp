#include <CGnuPlotPolygon.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotPolygon::
draw(CGnuPlotRenderer *renderer) const
{
  const CGnuPlotPolygon *poly = this;

  if (poly->getFillColor().isRGB())
    renderer->fillPolygon(poly->getPoints(), poly->getFillColor().color());

  CRGBA lc = poly->getStrokeColor().getValue(CRGBA(0,0,0));

  renderer->drawPolygon(poly->getPoints(), getLineWidth().getValue(0), lc);
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
