#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CQGnuPlotPolygon::
CQGnuPlotPolygon(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(group, this), CGnuPlotPolygon(group)
{
  setObjectName("polygon");
}

double
CQGnuPlotPolygon::
getLineWidth() const
{
  return CGnuPlotPolygon::getLineWidth().getValue(0);
}

void
CQGnuPlotPolygon::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotPolygon::draw(renderer);
}
