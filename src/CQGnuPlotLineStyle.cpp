#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotUtil.h>
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

CQGnuPlot::SymbolType
CQGnuPlotLineStyle::
pointType() const
{
  return CQGnuPlotUtil::symbolType(CGnuPlotLineStyle::pointType());
}

void
CQGnuPlotLineStyle::
setPointType(const CQGnuPlot::SymbolType &type)
{
  CGnuPlotLineStyle::setPointType(CQGnuPlotUtil::symbolType(type));
}
