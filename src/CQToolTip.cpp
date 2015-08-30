#include <CQToolTip.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QCursor>
#include <QKeyEvent>
#include <QHelpEvent>
#include <QToolTip>
#include <QTimer>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionFrame>

#include <cassert>

CQToolTip *
CQToolTip::
getInstance()
{
  static CQToolTip *instance;

  if (! instance)
    instance = new CQToolTip;

  return instance;
}

// the tooltip widget
CQToolTip::
CQToolTip() :
 QWidget(0, Qt::Window | Qt::FramelessWindowHint),
 tooltips_(), tooltip_(), parent_(), hideSecs_(3), hideTimer_(0),
 margin_(style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth)),
 opacity_(style()->styleHint(QStyle::SH_ToolTipLabel_Opacity)/255.0)
{
  setAttribute(Qt::WA_TranslucentBackground);

  //setPalette(QToolTip::palette());
  setWindowOpacity(opacity_);

  layout_ = new QVBoxLayout(this);
  layout_->setMargin(margin_); layout_->setSpacing(0);

  qApp->installEventFilter(this);
}

CQToolTip::
~CQToolTip()
{
  qApp->removeEventFilter(this);
}

void
CQToolTip::
show(const QPoint &pos, CQToolTipIFace *tooltip, QWidget *parent)
{
  if (isVisible()) return;

  if (! tooltip->canTip(pos))
    return;

  QDesktopWidget *desktop = QApplication::desktop();

  if (desktop->isVirtualDesktop())
    setParent(desktop->screen(desktop->screenNumber(pos)));
  else
    setParent(desktop->screen(desktop->screenNumber(this)));

  setWindowFlags(Qt::ToolTip);

  QWidget *tipWidget = tooltip->showWidget(pos);

  tooltip_ = tipWidget;
  parent_  = parent;

  for (int i = 0; i < layout_->count(); ++i) {
    QLayoutItem *item = layout_->takeAt(i);

    if (item->widget())
      item->widget()->hide();
  }

  layout_->addWidget(tooltip_);

  if (tooltip->margin() >= 0)
    layout_->setMargin(tooltip->margin());
  else
    layout_->setMargin(margin_);

  tooltip_->show();

  showAtPos(pos);

  updateOpacity(tooltip);

  tooltip_->update();

  startHideTimer();
}

void
CQToolTip::
updateOpacity(CQToolTipIFace *tooltip)
{
  double o = opacity_;

  if (tooltip->opacity() >= 0)
    o = tooltip->opacity();

  setWindowOpacity(o);

  //tooltip_->setWindowOpacity(o);
}

void
CQToolTip::
showAtPos(const QPoint &pos)
{
  //QSize size = w_->cursor().bitmap()->size();
  QSize size(6,16);

  int cw = size.width ();
  int ch = size.height();

  int x = pos.x() + cw;
  int y = pos.y() + ch;

  QPoint pos1 = QPoint(x, y);

  move(pos1);

  QWidget::show();
}

void
CQToolTip::
enterEvent(QEvent *)
{
  hideLater();
}

void
CQToolTip::
paintEvent(QPaintEvent *)
{
  CQToolTipIFace *tooltip = getToolTip(parent_);

  if (tooltip && tooltip->isTransparent())
    return;

  QStylePainter painter(this);

  QStyleOptionFrame opt;

  opt.initFrom(this);

  painter.drawPrimitive(QStyle::PE_PanelTipLabel, opt);
}

bool
CQToolTip::
eventFilter(QObject *o, QEvent *e)
{
  switch (e->type()) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
      int                   key = static_cast<QKeyEvent *>(e)->key();
      Qt::KeyboardModifiers mod = static_cast<QKeyEvent *>(e)->modifiers();

      if ((mod & Qt::KeyboardModifierMask) ||
          (key == Qt::Key_Shift || key == Qt::Key_Control ||
           key == Qt::Key_Alt || key == Qt::Key_Meta))
        break;

      hideLater();

      break;
    }
    case QEvent::Leave:
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::Wheel:
      hideLater();
      break;
    case QEvent::MouseMove: {
      if (isVisible()) {
        QWidget *parent = static_cast<QWidget *>(o);

        CQToolTipIFace *tooltip = getToolTip(parent);

        if (tooltip) {
          QPoint pos = ((QMouseEvent *) e)->globalPos();

          showAtPos(pos);

          if (! tooltip->updateWidget(pos)) {
            //hideLater();
            return false;
          }

          updateOpacity(tooltip);

          tooltip_->update();
        }
      }

      startHideTimer();

      break;
    }
    case QEvent::ToolTip: {
      QWidget *parent = static_cast<QWidget *>(o);

      CQToolTipIFace *tooltip = getToolTip(parent);

      if (tooltip && ! isVisible()) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);

        show(helpEvent->globalPos(), tooltip, parent);

        if (tooltip->trackMouse())
          setMouseTracking(true);

        return true;
      }

      break;
    }
    default:
      break;
  }

  return false;
}

void
CQToolTip::
timerEvent(QTimerEvent *event)
{
  if (event->timerId() == hideTimer_)
    hideLater();
}

void
CQToolTip::
hideLater()
{
  if (! isVisible()) return;

  if (hideTimer_) {
    killTimer(hideTimer_);

    hideTimer_ = 0;
  }

  QTimer::singleShot(0, this, SLOT(hideSlot()));
}

void
CQToolTip::
startHideTimer()
{
  if (hideTimer_)
    killTimer(hideTimer_);

  double hideSecs =  hideSecs_;

  CQToolTipIFace *tooltip = getToolTip(parent_);

  if (tooltip && tooltip->hideSecs() > 0)
    hideSecs = tooltip->hideSecs();

  hideTimer_ = startTimer(hideSecs*1000);
}

void
CQToolTip::
hideSlot()
{
  this->hide();

  CQToolTipIFace *tooltip = getToolTip(parent_);

  //if (tooltip && tooltip->trackMouse())
  setMouseTracking(false);

  if (tooltip)
    tooltip->hideWidget();
}

void
CQToolTip::
setToolTip(QWidget *parent, QWidget *tooltip)
{
  setToolTip(parent, new CQToolTipWidgetIFace(tooltip));
}

void
CQToolTip::
setToolTip(QWidget *parent, CQToolTipIFace *tooltip)
{
  assert(parent);

  CQToolTip *inst = CQToolTip::getInstance();

  if (inst->tooltips_.contains(parent)) {
    delete inst->tooltips_.value(parent);

    inst->tooltips_.remove(parent);
  }

  if (tooltip) {
    inst->hide();

    inst->tooltips_[parent] = tooltip;
  }
}

void
CQToolTip::
unsetToolTip(QWidget *parent)
{
  setToolTip(parent, (CQToolTipIFace *) 0);
}

CQToolTipIFace *
CQToolTip::
getToolTip(QWidget *parent) const
{
  if (! tooltips_.contains(parent))
    return 0;

  return tooltips_.value(parent);
}
