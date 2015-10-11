#include <CQGnuPlotEndBar.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotEndBar::
CQGnuPlotEndBar(CQGnuPlotPlot *plot) :
 CGnuPlotEndBar(plot)
{
}

CQGnuPlotEndBar::
~CQGnuPlotEndBar()
{
}

CQGnuPlotStroke *
CQGnuPlotEndBar::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}
