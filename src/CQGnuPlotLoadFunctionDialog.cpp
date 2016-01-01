#include <CQGnuPlotLoadFunctionDialog.h>
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

CQGnuPlotLoadFunctionDialog::
CQGnuPlotLoadFunctionDialog(QWidget *parent) :
 CQDialog(parent)
{
  enum_ = new CQGnuPlotEnum;

  // app()->functionStyle
  enum_->setPlotStyle(CQGnuPlotEnum::PlotLines);
}

void
CQGnuPlotLoadFunctionDialog::
createWidgets(QWidget *)
{
  addField("Style", new CQEnumCombo(this, enum_, "plotStyle"));

  dimensionGroup_ = addRadioButtons("Dimension", {"2D", "3D"}, this, SLOT(dimensionSlot()));

  addField("Function", (fnEdit_ = new QLineEdit));

  addField("Samples", (samplesEdit_ = new CQIntegerSpin));

  addField("XMin", (xminEdit_ = new CQRealSpin));
  addField("XMax", (xmaxEdit_ = new CQRealSpin));

  addField("Line Type", (lineTypeEdit_ = new CQIntegerSpin));

  samplesEdit_ ->setValue(100);
  xminEdit_    ->setValue(-10);
  xmaxEdit_    ->setValue(10);
  lineTypeEdit_->setValue(1);
}

bool
CQGnuPlotLoadFunctionDialog::
is2D() const
{
  QList<QRadioButton *> buttons = dimensionGroup_->findChildren<QRadioButton *>();

  return buttons[0]->isChecked();
}

CGnuPlotTypes::PlotStyle
CQGnuPlotLoadFunctionDialog::
plotStyle() const
{
  return CQGnuPlotEnum::plotStyleConv(enum_->plotStyle());
}

QStringList
CQGnuPlotLoadFunctionDialog::
functions() const
{
  return fnEdit_->text().split(",", QString::SkipEmptyParts);
}

int
CQGnuPlotLoadFunctionDialog::
samples() const
{
  return samplesEdit_->value();
}

double
CQGnuPlotLoadFunctionDialog::
xmin() const
{
  return xminEdit_->value();
}

double
CQGnuPlotLoadFunctionDialog::
xmax() const
{
  return xmaxEdit_->value();
}

int
CQGnuPlotLoadFunctionDialog::
lineType() const
{
  return lineTypeEdit_->value();
}

void
CQGnuPlotLoadFunctionDialog::
dimensionSlot()
{
}
