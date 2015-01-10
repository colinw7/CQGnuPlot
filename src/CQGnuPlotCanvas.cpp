#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>

#include <QPainter>
#include <QToolTip>

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
  window_->setCurrentGroup(window_->getGroupAt(e->pos()));
}

void
CQGnuPlotCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CQGnuPlotGroup *group = window_->getGroupAt(e->pos());

  if (group) {
    CPoint2D p = group->pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()));

    group->unmapLogPoint(&p.x, &p.y);

    window_->showPos(p.x, p.y);
  }

  window_->mouseMove(e->pos());
}

bool
CQGnuPlotCanvas::
event(QEvent *e)
{
  if (e->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);

    CQGnuPlot::TipRect tip;

    if (window_->mouseTip(helpEvent->pos(), tip))
      QToolTip::showText(helpEvent->globalPos(), tip.str, this, tip.rect.toRect());

    return true;
  }

  return QWidget::event(e);
}
