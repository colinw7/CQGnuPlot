#include <CQGnuPlotMark.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotMark::
CQGnuPlotMark(CQGnuPlotPlot *plot) :
 CGnuPlotMark(plot)
{
}

CQGnuPlotMark *
CQGnuPlotMark::
dup() const
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  CQGnuPlotMark *stroke = new CQGnuPlotMark(qplot);

  stroke->CGnuPlotMark::operator=(*this);

  return stroke;
}

CQGnuPlotEnum::SymbolType
CQGnuPlotMark::
type() const
{
  return CQGnuPlotEnum::symbolConv(CGnuPlotMark::type());
}

void
CQGnuPlotMark::
setType(const CQGnuPlotEnum::SymbolType &type)
{
  CGnuPlotMark::setType(CQGnuPlotEnum::symbolConv(type));
}

QColor
CQGnuPlotMark::
color() const
{
  return toQColor(CGnuPlotMark::color());
}

void
CQGnuPlotMark::
setColor(const QColor &color)
{
  CGnuPlotMark::setColor(fromQColor(color));
}
