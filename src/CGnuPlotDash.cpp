#include <CGnuPlotDash.h>
#include <CGnuPlot.h>

CLineDash
CGnuPlotDash::
calcDash(CGnuPlot *plot, const CLineDash &dash) const
{
  if      (ind_.isValid())
    return plot->getLineDash(ind_.getValue());
  else if (dash_.isValid())
    return dash_.getValue();
  else
    return dash;
}

void
CGnuPlotDash::
print(std::ostream &os) const
{
  if      (ind_.isValid())
    os << ind_.getValue();
  else if (dash_.isValid() && ! dash_.getValue().isSolid())
    os << "(" << dash_.getValue() << ")";
  else
    os << "solid";
}
