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
    os << "boxplot representation is box and whisker\n";
  else
    os << "boxplot representation is finance bar\n";

  if (fraction_)
    os << "boxplot range extends from the median to include " <<
          fraction_.value() << " of the points\n";
  else
    os << "boxplot range extends from the box by " <<
          range_.value_or(1.5) << " of the interquartile distance\n";

  if (outliers_)
    os << "outliers will be drawn using point type " << pointType_.value_or(7) << "\n";
  else
    os << "outliers will not be drawn\n";

  os << "separation between boxplots is " << separation_.value_or(1) << "\n";

  if      (labels_ == BoxLabels::Off)
    os << "factor labels are off\n";
  else if (labels_ == BoxLabels::Auto)
    os << "factor labels are automatic\n";
  else if (labels_ == BoxLabels::X)
    os << "factor labels will be put on the x axis\n";
  else if (labels_ == BoxLabels::X2)
    os << "factor labels will be put on the x2 axis\n";

  if (! sorted_)
    os << "factor labels will appear in the order they were found\n";
  else
    os << "factor labels will be sorted alphabetically\n";
}

void
CGnuPlotBoxPlotStyleValue::
unset()
{
  type_       = BoxType::CandleSticks;
  range_      = std::optional<double>();
  fraction_   = std::optional<double>();
  outliers_   = true;
  separation_ = std::optional<double>();
  pointType_  = std::optional<int>();
  labels_     = BoxLabels::Auto;
  sorted_     = false;
}
