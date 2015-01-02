#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotLineStyle::
CQGnuPlotLineStyle()
{
}

QColor
CQGnuPlotLineStyle::
color() const
{
  return CQUtil::rgbaToColor(CGnuPlotLineStyle::color());
}

void
CQGnuPlotLineStyle::
setColor(const QColor &c)
{
  CGnuPlotLineStyle::setColor(CQUtil::colorToRGBA(c));
}

CQGnuPlot::CQSymbolType
CQGnuPlotLineStyle::
pointType() const
{
  return CQGnuPlotUtil::symbolConv(CGnuPlotLineStyle::pointType());
}

void
CQGnuPlotLineStyle::
setPointType(const CQGnuPlot::CQSymbolType &type)
{
  CGnuPlotLineStyle::setPointType(CQGnuPlotUtil::symbolConv(type));
}
