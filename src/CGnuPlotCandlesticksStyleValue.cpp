#include <CGnuPlotCandlesticksStyleValue.h>
#include <CGnuPlotPlot.h>

void
CGnuPlotCandlesticksStyleValue::
init()
{
  boxWidth_ = plot_->boxWidth();
}

double
CGnuPlotCandlesticksStyleValue::
getSpacing() const
{
  double s = 0.1;

  if      (type() == BoxWidthType::ABSOLUTE) return width();
  else if (type() == BoxWidthType::RELATIVE) return width()*s;
  else                                       return s;
}
