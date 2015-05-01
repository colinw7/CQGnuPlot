#ifndef CQLineEditMenu_H
#define CQLineEditMenu_H

#include <QFrame>
#include <QLineEdit>
#include <QMenu>
#include <QWidgetAction>

class QTimer;
class QStackedWidget;

class CQLineEditMenuMenu;
class CQLineEditMenuEdit;
class CQLineEditMenuArea;
class CQLineEditMenuAction;

class CQLineEditContainer : public QWidget {
  Q_OBJECT

 public:
  CQLineEditContainer(QWidget *parent);

  void addWidget(QWidget *w);
  QWidget *removeWidget();

 private:
  QStackedWidget *stack_;
  QWidget        *spacer_;
  QWidget        *w_;
};

// class to support a line edit and a popup menu dialog containing
// the same line edit with extra controls
class CQLineEditMenu : public CQLineEditContainer {
  Q_OBJECT

 public:
  CQLineEditMenu(QWidget *parent);

  void setMenuWidget(QWidget *w) { menuWidget_ = w; }
  QWidget *getMenuWidget() const { return menuWidget_; }

  QLineEdit *lineEdit() const;

 private slots:
  void menuShowSlot();
  void menuHideSlot();

 public slots:
  void startTimer();
  void stopTimer();

  void showMenu();
  void hideMenu();

 private:
  CQLineEditMenuMenu *menu_;
  QWidget            *menuWidget_;
  int                 timeout_;
  CQLineEditMenuEdit *lineEdit_;
  QTimer             *timer_;
  bool                inMenu_;
 private:
};

class CQLineEditMenuMenu : public QMenu {
  Q_OBJECT

 public:
  CQLineEditMenuMenu(CQLineEditMenu *edit);

  CQLineEditMenu *getEdit() const { return edit_; }

  void display();

  void showWidgets();
  void hideWidgets();

 private:
  CQLineEditMenu       *edit_;
  CQLineEditMenuAction *action_;
};

class CQLineEditMenuEdit : public QLineEdit {
  Q_OBJECT

 public:
  CQLineEditMenuEdit(CQLineEditMenu *edit);

 private:
  void focusInEvent (QFocusEvent *);
  void focusOutEvent(QFocusEvent *);

  bool event(QEvent *e);

 private:
  CQLineEditMenu *edit_;
};

class CQLineEditMenuAction : public QWidgetAction {
  Q_OBJECT

 public:
  CQLineEditMenuAction(CQLineEditMenuMenu *menu);

  QWidget *getWidget() const;

  CQLineEditMenuArea *getArea() const { return area_; }

 private:
  QWidget *createWidget(QWidget *parent);

 private:
  CQLineEditMenuMenu *menu_;
  CQLineEditMenuArea *area_;
};

class CQLineEditMenuArea : public QFrame {
 public:
  CQLineEditMenuArea(CQLineEditMenuMenu *menu);

  void showWidgets();
  void hideWidgets();

 private:
  CQLineEditMenuMenu *menu_;
  QWidget        *toolHolder_;
  QWidget        *menuHolder_;
};

#endif
