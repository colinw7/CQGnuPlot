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
  return CQUtil::rgbaToColor(CGnuPlotLineStyle::color(CRGBA(0,0,0)));
}

void
CQGnuPlotLineStyle::
setColor(const QColor &c)
{
  CGnuPlotLineStyle::setColor(CQUtil::colorToRGBA(c));
}

CQGnuPlot::SymbolType
CQGnuPlotLineStyle::
pointType() const
{
  return CQGnuPlotUtil::symbolConv(CGnuPlotLineStyle::pointType());
}

void
CQGnuPlotLineStyle::
setPointType(const CQGnuPlot::SymbolType &type)
{
  CGnuPlotLineStyle::setPointType(CQGnuPlotUtil::symbolConv(type));
}
