#include <CQGnuPlotMultiplot.h>
#include <CQGnuPlotGroup.h>
#include <CQUtil.h>

CQGnuPlotMultiplot::
CQGnuPlotMultiplot(CQGnuPlot *plot) :
 CGnuPlotMultiplot(plot)
{
}

QString
CQGnuPlotMultiplot::
title() const
{
  return CGnuPlotMultiplot::title().c_str();
}

void
CQGnuPlotMultiplot::
setTitle(const QString &s)
{
  CGnuPlotMultiplot::setTitle(s.toStdString());
}

QFont
CQGnuPlotMultiplot::
titleFont() const
{
  return CQUtil::toQFont(CGnuPlotMultiplot::titleFont());
}

void
CQGnuPlotMultiplot::
setTitleFont(const QFont &f)
{
  CGnuPlotMultiplot::setTitleFont(CQUtil::fromQFont(f));
}
