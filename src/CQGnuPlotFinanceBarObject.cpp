#include <CQGnuPlotFinanceBarObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotStroke.h>
#include <CQGnuPlotMark.h>

CQGnuPlotFinanceBarObject::
CQGnuPlotFinanceBarObject(CQGnuPlotPlot *plot) :
 CGnuPlotFinanceBarObject(plot)
{
}

CQGnuPlotFinanceBarObject::
~CQGnuPlotFinanceBarObject()
{
}

CQGnuPlotStroke *
CQGnuPlotFinanceBarObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotFinanceBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotFinanceBarObject::draw(renderer);
}
