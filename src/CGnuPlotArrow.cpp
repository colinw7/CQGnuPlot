#include <CGnuPlotArrow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotArrow::
CGnuPlotArrow(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
}

CGnuPlotArrow *
CGnuPlotArrow::
setData(const CGnuPlotArrow *arrow)
{
  (void) CGnuPlotGroupAnnotation::setData(arrow);

  data_ = arrow->data_;

  return this;
}

CGnuPlot *
CGnuPlotArrow::
app() const
{
  return (group_ ? group_->app() : nullptr);
}

void
CGnuPlotArrow::
setLineColor(const CGnuPlotColorSpec &c)
{
  setLineColor(c.color());
}

void
CGnuPlotArrow::
initClip()
{
  // TODO: more coords
  clip_ = ! getFrom().isScreen();
}

bool
CGnuPlotArrow::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return data_.inside(mouseEvent);
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
