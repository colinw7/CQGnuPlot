#include <CQPanMouseMode.h>
#include <CQMenu.h>
#include <CQUtil.h>

#include <QBitmap>
#include <QPen>
#include <QPainter>

#include "xpm/pan.xpm"

CQPanMouseMode::
CQPanMouseMode(CQMouseModeIFace *iface, CQPanZoomIFace *panIFace) :
 CQMouseMode(iface->getPanModeId(), iface), panIFace_(panIFace)
{
}

CQMenuItem *
CQPanMouseMode::
createMenuItem(CQMenu *menu)
{
  CQMenuItem *menuItem = new CQMenuItem(menu, "&Pan", CQMenuItem::CHECKABLE);

  menuItem->setStatusTip("Pan Display");
  menuItem->setXPMIcon(pan_data);

  action_ = menuItem->getAction();

  connect(action_, SIGNAL(toggled(bool)), this, SLOT(menuItemSlot(bool)));

  return menuItem;
}

QAction *
CQPanMouseMode::
createMenuItem(QMenu *menu)
{
  action_ = new QAction("&Pan", menu);

  action_->setCheckable(true);

  action_->setStatusTip("Pan Display");
  action_->setIcon(QIcon(QPixmap(pan_data)));

  connect(action_, SIGNAL(toggled(bool)), this, SLOT(menuItemSlot(bool)));

  menu->addAction(action_);

  return action_;
}

void
CQPanMouseMode::
handleMousePress(const MouseEvent &)
{
}

void
CQPanMouseMode::
handleMouseRelease(const MouseEvent &)
{
  if (! moving_)
    panIFace_->panTo(CQUtil::fromQPoint(curr_wpos_));
}

void
CQPanMouseMode::
handleMouseDrag(const MouseEvent &)
{
  int dx = curr_ppos_.x() - prev_ppos_.x();
  int dy = curr_ppos_.y() - prev_ppos_.y();

  QPointF p1 = iface_->pixelToWindow(QPoint( 0,  0));
  QPointF p2 = iface_->pixelToWindow(QPoint(dx, dy));

  panIFace_->panBy(p2.x() - p1.x(), p2.y() - p1.y());
}

void
CQPanMouseMode::
handleMouseMove(const MouseEvent &)
{
}

void
CQPanMouseMode::
handleKeyPress(QKeyEvent *e)
{
  int key = e->key();

  if      (key == Qt::Key_Left)
    panIFace_->panLeft();
  else if (key == Qt::Key_Right)
    panIFace_->panRight();
  else if (key == Qt::Key_Down)
    panIFace_->panDown();
  else if (key == Qt::Key_Up)
    panIFace_->panUp();
  else if (key == Qt::Key_Home)
    panIFace_->resetPan();
}

void
CQPanMouseMode::
accept()
{
}

void
CQPanMouseMode::
cancel()
{
}

void
CQPanMouseMode::
escape()
{
}

void
CQPanMouseMode::
drawOverlay(QPainter *)
{
}

QCursor
CQPanMouseMode::
getCursor() const
{
  return Qt::OpenHandCursor;
}
