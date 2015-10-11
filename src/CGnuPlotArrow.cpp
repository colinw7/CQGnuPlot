#include <CGnuPlotArrow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlot *
CGnuPlotArrow::
app() const
{
  return (group_ ? group_->app() : 0);
}

void
CGnuPlotArrow::
setLineColor(const CGnuPlotColorSpec &c)
{
  setLineColor(c.color());
}

bool
CGnuPlotArrow::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return data_.inside(data);
}

CGnuPlotTipData
CGnuPlotArrow::
tip() const
{
  return data_.tip();
}

void
CGnuPlotArrow::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  data_.draw(renderer, group_, highlighted);
}

void
CGnuPlotArrow::
print(std::ostream &os) const
{
  style().print(app(), os);

  os << " from " << getFrom();

  if      (coordType() == CoordType::FROM_RTO)
    os << " rto " << getTo();
  else if (coordType() == CoordType::FROM_TO)
    os << " to " << getTo();
  else if (coordType() == CoordType::FROM_ANGLE)
    os << " length " << getLength() << " angle " << getAngle();
}
