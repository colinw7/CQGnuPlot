#include <CGnuPlotDash.h>
#include <CGnuPlot.h>

CLineDash
CGnuPlotDash::
calcDash(CGnuPlot *plot, const CLineDash &dash) const
{
  if      (ind_)
    return plot->getLineDash(ind_.value());
  else if (dash_)
    return dash_.value();
  else
    return dash;
}

void
CGnuPlotDash::
print(std::ostream &os) const
{
  if      (ind_)
    os << ind_.value();
  else if (dash_ && ! dash_.value().isSolid())
    os << "(" << dash_.value() << ")";
  else
    os << "solid";
}
