#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>
#include <CQToolTip.h>
#include <CQUtil.h>
#include <CQUtilEvent.h>
#include <CQWidgetUtil.h>

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>

class CQGnuPlotCanvasTipLabel : public QWidget {
 public:
  CQGnuPlotCanvasTipLabel(QWidget *parent=nullptr) :
   QWidget(parent) {
  }

  const CGnuPlotTipData &tip() const { return tip_; }

  void setTip(const CGnuPlotTipData &t) {
    tip_ = t;
  }

  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;

    QRectF r(rect());

    double rr = r.height()/6.0;

    path.addRoundedRect(r.adjusted(1, 1, -1, -1), rr, rr);

    QColor bg = palette().color(QPalette::ToolTipBase);

    bg.setAlpha(180);

    painter.fillPath(path, bg);

    QString text;

    if (tip_.ystr() == "") {
      painter.setPen(CQUtil::toQColor(tip_.xcolor()));

      QString text = tip_.xstr().c_str();

      painter.drawText(rect(), Qt::AlignCenter, text);
    }
    else {
#if 0
      // TODO: encode xstr, ystr for HTML or draw manually
      QString html = QString("<font color=\"%1\">%2:</font> <font color=\"%3\">%4</font>").
        arg(CQUtil::colorToHtml(CQUtil::toQColor(tip_.xcolor()))).
        arg(tip_.xstr().c_str()).
        arg(CQUtil::colorToHtml(CQUtil::toQColor(tip_.ycolor()))).
        arg(tip_.ystr().c_str());

      CQUtil::drawHtmlText(this, &painter, html, palette(), rect());
#endif
      QFontMetrics fm(font());

      QString text = QString("%1: %2").arg(tip_.xstr().c_str()).arg(tip_.ystr().c_str());

      int tw = fm.horizontalAdvance(text);

      int dx = (rect().width() - tw)/2;
      int dy = (fm.ascent() - fm.descent())/2;

      painter.setPen(CQUtil::toQColor(tip_.xcolor()));

      QString xtext = QString("%1: ").arg(tip_.xstr().c_str());

      painter.drawText(dx, rect().height()/2 + dy, xtext);

      int xtw = fm.horizontalAdvance(xtext);

      painter.setPen(CQUtil::toQColor(tip_.ycolor()));

      QString ytext = tip_.ystr().c_str();

      painter.drawText(dx + xtw, rect().height()/2 + dy, ytext);
    }

    QPen pen(CQUtil::toQColor(tip_.borderColor()));

    pen.setWidth(2);

    painter.strokePath(path, pen);
  }

  QSize sizeHint() const override {
    QFontMetrics fm(font());

    QString text;

    if (tip_.ystr() == "")
      text = tip_.xstr().c_str();
    else
      text = QString("%1 : %2").arg(tip_.xstr().c_str()).arg(tip_.ystr().c_str());

    return QSize(fm.horizontalAdvance(text) + 2*border_, fm.height() + 2*border_);
  }

 private:
  CGnuPlotTipData tip_;
  int             border_ { 4 };
};

class CQGnuPlotCanvasTip : public CQToolTipIFace {
 public:
  CQGnuPlotCanvasTip(CQGnuPlotCanvas *canvas) :
   canvas_(canvas), label_(nullptr) {
  }

 ~CQGnuPlotCanvasTip() {
    delete label_;
  }

  bool canTip(const QPoint &pos) const override {
    CGnuPlotTipData tip;

    QPoint p = canvas_->mapFromGlobal(pos);

    CGnuPlotMouseEvent mouseEvent;

    mouseEvent.setPixel(CPoint2D(p.x(), p.y()));

    return canvas_->qwindow()->mouseTip(mouseEvent, tip);
  }

  QWidget *showWidget(const QPoint &pos) override {
    if (! label_)
      label_ = new CQGnuPlotCanvasTipLabel();

    if (! updateWidget(pos))
      return label_;

    return label_;
  }

  void hideWidget() override {
    canvas_->qwindow()->highlightObject(nullptr);

    delete label_;

    label_ = nullptr;
  }

  bool trackMouse() const override { return true; }

  bool updateWidget(const QPoint &pos) override {
    QPoint p = canvas_->mapFromGlobal(pos);

    CGnuPlotMouseEvent mouseEvent;

    mouseEvent.setPixel(CPoint2D(p.x(), p.y()));

    if (! canvas_->qwindow()->mouseTip(mouseEvent, tip_))
      return false;

    label_->resize(label_->sizeHint());

    label_->setTip(tip_);

    return true;
  }

  int margin() const override { return 0; }

  double opacity() const override { return 1.0; }

  bool isTransparent() const override { return true; }

