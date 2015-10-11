#include <CQGnuPlotPathObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotPathObject::
CQGnuPlotPathObject(CQGnuPlotPlot *plot) :
 CGnuPlotPathObject(plot)
{
}

CQGnuPlotPathObject::
~CQGnuPlotPathObject()
{
}

CQGnuPlotStroke *
CQGnuPlotPathObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotPathObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotPathObject::draw(renderer);
}
