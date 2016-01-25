#include <CQDialog.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>

CQDialog::
CQDialog(QWidget *parent, uint buttons) :
 QDialog(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  //---

  frame_ = new CQDialogForm;

  layout->addWidget(frame_);

  layout->addStretch(1);

  //---

  QHBoxLayout *buttonLayout = new QHBoxLayout;

  QFrame *buttonFrame = new QFrame;

  buttonFrameLayout_ = new QHBoxLayout(buttonFrame);
  buttonFrameLayout_->setMargin(0); buttonFrameLayout_->setSpacing(2);

  buttonLayout->addWidget(buttonFrame);
  buttonLayout->addStretch();

  if (buttons & BUTTON_OK) {
    QPushButton *ok = new QPushButton("OK");

    connect(ok, SIGNAL(clicked()), this, SLOT(acceptSlot()));

    buttonLayout->addWidget(ok);
  }

  if (buttons & BUTTON_APPLY) {
    QPushButton *apply = new QPushButton("Apply");

    connect(apply, SIGNAL(clicked()), this, SLOT(applySlot()));

    buttonLayout->addWidget(apply);
  }

  if (buttons & BUTTON_CANCEL) {
    QPushButton *cancel = new QPushButton("Cancel");

    connect(cancel, SIGNAL(clicked()), this, SLOT(rejectSlot()));

    buttonLayout->addWidget(cancel);
  }

  layout->addLayout(buttonLayout);
}

void
CQDialog::
init()
{
  createWidgets(frame_);

  initialized_ = true;
}

int
CQDialog::
exec()
{
  if (! initialized_)
    init();

  return QDialog::exec();
}

QCheckBox *
CQDialog::
addCheckBox(const QString &name, const QObject *receiver, const char *member)
{
  return frame_->addCheckBox(name, receiver, member);
}

QGroupBox *
CQDialog::
addRadioButtons(const QString &name, const std::vector<QString> &names,
                const QObject *receiver, const char *member)
{
  return frame_->addRadioButtons(name, names, receiver, member);
}

void
CQDialog::
addField(const QString &name, QWidget *w, bool stretch)
{
  frame_->addField(name, w, stretch);
}

QPushButton *
CQDialog::
addButton(const QString &name)
{
  QPushButton *pb = new QPushButton(name);

  buttonFrameLayout_->addWidget(pb);

  return pb;
}

void
CQDialog::
acceptSlot()
{
  accepted_ = true;

  accept();

  emit accepted();

  QDialog::accept();
}

void
CQDialog::
applySlot()
{
  accepted_ = true;

  accept();

  emit accepted();

  accepted_ = false;
}

void
CQDialog::
rejectSlot()
{
  accepted_ = false;

  reject();

  QDialog::reject();
}

//------

CQDialogForm::
CQDialogForm(QWidget *parent) :
 QWidget(parent)
{
}

QCheckBox *
CQDialogForm::
addCheckBox(const QString &name, const QObject *receiver, const char *member)
{
  QCheckBox *check = new QCheckBox;

  addField(name, check);

  if (receiver)
    connect(check, SIGNAL(clicked(bool)), receiver, member);

  return check;
}

QGroupBox *
CQDialogForm::
addRadioButtons(const QString &name, const std::vector<QString> &names,
                const QObject *receiver, const char *member)
{
  QGroupBox *box = new QGroupBox;

  QHBoxLayout *layout = new QHBoxLayout(box);
  layout->setMargin(0); layout->setSpacing(2);

  int i = 0;

  for (const auto &n : names) {
    QRadioButton *button = new QRadioButton(n);

    layout->addWidget(button);

    button->setChecked(! (i++));
  }

  layout->addStretch();

  addField(name, box);

  if (receiver)
    connect(box, SIGNAL(clicked(bool)), receiver, member);

  return box;
}

void
CQDialogForm::
addField(const QString &name, QWidget *w, bool stretch)
{
  if (! grid_) {
    grid_ = new QGridLayout(this);

    if (stretch)
      grid_->setColumnStretch(2, 1);
  }

  if (name.length()) {
    grid_->addWidget(new QLabel(name), row_, 0);
    grid_->addWidget(w               , row_, 1);
  }
  else
    grid_->addWidget(w, row_, 0, 1, 2);

  ++row_;
}

void
CQDialogForm::
addStretch()
{
  if (grid_)
    grid_->setRowStretch(row_, 1);
}
