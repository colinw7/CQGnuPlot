#include <CQGnuPlotImageObject.h>
#include <CQGnuPlotPlot.h>

CQGnuPlotImageObject::
CQGnuPlotImageObject(CQGnuPlotPlot *plot) :
 CGnuPlotImageObject(plot)
{
}

CQGnuPlotImageObject::
~CQGnuPlotImageObject()
{
}

void
CQGnuPlotImageObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotImageObject::draw(renderer);
}
