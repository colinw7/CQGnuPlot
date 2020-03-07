#include <CQGnuPlotMultiplot.h>
#include <CQGnuPlotGroup.h>
#include <CQFontUtil.h>
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
  return CQFontUtil::toQFont(CGnuPlotMultiplot::titleFont());
}

void
CQGnuPlotMultiplot::
setTitleFont(const QFont &f)
{
  CGnuPlotMultiplot::setTitleFont(CQFontUtil::fromQFont(f));
}