  bool outside() const override { return canvas_->qwindow()->isTipOutside(); }

  Qt::Alignment alignment() const override {
    if (outside())
      return Qt::AlignLeft | Qt::AlignBottom;
    else
      return Qt::AlignHCenter | Qt::AlignBottom;
  }

 private:
  CQGnuPlotCanvas         *canvas_;
  CQGnuPlotCanvasTipLabel *label_;
  CGnuPlotTipData          tip_;
};

//----

CQGnuPlotCanvas::
CQGnuPlotCanvas(CQGnuPlotMainWindow *window) :
 QWidget(nullptr), window_(window), pressed_(false)
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

  CGnuPlotMouseEvent mouseEvent;

  mouseEvent.setPixel  (CPoint2D(e->pos().x(), e->pos().y()));
  mouseEvent.setButton (CQUtil::convertButton(e->button()));
  mouseEvent.setShift  (e->modifiers() & Qt::ShiftModifier  );
  mouseEvent.setControl(e->modifiers() & Qt::ControlModifier);
  mouseEvent.setAlt    (e->modifiers() & Qt::AltModifier    );

  CQGnuPlotGroup *group = window_->getGroupAt(mouseEvent.pixel());

  window_->setCurrentGroup(group);

  //if (group) group->mousePress(mouseEvent);

  window_->mousePress(mouseEvent);
}

void
CQGnuPlotCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CGnuPlotMouseEvent mouseEvent;

  mouseEvent.setPixel(CPoint2D(e->pos().x(), e->pos().y()));

  CQGnuPlotGroup *group = window_->getGroupAt(mouseEvent.pixel());

  window_->setCurrentGroup(group);

  QString groupName = (group ? QString("Group%1").arg(group->id()) : QString(""));

  CPoint2D p;

  if (pixelToWindow(mouseEvent.pixel(), p))
    window_->showPos(groupName, e->pos().x(), e->pos().y(), p.x, p.y);

  window_->mouseMove(mouseEvent, pressed_);
}

void
CQGnuPlotCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  CGnuPlotMouseEvent mouseEvent;

  mouseEvent.setPixel  (CPoint2D(e->pos().x(), e->pos().y()));
  mouseEvent.setButton (CQUtil::convertButton(e->button()));
  mouseEvent.setShift  (e->modifiers() & Qt::ShiftModifier  );
  mouseEvent.setControl(e->modifiers() & Qt::ControlModifier);
  mouseEvent.setAlt    (e->modifiers() & Qt::AltModifier    );

  CQGnuPlotGroup *group = window_->getGroupAt(mouseEvent.pixel());

  window_->setCurrentGroup(group);

  if (group)
    group->mouseRelease(mouseEvent);

  window_->mouseRelease(mouseEvent);

  pressed_ = false;
}

void
CQGnuPlotCanvas::
wheelEvent(QWheelEvent *e)
{
  window_->mouseWheel(CQWidgetUtil::wheelDelta(e));
}

void
CQGnuPlotCanvas::
keyPressEvent(QKeyEvent *e)
{
  QPoint pos = this->mapFromGlobal(QCursor::pos());

  CGnuPlotMouseEvent mouseEvent;

  mouseEvent.setPixel(CPoint2D(pos.x(), pos.y()));

  CQGnuPlotGroup *group = window_->getGroupAt(mouseEvent.pixel());

  window_->setCurrentGroup(group);

  CGnuPlotKeyEvent keyEvent;

  keyEvent.setPixel  (CPoint2D(pos.x(), pos.y()));
  keyEvent.setShift  (e->modifiers() & Qt::ShiftModifier  );
  keyEvent.setControl(e->modifiers() & Qt::ControlModifier);
  keyEvent.setAlt    (e->modifiers() & Qt::AltModifier    );
  keyEvent.setType   (CQUtil::convertKey(e->key(), e->modifiers()));
  keyEvent.setKey    (e->key());
  keyEvent.setChar   (CEvent::keyTypeChar(keyEvent.type()));
  keyEvent.setText   (e->text().toStdString());

  window_->keyPress(keyEvent);
}

bool
CQGnuPlotCanvas::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  CQGnuPlotGroup *group = window_->getGroupAt(p);
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

    QPoint p = helpEvent->pos();

    CGnuPlotMouseEvent mouseEvent;

    mouseEvent.setPixel(CPoint2D(p.x(), p.y()));

    CGnuPlotTipData tip;

    if (window_->mouseTip(mouseEvent, tip)) {
      QToolTip::showText(helpEvent->globalPos(), tip.str, this, tip.rect.toRect());
    }

    return true;
  }
#endif

  return QWidget::event(e);
}
