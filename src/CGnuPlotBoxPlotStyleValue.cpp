#include <CGnuPlotBoxPlotStyleValue.h>

CGnuPlotBoxPlotStyleValue::
CGnuPlotBoxPlotStyleValue(CGnuPlotPlot *plot) :
 CGnuPlotStyleValue(plot)
{
}

void
CGnuPlotBoxPlotStyleValue::
init(const CGnuPlotBoxPlotStyleValue &boxPlot)
{
  type_       = boxPlot.type_;
  range_      = boxPlot.range_;
  fraction_   = boxPlot.fraction_;
  outliers_   = boxPlot.outliers_;
  separation_ = boxPlot.separation_;
  pointType_  = boxPlot.pointType_;
  labels_     = boxPlot.labels_;
  sorted_     = boxPlot.sorted_;
}

void
CGnuPlotBoxPlotStyleValue::
show(std::ostream &os) const
{
  if (type_ == BoxType::CandleSticks)
    os << "boxplot representation is box and whisker" << std::endl;
  else
    os << "boxplot representation is finance bar" << std::endl;

  if (fraction_.isValid())
    os << "boxplot range extends from the median to include " <<
          fraction_.getValue() << " of the points" << std::endl;
  else
    os << "boxplot range extends from the box by " <<
          range_.getValue(1.5) << " of the interquartile distance" << std::endl;

  if (outliers_)
    os << "outliers will be drawn using point type " << pointType_.getValue(7) << std::endl;
  else
    os << "outliers will not be drawn" << std::endl;

  os << "separation between boxplots is " << separation_.getValue(1) << std::endl;

  if      (labels_ == BoxLabels::Off)
    os << "factor labels are off" << std::endl;
  else if (labels_ == BoxLabels::Auto)
    os << "factor labels are automatic" << std::endl;
  else if (labels_ == BoxLabels::X)
    os << "factor labels will be put on the x axis" << std::endl;
  else if (labels_ == BoxLabels::X2)
    os << "factor labels will be put on the x2 axis" << std::endl;

  if (! sorted_)
    os << "factor labels will appear in the order they were found" << std::endl;
  else
    os << "factor labels will be sorted alphabetically" << std::endl;
}

void
CGnuPlotBoxPlotStyleValue::
unset()
{
  type_       = BoxType::CandleSticks;
  range_      = COptReal();
  fraction_   = COptReal();
  outliers_   = true;
  separation_ = COptReal();
  pointType_  = COptInt();
  labels_     = BoxLabels::Auto;
  sorted_     = false;
}
