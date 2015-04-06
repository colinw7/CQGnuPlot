#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotLineStyle::
CQGnuPlotLineStyle()
{
}

CQGnuPlotLineStyle::
~CQGnuPlotLineStyle()
{
}

QColor
CQGnuPlotLineStyle::
color() const
{
  return CQUtil::rgbaToColor(CGnuPlotLineStyle::calcColor(CRGBA(0,0,0)));
}

void
CQGnuPlotLineStyle::
setColor(const QColor &c)
{
  CGnuPlotColorSpec cs;

  cs.setRGB(CQUtil::colorToRGBA(c));

  CGnuPlotLineStyle::setColor(cs);
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
