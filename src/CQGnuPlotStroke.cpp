#include <CQGnuPlotStroke.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotStroke::
CQGnuPlotStroke(CQGnuPlotPlot *plot) :
 CGnuPlotStroke(plot)
{
}

CQGnuPlotStroke *
CQGnuPlotStroke::
dup() const
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  CQGnuPlotStroke *stroke = new CQGnuPlotStroke(qplot);

  stroke->CGnuPlotStroke::operator=(*this);

  return stroke;
}

QColor
CQGnuPlotStroke::
color() const
{
  return toQColor(CGnuPlotStroke::color());
}

void
CQGnuPlotStroke::
setColor(const QColor &color)
{
  CGnuPlotStroke::setColor(fromQColor(color));
}

CLineDash
CQGnuPlotStroke::
getDash() const
{
  return CGnuPlotStroke::lineDash();
}

void
CQGnuPlotStroke::
setDash(const CLineDash &dash)
{
  CGnuPlotStroke::setLineDash(dash);
}
