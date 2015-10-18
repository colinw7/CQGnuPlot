#include <CQGnuPlotBoxObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>
#include <CQGnuPlotMark.h>

CQGnuPlotBoxObject::
CQGnuPlotBoxObject(CQGnuPlotPlot *plot) :
 CGnuPlotBoxObject(plot)
{
}

CQGnuPlotBoxObject::
~CQGnuPlotBoxObject()
{
}

CQGnuPlotFill *
CQGnuPlotBoxObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotBoxObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

CQGnuPlotMark *
CQGnuPlotBoxObject::
outlierMark() const
{
  return dynamic_cast<CQGnuPlotMark *>(outlierMark_.get());
}

void
CQGnuPlotBoxObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBoxObject::draw(renderer);
}
