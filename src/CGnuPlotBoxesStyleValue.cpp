#include <CGnuPlotBoxesStyleValue.h>
#include <CGnuPlotPlot.h>

void
CGnuPlotBoxesStyleValue::
init()
{
  boxWidth_ = plot_->boxWidth();
}

double
CGnuPlotBoxesStyleValue::
getSpacing() const
{
  double s = plot_->getXSpacing();

  if      (type() == BoxWidthType::ABSOLUTE) return width();
  else if (type() == BoxWidthType::RELATIVE) return width()*s;
  else                                       return s;
}
