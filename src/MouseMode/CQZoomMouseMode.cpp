#include <CQZoomMouseMode.h>
#include <CQMenu.h>
#include <CQUtil.h>

#include <QBitmap>
#include <QPen>
#include <QPainter>

#include "xpm/zoom.xpm"

#include "cursors/zoom.xbm"
#include "cursors/zoommask.xbm"

CQZoomMouseMode::
CQZoomMouseMode(CQMouseModeIFace *iface, CQPanZoomIFace *zoomIFace) :
 CQMouseMode(ZOOM_MOUSE_MODE_ID, iface), zoomIFace_(zoomIFace)
{
  zoomCursor_ = QCursor(QBitmap::fromData(QSize(16,16), zoom_bits),
                        QBitmap::fromData(QSize(16,16), zoommask_bits),
                        2, 2);
}

CQMenuItem *
CQZoomMouseMode::
createMenuItem(CQMenu *menu)
{
  CQMenuItem *menuItem = new CQMenuItem(menu, "&Zoom In/Out", CQMenuItem::CHECKABLE);

  menuItem->setStatusTip("Zoom In or Out");
  menuItem->setXPMIcon(zoom_data);

  action_ = menuItem->getAction();

  connect(action_, SIGNAL(toggled(bool)), this, SLOT(menuItemSlot(bool)));

  return menuItem;
}

QAction *
CQZoomMouseMode::
createMenuItem(QMenu *menu)
{
  action_ = new QAction("&Zoom In/Out", menu);

  action_->setCheckable(true);

  action_->setStatusTip("Zoom In or Out");
  action_->setIcon(QIcon(QPixmap(zoom_data)));

  connect(action_, SIGNAL(toggled(bool)), this, SLOT(menuItemSlot(bool)));

  menu->addAction(action_);

  return action_;
}

void
CQZoomMouseMode::
handleMousePress(const MouseEvent &)
{
}

void
CQZoomMouseMode::
handleMouseRelease(const MouseEvent &e)
{
  if      (e.button == Qt::LeftButton) {
    if (moving_) {
      CBBox2D bbox(CQUtil::fromQPoint(press_wpos_), CQUtil::fromQPoint(curr_wpos_));

      zoomIFace_->zoomTo(bbox);
    }
    else {
      zoomIFace_->zoomIn(CQUtil::fromQPoint(curr_wpos_));
    }

    iface_->redrawOverlay();
  }
  else if (e.button == Qt::MidButton) {
    if (! moving_)
      zoomIFace_->panTo(CQUtil::fromQPoint(curr_wpos_));
  }
  else if (e.button == Qt::RightButton) {
    if (moving_) {
      CBBox2D bbox(CQUtil::fromQPoint(press_wpos_), CQUtil::fromQPoint(curr_wpos_));

      zoomIFace_->zoomTo(bbox);
    }
    else {
      zoomIFace_->zoomOut(CQUtil::fromQPoint(curr_wpos_));
    }

    iface_->redrawOverlay();
  }
}

void
CQZoomMouseMode::
handleMouseDrag(const MouseEvent &e)
{
  if      (e.button == Qt::LeftButton) {
    iface_->redrawOverlay();
  }
  else if (e.button == Qt::MidButton) {
    double dx = curr_wpos_.x() - prev_wpos_.x();
    double dy = curr_wpos_.y() - prev_wpos_.y();

    zoomIFace_->panBy(dx, dy);
  }
  else if (e.button == Qt::RightButton) {
    iface_->redrawOverlay();
  }
}

void
CQZoomMouseMode::
handleMouseMove(const MouseEvent &)
{
}

void
CQZoomMouseMode::
handleKeyPress(QKeyEvent *e)
{
  int key = e->key();

  if      (key == Qt::Key_Plus  || key == Qt::Key_Equal)
    zoomIFace_->zoomIn();
  else if (key == Qt::Key_Minus || key == Qt::Key_Underscore)
    zoomIFace_->zoomOut();
  else if  (key == Qt::Key_Left)
    zoomIFace_->panLeft();
  else if (key == Qt::Key_Right)
    zoomIFace_->panRight();
  else if (key == Qt::Key_Down)
    zoomIFace_->panDown();
  else if (key == Qt::Key_Up)
    zoomIFace_->panUp();
  else if (key == Qt::Key_Home) {
    zoomIFace_->resetZoom();
    zoomIFace_->resetPan();
  }
}

void
CQZoomMouseMode::
accept()
{
}

void
CQZoomMouseMode::
cancel()
{
}

void
CQZoomMouseMode::
escape()
{
}

void
CQZoomMouseMode::
drawOverlay(QPainter *painter)
{
  if (pressed_ && button_ != Qt::MidButton) {
    QPen pen;

    pen.setColor(overlayColor_);
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawRect(QRectF(press_wpos_, curr_wpos_));
  }
}

QCursor
CQZoomMouseMode::
getCursor() const
{
  return zoomCursor_;
}
