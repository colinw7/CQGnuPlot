#include <CQLineEditMenu.h>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QApplication>
#include <QTimer>
#include <QKeyEvent>

CQLineEditMenu::
CQLineEditMenu(QWidget *parent) :
 CQLineEditContainer(parent), menuWidget_(0), timeout_(100), inMenu_(false)
{
  setObjectName("editMenu");

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

  lineEdit_ = new CQLineEditMenuEdit(this);

  CQLineEditContainer::addWidget(lineEdit_);

  menu_ = new CQLineEditMenuMenu(this);

  connect(menu_, SIGNAL(aboutToShow()), this, SLOT(menuShowSlot()));
  connect(menu_, SIGNAL(aboutToHide()), this, SLOT(menuHideSlot()));

  timer_ = new QTimer(this);

  connect(timer_, SIGNAL(timeout()), this, SLOT(showMenu()));
}

void
CQLineEditMenu::
menuShowSlot()
{
  //QWidget *focusWidget = QApplication::focusWidget();

  //lineEdit_->clearFocus();

  menu_->showWidgets();

  lineEdit_->setFocus();

  //if (focusWidget) focusWidget->setFocus();

  inMenu_ = true;
}

void
CQLineEditMenu::
menuHideSlot()
{
  //QWidget *focusWidget = QApplication::focusWidget();

  //if (qobject_cast<CQLineEditMenuEdit *>(focusWidget)) focusWidget->clearFocus();

  lineEdit_->clearFocus();

  menu_->hideWidgets();

  lineEdit_->clearFocus();

  inMenu_ = false;
}

void
CQLineEditMenu::
showMenu()
{
  if (! inMenu_)
    menu_->display();
}

void
CQLineEditMenu::
hideMenu()
{
  lineEdit_->clearFocus();

  menu_->hide();
}

void
CQLineEditMenu::
startTimer()
{
  timer_->start(timeout_);
}

void
CQLineEditMenu::
stopTimer()
{
  timer_->stop();
}

//------

CQLineEditContainer::
CQLineEditContainer(QWidget *parent) :
 QWidget(parent), w_(0)
{
  setObjectName("container");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  stack_ = new QStackedWidget;
  stack_->setObjectName("stack");

  stack_->layout()->setSizeConstraint(QLayout::SetMinimumSize);

  layout->addWidget(stack_);

  spacer_ = new QWidget;
  spacer_->setObjectName("spacer");

  spacer_->setFixedSize(1, 1);

  stack_->addWidget(spacer_);
}

void
CQLineEditContainer::
addWidget(QWidget *w)
{
  w_ = w;

  stack_->addWidget(w_);

  stack_->setCurrentWidget(w_);

  spacer_->setFixedSize(1, 1);
}

QWidget *
CQLineEditContainer::
removeWidget()
{
  spacer_->setFixedSize(w_->size());

  stack_->setCurrentWidget(spacer_);

  stack_->removeWidget(w_);

  return w_;
}

//------

CQLineEditMenuMenu::
CQLineEditMenuMenu(CQLineEditMenu *edit) :
 QMenu(edit), edit_(edit)
{
  setObjectName("menu");

  action_ = new CQLineEditMenuAction(this);

  addAction(action_);
}

void
CQLineEditMenuMenu::
display()
{
  QPoint tl = edit_->mapToGlobal(edit_->rect().topLeft());

  popup(tl);
}

void
CQLineEditMenuMenu::
showWidgets()
{
  action_->getArea()->showWidgets();
}

void
CQLineEditMenuMenu::
hideWidgets()
{
  action_->getArea()->hideWidgets();
}

//------

CQLineEditMenuEdit::
CQLineEditMenuEdit(CQLineEditMenu *edit) :
 QLineEdit(edit), edit_(edit)
{
  setObjectName("edit");

  setFocusPolicy(Qt::ClickFocus);

  connect(this, SIGNAL(textChanged(const QString &)), edit_, SLOT(startTimer()));

  connect(this, SIGNAL(returnPressed()), edit_, SLOT(hideMenu()));
}

void
CQLineEditMenuEdit::
focusInEvent(QFocusEvent *)
{
  edit_->startTimer();
}

void
CQLineEditMenuEdit::
focusOutEvent(QFocusEvent *)
{
  edit_->stopTimer();
}

bool
CQLineEditMenuEdit::
event(QEvent *e)
{
  if (e->type() == QEvent::KeyPress ||
      e->type() == QEvent::KeyRelease ||
      e->type() == QEvent::ShortcutOverride) {
    QKeyEvent *ke = dynamic_cast<QKeyEvent *>(e);

    if (ke == NULL) return false;

    int key = ke->key();

    if (key == Qt::Key_Escape) {
      edit_->hideMenu();

      return true;
    }
  }

  return QLineEdit::event(e);
}

//------

CQLineEditMenuAction::
CQLineEditMenuAction(CQLineEditMenuMenu *menu) :
 QWidgetAction(menu), menu_(menu)
{
  setObjectName("action");

  area_ = new CQLineEditMenuArea(menu_);
}

QWidget *
CQLineEditMenuAction::
getWidget() const
{
  return area_;
}

QWidget *
CQLineEditMenuAction::
createWidget(QWidget *parent)
{
  area_->setParent(parent);

  return area_;
}

//-----

CQLineEditMenuArea::
CQLineEditMenuArea(CQLineEditMenuMenu *menu) :
 QFrame(menu), menu_(menu)
{
  setObjectName("area");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  menuHolder_ = new QWidget;

  layout->addWidget(menuHolder_);
}

void
CQLineEditMenuArea::
showWidgets()
{
  CQLineEditMenu *edit = menu_->getEdit();

  QWidget *menuWidget = edit->getMenuWidget();

  CQLineEditContainer *container = menuWidget->findChild<CQLineEditContainer *>();

  if (container)
    container->addWidget(edit->removeWidget());

  int w = menuWidget->sizeHint().width();

  menuWidget->setParent(menuHolder_);

  menuHolder_->setFixedHeight(menuWidget->sizeHint().height());

  menuHolder_->setFixedWidth(w);

  menuWidget->setFixedWidth(w);
}

void
CQLineEditMenuArea::
hideWidgets()
{
  CQLineEditMenu *edit = menu_->getEdit();

  QWidget *menuWidget = edit->getMenuWidget();

  CQLineEditContainer *container = menuWidget->findChild<CQLineEditContainer *>();

  if (container)
    edit->addWidget(container->removeWidget());
}
