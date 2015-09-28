#include <CGnuPlotArrowStyle.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlot.h>

double
CGnuPlotArrowStyle::
lineWidth(CGnuPlot *plot) const
{
  if (lineWidth_.isValid())
   return lineWidth_.getValue();

  if (plot && lineStyle_.isValid()) {
    CGnuPlotLineStyleP ls = plot->lineStyle(lineStyle_.getValue());

    return (ls.isValid() ? ls->calcWidth() : 1);
  }
  else
    return 1;
}

void
CGnuPlotArrowStyle::
print(CGnuPlot *plot, std::ostream &os) const
{
  os << headStr();
  os << " " << filledStr();
  if (! border_) os << " noborder";
  os << " " << frontStr();

  if (lineStyle_.isValid())
    os <<" lt " << lineStyle_;
  else
    os <<" lt black";

  os << " linewidth " << lineWidth(plot);

  os << " dashtype " << dash_;

  if (length_.value() > 0.0 || angle_ >= 0.0 || backAngle_ >= 0.0) {
    os << " arrow head" << (fhead_ && thead_ ? "s" : "") << ": " << filledStr();
    os << ", length "; length_.print(os);
    os << ", angle " << (angle_ > 0 ? angle_ : 0.0);

    if (backAngle_ >= 0)
      os << ", backangle " << backAngle_;

    if (fixed_)
      os << " fixed";
  }
}
