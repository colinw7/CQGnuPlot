#include <CQGnuPlotErrorBarObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotStroke.h>
#include <CQGnuPlotMark.h>

CQGnuPlotErrorBarObject::
CQGnuPlotErrorBarObject(CQGnuPlotPlot *plot) :
 CGnuPlotErrorBarObject(plot)
{
}

CQGnuPlotErrorBarObject::
~CQGnuPlotErrorBarObject()
{
}

CQGnuPlotStroke *
CQGnuPlotErrorBarObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

CQGnuPlotMark *
CQGnuPlotErrorBarObject::
mark() const
{
  return dynamic_cast<CQGnuPlotMark *>(mark_.get());
}

void
CQGnuPlotErrorBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotErrorBarObject::draw(renderer);
}
