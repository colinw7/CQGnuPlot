#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotPlot::
CQGnuPlotPlot(CQGnuPlotWindow *window) :
 CGnuPlotPlot(window), window_(window), renderer_(0)
{
  renderer_ = new CQGnuPlotRenderer(canvas());

  setRenderer(renderer_);
}

CQGnuPlotCanvas *
CQGnuPlotPlot::
canvas() const
{
  return window_->canvas();
}

void
CQGnuPlotPlot::
setPainter(QPainter *p)
{
  renderer_->setPainter(p);
}

QColor
CQGnuPlotPlot::
lineColor() const
{
  return toQColor(CGnuPlotPlot::lineColor());
}

void
CQGnuPlotPlot::
setLineColor(const QColor &c)
{
  CGnuPlotPlot::setLineColor(fromQColor(c));
}
