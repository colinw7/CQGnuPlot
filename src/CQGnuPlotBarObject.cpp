#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotBarObject::
CQGnuPlotBarObject(CQGnuPlotPlot *plot) :
 CGnuPlotBarObject(plot)
{
}

CQGnuPlotBarObject::
~CQGnuPlotBarObject()
{
}

CQGnuPlotFill *
CQGnuPlotBarObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotBarObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBarObject::draw(renderer);
}
