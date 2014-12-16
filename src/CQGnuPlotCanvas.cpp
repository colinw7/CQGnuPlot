#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>

#include <QPainter>

CQGnuPlotCanvas::
CQGnuPlotCanvas(CQGnuPlotWindow *window) :
 QWidget(0), window_(window)
{
  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

CQGnuPlotCanvas::
~CQGnuPlotCanvas()
{
}

void
CQGnuPlotCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  window_->paint(&p);
}

void
CQGnuPlotCanvas::
mousePressEvent(QMouseEvent *e)
{
  window_->setCurrentPlot(window_->getPlotAt(e->pos()));
}

void
CQGnuPlotCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CQGnuPlotPlot *plot = window_->getPlotAt(e->pos());
  if (! plot) return;

  CPoint2D p = plot->pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()));

  plot->unmapLogPoint(&p.x, &p.y);

  window_->showPos(p.x, p.y);
}
