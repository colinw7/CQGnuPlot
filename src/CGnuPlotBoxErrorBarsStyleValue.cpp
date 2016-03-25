#include <CGnuPlotBoxErrorBarsStyleValue.h>
#include <CGnuPlotPlot.h>

void
CGnuPlotBoxErrorBarsStyleValue::
init()
{
  boxWidth_ = plot_->boxWidth();
}

double
CGnuPlotBoxErrorBarsStyleValue::
getSpacing() const
{
  double s = plot_->getXSpacing();

  if      (type() == BoxWidthType::ABSOLUTE) return width();
  else if (type() == BoxWidthType::RELATIVE) return width()*s;
  else                                       return s;
}
