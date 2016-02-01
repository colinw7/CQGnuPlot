#include <CQGnuPlotSaveDialog.h>
#include <CQGnuPlotFilename.h>
#include <CQIntegerSpin.h>
#include <QRadioButton>
#include <QGroupBox>

CQGnuPlotSaveDialog::
CQGnuPlotSaveDialog(QWidget *parent) :
 CQDialog(parent)
{
  setWindowTitle("Save Image");
}

void
CQGnuPlotSaveDialog::
createWidgets(QWidget *)
{
  typeGroup_ = addRadioButtons("Type", {"PNG", "SVG"}, this, SLOT(typeSlot()));

  //---

  file_ = new CQGnuPlotFilename;

  file_->setSave(true);

  addField("Filename", file_);

  addField("Width" , widthEdit_  = new CQIntegerSpin(nullptr, 800));
  addField("Height", heightEdit_ = new CQIntegerSpin(nullptr, 800));
}

void
CQGnuPlotSaveDialog::
typeSlot()
{
}

bool
CQGnuPlotSaveDialog::
isPNG() const
{
  QList<QRadioButton *> buttons = typeGroup_->findChildren<QRadioButton *>();

  return buttons[0]->isChecked();
}

bool
CQGnuPlotSaveDialog::
isSVG() const
{
  QList<QRadioButton *> buttons = typeGroup_->findChildren<QRadioButton *>();

  return buttons[1]->isChecked();
}

QString
CQGnuPlotSaveDialog::
fileName() const
{
  return file_->name();
}

int
CQGnuPlotSaveDialog::
width() const
{
  return widthEdit_->value();
}

int
CQGnuPlotSaveDialog::
height() const
{
  return heightEdit_->value();
}
