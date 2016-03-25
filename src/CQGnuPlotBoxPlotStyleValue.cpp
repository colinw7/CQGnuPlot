#include <CQGnuPlotBoxPlotStyleValue.h>
#include <CQGnuPlotPlot.h>

CQGnuPlotBoxPlotStyleValue::
CQGnuPlotBoxPlotStyleValue(CQGnuPlotPlot *qplot) :
 CGnuPlotBoxPlotStyleValue(qplot), qplot_(qplot)
{
}

CQGnuPlotBoxPlotStyleValue::
~CQGnuPlotBoxPlotStyleValue()
{
}

CQGnuPlotEnum::BoxType
CQGnuPlotBoxPlotStyleValue::
boxType() const
{
  return CQGnuPlotEnum::boxTypeConv(CGnuPlotBoxPlotStyleValue::type());
}

void
CQGnuPlotBoxPlotStyleValue::
setBoxType(const CQGnuPlotEnum::BoxType &type)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setType(CQGnuPlotEnum::boxTypeConv(type));
}

void
CQGnuPlotBoxPlotStyleValue::
setRange(double r)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setRange(r);
}

void
CQGnuPlotBoxPlotStyleValue::
setFraction(double r)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setFraction(r);
}

void
CQGnuPlotBoxPlotStyleValue::
setOutliers(bool b)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setOutliers(b);
}

void
CQGnuPlotBoxPlotStyleValue::
setSeparation(double r)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setSeparation(r);
}

void
CQGnuPlotBoxPlotStyleValue::
setPointType(int t)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setPointType(t);
}

CQGnuPlotEnum::BoxLabels
CQGnuPlotBoxPlotStyleValue::
boxLabels() const
{
  return CQGnuPlotEnum::boxLabelsConv(CGnuPlotBoxPlotStyleValue::labels());
}

void
CQGnuPlotBoxPlotStyleValue::
setBoxLabels(const CQGnuPlotEnum::BoxLabels &labels)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setLabels(CQGnuPlotEnum::boxLabelsConv(labels));
}

void
CQGnuPlotBoxPlotStyleValue::
setSorted(bool b)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlotStyleValue::setSorted(b);
}
