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

  if (lineStyle_.isValid()) {
    CGnuPlotLineStyleP ls = plot->lineStyle(lineStyle_.getValue());

    return (ls ? ls->width() : 1);
  }
  else
    return 1;
}

void
CGnuPlotArrowStyle::
print(CGnuPlot *plot, std::ostream &os) const
{
  os << headStr() << " " << filledStr() << " " << frontStr() <<
        " linetype " << lineStyle_ << " linewidth " << lineWidth(plot);

  os << " dashtype " << dash_;

  if (length_.value() > 0.0 || angle_ >= 0.0 || backAngle_ >= 0.0) {
    os << " arrow head" << (fhead_ && thead_ ? "s" : "") << ": " << filledStr();
    os << ", length "; length_.print(os);
    os << ", angle " << angle_ << ", backangle " << backAngle_;
  }
}
