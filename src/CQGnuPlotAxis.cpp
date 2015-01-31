#include <CQGnuPlotAxis.h>
#include <CQGnuPlotGroup.h>

CQGnuPlotAxis::
CQGnuPlotAxis(CQGnuPlotGroup *group, const std::string &id, COrientation dir,
              double start, double end) :
 CGnuPlotAxis(group, id, dir, start, end), displayed_(true)
{
}

CQGnuPlotAxis::
~CQGnuPlotAxis()
{
}

bool
CQGnuPlotAxis::
isDisplayed() const
{
  return displayed_;
}

void
CQGnuPlotAxis::
setDisplayed(bool b)
{
  displayed_ = b;
}
