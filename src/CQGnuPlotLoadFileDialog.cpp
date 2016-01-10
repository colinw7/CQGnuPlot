#include <CQGnuPlotLoadFileDialog.h>
#include <CQGnuPlotUtil.h>
#include <CQEnumCombo.h>
#include <CQIntegerSpin.h>
#include <CQRealSpin.h>

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QFileDialog>

#include <svg/file_dialog_svg.h>

CQGnuPlotLoadFileDialog::
CQGnuPlotLoadFileDialog(QWidget *parent) :
 CQDialog(parent)
{
  enum_ = new CQGnuPlotEnum;

  enum_->setPlotStyle(CQGnuPlotEnum::PlotLinesPoints);
}

void
CQGnuPlotLoadFileDialog::
createWidgets(QWidget *)
{
  addField("Style", new CQEnumCombo(this, enum_, "plotStyle"));

  dimensionGroup_ = addRadioButtons("Dimension", {"2D", "3D"}, this, SLOT(dimensionSlot()));

  binaryCheck_ = addCheckBox("Binary", this, SLOT(binarySlot()));

  parametricCheck_ = addCheckBox("Parametric", this, SLOT(parametricSlot()));

  file_ = new CQGnuPlotFilename;

  addField("Filename", file_);

  addField("XMin", (xminEdit_ = new CQRealSpin));
  addField("XMax", (xmaxEdit_ = new CQRealSpin));

  addField("Using", (usingEdit_ = new QLineEdit));

  addField("Line Type", (lineTypeEdit_ = new CQIntegerSpin));

  xminEdit_    ->setValue(-10);
  xmaxEdit_    ->setValue(10);
  lineTypeEdit_->setValue(1);
}

bool
CQGnuPlotLoadFileDialog::
is2D() const
{
  QList<QRadioButton *> buttons = dimensionGroup_->findChildren<QRadioButton *>();

  return buttons[0]->isChecked();
}

bool
CQGnuPlotLoadFileDialog::
isParametric() const
{
  return parametricCheck_->isChecked();
}

bool
CQGnuPlotLoadFileDialog::
isBinary() const
{
  return binaryCheck_->isChecked();
}

CGnuPlotTypes::PlotStyle
CQGnuPlotLoadFileDialog::
plotStyle() const
{
  return CQGnuPlotEnum::plotStyleConv(enum_->plotStyle());
}

QString
CQGnuPlotLoadFileDialog::
fileName() const
{
  return file_->name();
}

void
CQGnuPlotLoadFileDialog::
dimensionSlot()
{
}

void
CQGnuPlotLoadFileDialog::
binarySlot()
{
}

double
CQGnuPlotLoadFileDialog::
xmin() const
{
  return xminEdit_->value();
}

double
CQGnuPlotLoadFileDialog::
xmax() const
{
  return xmaxEdit_->value();
}

QString
CQGnuPlotLoadFileDialog::
usingStr() const
{
  return usingEdit_->text();
}

int
CQGnuPlotLoadFileDialog::
lineType() const
{
  return lineTypeEdit_->value();
}

void
CQGnuPlotLoadFileDialog::
parametricSlot()
{
}

//------

CQGnuPlotFilename::
CQGnuPlotFilename(QWidget *parent) :
 QFrame(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);

  edit_   = new QLineEdit;
  button_ = new QToolButton;

  button_->setIcon(CQGnuPlotUtil::svgIcon(file_dialog_data, FILE_DIALOG_DATA_LEN));

  layout->addWidget(edit_);
  layout->addWidget(button_);

  connect(button_, SIGNAL(clicked()), this, SLOT(fileSlot()));
}

void
CQGnuPlotFilename::
fileSlot()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    "Open File", "", "Files (*.*)");

  if (! fileName.length())
    return;

  edit_->setText(fileName);
}

QString
CQGnuPlotFilename::
name() const
{
  return edit_->text();
}
