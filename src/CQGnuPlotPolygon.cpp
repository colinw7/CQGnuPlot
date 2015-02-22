#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CQGnuPlotPolygon::
CQGnuPlotPolygon(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotPolygon(group)
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
draw() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  CGnuPlotPolygon::draw();

  if (isSelected())
    renderer->drawPolygon(getPoints(), 2.0, CRGBA(1,0,0));
}
