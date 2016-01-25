#include <CQGnuPlotLoadFunctionDialog.h>
#include <CQGnuPlotUtil.h>
#include <CQEnumCombo.h>
#include <CQIntegerSpin.h>
#include <CQRealSpin.h>
#include <CParseLine.h>

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
  setWindowTitle("Load Function");

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

  parametricCheck_ = addCheckBox("Parametric", this, SLOT(parametricSlot()));

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

bool
CQGnuPlotLoadFunctionDialog::
isParametric() const
{
  return parametricCheck_->isChecked();
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
  QStringList fns;

  CParseLine line(fnEdit_->text().toStdString());

  int pos = line.pos();

  while (line.isValid()) {
    if (line.skipSpaceAndChar('(')) {
      line.skipChar();

      int brackets = 1;

      while (line.isValid()) {
        if      (line.isChar('('))
          ++brackets;
        else if (line.isChar(')')) {
          --brackets;

          if (brackets <= 0)
            break;
        }
        else if (line.isChar('"') || line.isChar('\'')) {
          CGnuPlot::skipString(line);
        }
        else
          line.skipChar();
      }
    }
    else if (line.skipSpaceAndChar(',')) {
      std::string str = line.substr(pos, line.pos() - pos - 1);

      str = CStrUtil::stripSpaces(str);

      if (str.size())
        fns << str.c_str();

      pos = line.pos();
    }
    else
      line.skipChar();
  }

  std::string str = line.substr(pos, line.pos() - pos);

  str = CStrUtil::stripSpaces(str);

  fns << str.c_str();

  //return fnEdit_->text().split(",", QString::SkipEmptyParts);
  return fns;
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

void
CQGnuPlotLoadFunctionDialog::
parametricSlot()
{
}
