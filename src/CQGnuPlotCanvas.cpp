#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>
#include <CQToolTip.h>
#include <CQUtil.h>

#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

class CQGnuPlotCanvasTipLabel : public QWidget {
 public:
  CQGnuPlotCanvasTipLabel(QWidget *parent=0) :
   QWidget(parent) {
  }

  const CGnuPlotTipData &tip() const { return tip_; }

  void setTip(const CGnuPlotTipData &t) {
    tip_ = t;
  }

  void paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;

    QRectF r(rect());

    path.addRoundedRect(r.adjusted(1,1,-1,-1), r.width()/6.0, r.height()/6.0);

    painter.fillPath(path, QColor(255,255,255,127));

    QString text;

    if (tip_.ystr() == "") {
      painter.setPen(CQUtil::toQColor(tip_.xcolor()));

      QString text = tip_.xstr().c_str();

      painter.drawText(rect(), Qt::AlignCenter, text);
    }
    else {
      QString html = QString("<font color=\"%1\">%2:</font> <font color=\"%3\">%4</font>").
        arg(CQUtil::colorToHtml(CQUtil::toQColor(tip_.xcolor()))).
        arg(tip_.xstr().c_str()).
        arg(CQUtil::colorToHtml(CQUtil::toQColor(tip_.ycolor()))).
        arg(tip_.ystr().c_str());

      CQUtil::drawHtmlText(this, &painter, html, palette(), rect());
    }

    QPen pen(CQUtil::toQColor(tip_.borderColor()));

    pen.setWidth(2);

    painter.strokePath(path, pen);
  }

  QSize sizeHint() const {
    QFontMetrics fm(font());

    QString text;

    if (tip_.ystr() == "")
      text = tip_.xstr().c_str();
    else
      text = QString("%1 : %2").arg(tip_.xstr().c_str()).arg(tip_.ystr().c_str());

    return QSize(fm.width(text) + 2*border_, fm.height() + 2*border_);
  }

 private:
  CGnuPlotTipData tip_;
  int             border_ { 4 };
};

class CQGnuPlotCanvasTip : public CQToolTipIFace {
 public:
  CQGnuPlotCanvasTip(CQGnuPlotCanvas *canvas) :
   canvas_(canvas), label_(0) {
  }

 ~CQGnuPlotCanvasTip() {
    delete label_;
  }

  bool canTip(const QPoint &pos) const {
    CGnuPlotTipData tip;

    return canvas_->qwindow()->mouseTip(canvas_->mapFromGlobal(pos), tip);
  }

  QWidget *showWidget(const QPoint &pos) {
    if (! label_)
      label_ = new CQGnuPlotCanvasTipLabel();

    if (! updateWidget(pos))
      return label_;

    return label_;
  }

  void hideWidget() {
    delete label_;

    label_ = 0;
  }

  bool trackMouse() const { return true; }

  bool updateWidget(const QPoint &pos) {
    if (! canvas_->qwindow()->mouseTip(canvas_->mapFromGlobal(pos), tip_))
      return false;

    label_->setTip(tip_);

    return true;
  }

  int margin() const { return 0; }

  double opacity() const { return 1.0; }

  bool isTransparent() const { return true; }

 private:
  CQGnuPlotCanvas         *canvas_;
  CQGnuPlotCanvasTipLabel *label_;
  CGnuPlotTipData          tip_;
};

//----

CQGnuPlotCanvas::
CQGnuPlotCanvas(CQGnuPlotWindow *window) :
 QWidget(0), window_(window), pressed_(false)
{
  setObjectName("canvas");

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);

  CQToolTip::setToolTip(this, new CQGnuPlotCanvasTip(this));
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
  pressed_ = true;

  CQGnuPlotGroup *group = window_->getGroupAt(e->pos());

  window_->setCurrentGroup(group);

  if (group)
    group->mousePress(e->pos());

  window_->mousePress(e->pos());
}

void
CQGnuPlotCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CQGnuPlotGroup *group = window_->getGroupAt(e->pos());

  QString groupName = (group ? QString("Group%1").arg(group->id()) : QString(""));

  CPoint2D p;

  if (pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()), p))
    window_->showPos(groupName, e->pos().x(), e->pos().y(), p.x, p.y);

  window_->mouseMove(e->pos(), pressed_);
}

void
CQGnuPlotCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  CQGnuPlotGroup *group = window_->getGroupAt(e->pos());

  window_->setCurrentGroup(group);

  if (group)
    group->mouseRelease(e->pos());

  window_->mouseRelease(e->pos());

  pressed_ = false;
}

void
CQGnuPlotCanvas::
keyPressEvent(QKeyEvent *e)
{
  QPoint pos = this->mapFromGlobal(QCursor::pos());

  CQGnuPlotGroup *group = window_->getGroupAt(pos);

  window_->setCurrentGroup(group);

  window_->keyPress(e->key(), e->modifiers());
}

bool
CQGnuPlotCanvas::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  QPoint pos(p.x, p.y);

  CQGnuPlotGroup *group = window_->getGroupAt(pos);
  if (! group) return false;

  CGnuPlotRenderer *renderer = group->app()->renderer();

  renderer->setRegion(group->region());

  renderer->setRange(group->getMappedDisplayRange(1, 1));

  renderer->pixelToWindow(p, w);

  double z = 0;

  group->unmapLogPoint(1, 1, 1, &w.x, &w.y, &z);

  return true;
}

bool
CQGnuPlotCanvas::
event(QEvent *e)
{
#if 0
  if (e->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);

    CGnuPlotTipData tip;

    if (window_->mouseTip(helpEvent->pos(), tip))
      QToolTip::showText(helpEvent->globalPos(), tip.str, this, tip.rect.toRect());

    return true;
  }
#endif

  return QWidget::event(e);
}
