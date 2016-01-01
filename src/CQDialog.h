#ifndef CQDIALOG_H
#define CQDIALOG_H

#include <QDialog>

class QGridLayout;
class QCheckBox;
class QGroupBox;
class QHBoxLayout;

class CQDialog : public QDialog {
  Q_OBJECT

 public:
  enum ButtonType {
    BUTTON_NONE   = 0,
    BUTTON_OK     = (1<<0),
    BUTTON_APPLY  = (1<<1),
    BUTTON_CANCEL = (1<<2),
    BUTTON_ALL    = (BUTTON_OK | BUTTON_APPLY | BUTTON_CANCEL)
  };

 public:
  CQDialog(QWidget *parent=0, uint buttons=BUTTON_ALL);

  virtual ~CQDialog() { }

  void init();

  int exec();

  QCheckBox *addCheckBox(const QString &name, const QObject *receiver, const char *member);

  QGroupBox *addRadioButtons(const QString &name, const std::vector<QString> &names,
                             const QObject *receiver, const char *member);

  void addField(const QString &name, QWidget *w);

  QPushButton *addButton(const QString &name);

  bool accepted() const { return accepted_; }

 protected:
  virtual void createWidgets(QWidget *) { }

  virtual void accept() { }
  virtual void reject() { }

 protected slots:
  void acceptSlot();
  void applySlot ();
  void rejectSlot();

 private:
  QWidget     *frame_             { 0 };
  bool         initialized_       { false };
  bool         accepted_          { false };
  QGridLayout *grid_              { 0 };
  mutable int  row_               { 0 };
  QHBoxLayout *buttonFrameLayout_ { 0 };
};

#endif
