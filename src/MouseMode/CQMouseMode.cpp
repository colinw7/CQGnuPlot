#include <CQMouseMode.h>
#include <CQMenu.h>
#include <CQToolBar.h>

#include <cassert>

CQMouseModeMgr *
CQMouseModeMgr::
getInstance()
{
  static CQMouseModeMgr *instance;

  if (! instance)
    instance = new CQMouseModeMgr;

  return instance;
}

CQMouseModeMgr::
CQMouseModeMgr() :
 currentId_(0)
{
}

void
CQMouseModeMgr::
addMode(uint id, CQMouseMode *mode)
{
  assert(id > 0 && modeMap_.find(id) == modeMap_.end());

  modeMap_[id] = mode;
}

CQMouseMode *
CQMouseModeMgr::
getMode(uint id) const
{
  ModeMap::const_iterator p = modeMap_.find(id);

  if (p != modeMap_.end())
    return (*p).second;

  if (! modeMap_.empty())
    return (*modeMap_.begin()).second;

  return NULL;
}

void
CQMouseModeMgr::
setCurrentMode(uint id)
{
  assert(id > 0 && modeMap_.find(id) != modeMap_.end());

  currentId_ = id;

  ModeMap::const_iterator p1, p2;

  for (p1 = modeMap_.begin(), p2 = modeMap_.end(); p1 != p2; ++p1) {
    QAction *action = (*p1).second->getAction();
    assert(action);

    action->setChecked((*p1).first == currentId_);
  }
}

CQMouseMode *
CQMouseModeMgr::
getCurrentMode() const
{
  return getMode(currentId_);
}

void
CQMouseModeMgr::
createMenuItems(CQMenu *menu)
{
  ModeMap::const_iterator p1, p2;

  for (p1 = modeMap_.begin(), p2 = modeMap_.end(); p1 != p2; ++p1)
    (*p1).second->createMenuItem(menu);
}

void
CQMouseModeMgr::
createMenuItems(QMenu *menu)
{
  ModeMap::const_iterator p1, p2;

  for (p1 = modeMap_.begin(), p2 = modeMap_.end(); p1 != p2; ++p1)
    (*p1).second->createMenuItem(menu);
}

void
CQMouseModeMgr::
addToToolBar(CQToolBar *toolbar)
{
  ModeMap::const_iterator p1, p2;

  for (p1 = modeMap_.begin(), p2 = modeMap_.end(); p1 != p2; ++p1)
    toolbar->addItem((*p1).second->getAction());
}

//----------

CQMouseMode::
CQMouseMode(uint id, CQMouseModeIFace *iface) :
 id_(id), iface_(iface), action_(0), pressed_(false), button_(Qt::LeftButton), moving_(false)
{
  CQMouseModeMgrInst->addMode(id_, this);

  overlayColor_ = QColor(0,0,0);
}

void
CQMouseMode::
mousePress(const MouseEvent &e)
{
  pressed_ = true;
  button_  = e.button;
  moving_  = false;

  press_ppos_ = e.pixel;
  press_wpos_ = e.window;

  prev_ppos_ = press_ppos_;
  prev_wpos_ = press_wpos_;
  curr_ppos_ = press_ppos_;
  curr_wpos_ = press_wpos_;

  handleMousePress(e);
}

void
CQMouseMode::
mouseRelease(const MouseEvent &e)
{
  pressed_ = false;

  handleMouseRelease(e);
}

void
CQMouseMode::
mouseDrag(const MouseEvent &e)
{
  MouseEvent e1 = e;

  e1.button = button_;

  prev_ppos_ = curr_ppos_;
  prev_wpos_ = curr_wpos_;
  curr_ppos_ = e1.pixel;
  curr_wpos_ = e1.window;

  if (! moving_) {
    int dx = abs(curr_ppos_.x() - press_ppos_.x());
    int dy = abs(curr_ppos_.y() - press_ppos_.y());

    if (dx >= 4 || dy >= 4)
      moving_ = true;
  }

  handleMouseDrag(e1);
}

void
CQMouseMode::
mouseMove(const MouseEvent &e)
{
  prev_ppos_ = curr_ppos_;
  prev_wpos_ = curr_wpos_;
  curr_ppos_ = e.pixel;
  curr_wpos_ = e.window;

  handleMouseMove(e);
}

void
CQMouseMode::
keyPress(QKeyEvent *e)
{
  handleKeyPress(e);
}

void
CQMouseMode::
menuItemSlot(bool checked)
{
  if (! checked) return;

  static bool locked;

  if (locked) return;

  locked = true;

  CQMouseModeMgrInst->setCurrentMode(id_);

  iface_->setCursor(getCursor());

  locked = false;
}

void
CQMouseMode::
escape()
{
  pressed_ = false;
  moving_  = false;
}

void
CQMouseMode::
drawOverlay(QPainter *)
{
}
