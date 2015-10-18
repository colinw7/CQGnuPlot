#include <CQGnuPlotBoxBarObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotBoxBarObject::
CQGnuPlotBoxBarObject(CQGnuPlotPlot *plot) :
 CGnuPlotBoxBarObject(plot)
{
}

CQGnuPlotBoxBarObject::
~CQGnuPlotBoxBarObject()
{
}

CQGnuPlotFill *
CQGnuPlotBoxBarObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotBoxBarObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotBoxBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBoxBarObject::draw(renderer);
}
