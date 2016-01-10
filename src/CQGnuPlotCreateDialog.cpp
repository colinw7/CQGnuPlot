#include <CQGnuPlotCreateDialog.h>
#include <CQEnumCombo.h>
#include <CQColorChooser.h>
#include <CQPoint2DEdit.h>
#include <CQAngleSpinBox.h>
#include <CQIntegerSpin.h>
#include <CQRealSpin.h>
#include <CQLineDash.h>
#include <CQUtil.h>
#include <QStackedWidget>
#include <QGroupBox>

CQGnuPlotCreateDialog::
CQGnuPlotCreateDialog(QWidget *parent) :
 CQDialog(parent)
{
  enum_ = new CQGnuPlotEnum;

  enum_->setObjectType(CQGnuPlotEnum::ObjectTypeArrow);
}

void
CQGnuPlotCreateDialog::
createWidgets(QWidget *)
{
  typeCombo_ = new CQEnumCombo(this, enum_, "objectType");

  connect(typeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(typeSlot(int)));

  addField("Type", typeCombo_);

  //---

  QGroupBox *strokeGroup = new QGroupBox("Stroke");

  addField("", strokeGroup);

  QVBoxLayout *strokeLayout = new QVBoxLayout(strokeGroup);
  strokeLayout->setMargin(0); strokeLayout->setSpacing(0);

  CQDialogForm *strokeFrame = new CQDialogForm;
  strokeLayout->addWidget(strokeFrame);

  strokeFrame->addField("Color", (strokeColor_ = new CQColorChooser));
  strokeFrame->addField("Width", (strokeWidth_ = new CQIntegerSpin));
  strokeFrame->addField("Dash" , (strokeDash_  = new CQLineDash));

  strokeColor_->setColor(QColor(0,0,0));
  strokeWidth_->setValue(1);

  strokeColor_->setStyles(CQColorChooser::Text | CQColorChooser::ColorButton);

  //---

  QGroupBox *fillGroup = new QGroupBox("Fill");

  addField("", fillGroup);

  QVBoxLayout *fillLayout = new QVBoxLayout(fillGroup);
  fillLayout->setMargin(0); fillLayout->setSpacing(0);

  CQDialogForm *fillFrame = new CQDialogForm;
  fillLayout->addWidget(fillFrame);

  fillFrame->addField("Color", (fillColor_ = new CQColorChooser));

  fillColor_->setStyles(CQColorChooser::Text | CQColorChooser::ColorButton);

  //---

  stack_ = new QStackedWidget;

  addField("", stack_);

  arrowFrame_   = new CQDialogForm;
  circleFrame_  = new CQDialogForm;
  ellipseFrame_ = new CQDialogForm;
  labelFrame_   = new CQDialogForm;
  polyFrame_    = new CQDialogForm;
  rectFrame_    = new CQDialogForm;

  stack_->addWidget(arrowFrame_  );
  stack_->addWidget(circleFrame_ );
  stack_->addWidget(ellipseFrame_);
  stack_->addWidget(labelFrame_  );
  stack_->addWidget(polyFrame_   );
  stack_->addWidget(rectFrame_   );

  arrowFrame_->addField("From", (arrowFrom_  = new CQPoint2DEdit));
  arrowFrame_->addField("To"  , (arrowTo_    = new CQPoint2DEdit));
  arrowFrame_->addStretch();

  circleFrame_->addField("Center"     , (circleCenter_ = new CQPoint2DEdit));
  circleFrame_->addField("Radius"     , (circleRadius_ = new CQRealSpin));
  circleFrame_->addField("Start Angle", (circleStart_  = new CQAngleSpinBox));
  circleFrame_->addField("End Angle"  , (circleEnd_    = new CQAngleSpinBox));
  circleFrame_->addStretch();

  circleRadius_->setValue(1);
  circleStart_ ->setAngle(CAngle(  0));
  circleEnd_   ->setAngle(CAngle(360));

  ellipseFrame_->addField("Center", (ellipseCenter_ = new CQPoint2DEdit));
  ellipseFrame_->addField("Angle" , (ellipseAngle_  = new CQAngleSpinBox));
  ellipseFrame_->addStretch();

  labelFrame_->addField("Origin", (labelOrigin_ = new CQPoint2DEdit));
  labelFrame_->addField("Text"  , (labelText_   = new QLineEdit));
  labelFrame_->addField("Angle" , (labelAngle_  = new CQAngleSpinBox));
  labelFrame_->addStretch();

  polyFrame_->addCheckBox("Polygon");

  rectFrame_->addField("From", (rectFrom_  = new CQPoint2DEdit));
  rectFrame_->addField("To"  , (rectTo_    = new CQPoint2DEdit));
  rectFrame_->addStretch();

}

CGnuPlotTypes::ObjectType
CQGnuPlotCreateDialog::
objectType() const
{
  return CQGnuPlotEnum::objectTypeConv(enum_->objectType());
}

CRGBA
CQGnuPlotCreateDialog::
strokeColor() const
{
  QColor c = strokeColor_->color();

  return CQUtil::colorToRGBA(c);
}

int
CQGnuPlotCreateDialog::
strokeWidth() const
{
  return strokeWidth_->value();
}

CLineDash
CQGnuPlotCreateDialog::
strokeDash() const
{
  return strokeDash_->getLineDash();
}

CRGBA
CQGnuPlotCreateDialog::
fillColor() const
{
  QColor c = fillColor_->color();

  return CQUtil::colorToRGBA(c);
}

void
CQGnuPlotCreateDialog::
typeSlot(int ind)
{
  if (ind > 0 && ind <= stack_->count())
    stack_->setCurrentIndex(ind - 1);
}

CPoint2D
CQGnuPlotCreateDialog::
arrowFrom() const
{
  return arrowFrom_->getValue();
}

CPoint2D
CQGnuPlotCreateDialog::
arrowTo() const
{
  return arrowTo_->getValue();
}

CPoint2D
CQGnuPlotCreateDialog::
circleCenter() const
{
  return circleCenter_->getValue();
}

double
CQGnuPlotCreateDialog::
circleRadius() const
{
  return circleRadius_->value();
}

double
CQGnuPlotCreateDialog::
circleStart() const
{
  return circleStart_->getAngle().degrees();
}

double
CQGnuPlotCreateDialog::
circleEnd() const
{
  return circleEnd_->getAngle().degrees();
}

CPoint2D
CQGnuPlotCreateDialog::
ellipseCenter() const
{
  return ellipseCenter_->getValue();
}

double
CQGnuPlotCreateDialog::
ellipseAngle() const
{
  return ellipseAngle_->getAngle().degrees();
}

CPoint2D
CQGnuPlotCreateDialog::
labelOrigin() const
{
  return labelOrigin_->getValue();
}

std::string
CQGnuPlotCreateDialog::
labelText() const
{
  return labelText_->text().toStdString();
}

double
CQGnuPlotCreateDialog::
labelAngle() const
{
  return labelAngle_->getAngle().degrees();
}

CPoint2D
CQGnuPlotCreateDialog::
rectFrom() const
{
  return rectFrom_->getValue();
}

CPoint2D
CQGnuPlotCreateDialog::
rectTo() const
{
  return rectTo_->getValue();
}
