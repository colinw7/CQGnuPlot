#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotLineStyle::
CQGnuPlotLineStyle(CGnuPlot *plot) :
 CGnuPlotLineStyle(plot)
{
}

CQGnuPlotLineStyle::
~CQGnuPlotLineStyle()
{
}

int
CQGnuPlotLineStyle::
lineType() const
{
  return CGnuPlotLineStyle::lineType().getValue(0);
}

double
CQGnuPlotLineStyle::
lineWidth() const
{
  return CGnuPlotLineStyle::calcWidth();
}

int
CQGnuPlotLineStyle::
lineDash() const
{
  return CGnuPlotLineStyle::lineDash().ind().getValue(0);
}

void
CQGnuPlotLineStyle::
setLineDash(int ind)
{
  return CGnuPlotLineStyle::setLineDash(CGnuPlotDash(ind));
}

QColor
CQGnuPlotLineStyle::
lineColor() const
{
  return CQUtil::rgbaToColor(CGnuPlotLineStyle::calcColor(0, CRGBA(0,0,0)));
}

void
CQGnuPlotLineStyle::
setLineColor(const QColor &c)
{
  CGnuPlotColorSpec cs;

  cs.setRGB(CQUtil::colorToRGBA(c));

  CGnuPlotLineStyle::setLineColor(cs);
}

CQGnuPlotEnum::SymbolType
CQGnuPlotLineStyle::
pointType() const
{
  return CQGnuPlotEnum::symbolConv(CGnuPlotLineStyle::calcPointType());
}

void
CQGnuPlotLineStyle::
setPointType(const CQGnuPlotEnum::SymbolType &type)
{
  CGnuPlotLineStyle::setPointType(int(CQGnuPlotEnum::symbolConv(type)));
}

double
CQGnuPlotLineStyle::
pointSize() const
{
  return CGnuPlotLineStyle::calcPointSize();
}

int
CQGnuPlotLineStyle::
pointInterval() const
{
  return CGnuPlotLineStyle::pointInterval().getValue(0);
}
