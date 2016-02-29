#include <CQGnuPlotBoxPlot.h>
#include <CQGnuPlotPlot.h>

CQGnuPlotBoxPlot::
CQGnuPlotBoxPlot(CQGnuPlotPlot *plot) :
 CGnuPlotBoxPlot(plot)
{
}

CQGnuPlotBoxPlot::
~CQGnuPlotBoxPlot()
{
}

CQGnuPlotEnum::BoxType
CQGnuPlotBoxPlot::
boxType() const
{
  return CQGnuPlotEnum::boxTypeConv(CGnuPlotBoxPlot::type());
}

void
CQGnuPlotBoxPlot::
setBoxType(const CQGnuPlotEnum::BoxType &type)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setType(CQGnuPlotEnum::boxTypeConv(type));
}

void
CQGnuPlotBoxPlot::
setRange(double r)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setRange(r);
}

void
CQGnuPlotBoxPlot::
setFraction(double r)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setFraction(r);
}

void
CQGnuPlotBoxPlot::
setOutliers(bool b)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setOutliers(b);
}

void
CQGnuPlotBoxPlot::
setSeparation(double r)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setSeparation(r);
}

void
CQGnuPlotBoxPlot::
setPointType(int t)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setPointType(t);
}

CQGnuPlotEnum::BoxLabels
CQGnuPlotBoxPlot::
boxLabels() const
{
  return CQGnuPlotEnum::boxLabelsConv(CGnuPlotBoxPlot::labels());
}

void
CQGnuPlotBoxPlot::
setBoxLabels(const CQGnuPlotEnum::BoxLabels &labels)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setLabels(CQGnuPlotEnum::boxLabelsConv(labels));
}

void
CQGnuPlotBoxPlot::
setSorted(bool b)
{
  plot_->clearBoxCache();

  CGnuPlotBoxPlot::setSorted(b);
}
