#include <CQGnuPlotCreateDialog.h>
#include <CQEnumCombo.h>
#include <CQColorChooser.h>
#include <CQGnuPlotPositionEdit.h>
#include <CQAngleSpinBox.h>
#include <CQIntegerSpin.h>
#include <CQRealSpin.h>
#include <CQLineDash.h>
#include <CQUtil.h>
#include <CQPixmapCache.h>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>

#include <svg/arrow_obj_svg.h>
#include <svg/circle_obj_svg.h>
#include <svg/ellipse_obj_svg.h>
#include <svg/label_obj_svg.h>
#include <svg/polygon_obj_svg.h>
#include <svg/rectangle_obj_svg.h>

CQGnuPlotCreateDialog::
CQGnuPlotCreateDialog(QWidget *parent) :
 CQDialog(parent)
{
  setObjectName("createDialog");

  setWindowTitle("Create Object");

  enum_ = new CQGnuPlotEnum;

  enum_->setObjectType(CQGnuPlotEnum::ObjectTypeArrow);
}

void
CQGnuPlotCreateDialog::
createWidgets(QWidget *)
{
  typeCombo_ = new CQEnumCombo(this, enum_, "objectType");

  typeCombo_->setItemText(int(CQGnuPlotEnum::ObjectTypeNone     ), "None");
  typeCombo_->setItemData(int(CQGnuPlotEnum::ObjectTypeArrow    ),
                          "Arrow"    , CQPixmapCacheInst->getIcon("ARROW_OBJ"));
  typeCombo_->setItemData(int(CQGnuPlotEnum::ObjectTypeCircle   ),
                          "Circle"   , CQPixmapCacheInst->getIcon("CIRCLE_OBJ"));
  typeCombo_->setItemData(int(CQGnuPlotEnum::ObjectTypeEllipse  ),
                          "Ellipse"  , CQPixmapCacheInst->getIcon("ELLIPSE_OBJ"));
  typeCombo_->setItemData(int(CQGnuPlotEnum::ObjectTypeLabel    ),
                          "Label"    , CQPixmapCacheInst->getIcon("LABEL_OBJ"));
  typeCombo_->setItemData(int(CQGnuPlotEnum::ObjectTypePolygon  ),
                          "Polygon"  , CQPixmapCacheInst->getIcon("POLYGON_OBJ"));
  typeCombo_->setItemData(int(CQGnuPlotEnum::ObjectTypeRectangle),
                          "Rectangle", CQPixmapCacheInst->getIcon("RECTANGLE_OBJ"));

  connect(typeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(typeSlot(int)));

  addField("Type", typeCombo_, true);

  dimensionGroup_ = addRadioButtons("Dimension", {"2D", "3D"}, this, SLOT(dimensionSlot()));

  //---

  QGroupBox *strokeGroup = new QGroupBox("Stroke");
  strokeGroup->setObjectName("strokeGroup");

  addField("", strokeGroup);

  QVBoxLayout *strokeLayout = new QVBoxLayout(strokeGroup);
  strokeLayout->setMargin(0); strokeLayout->setSpacing(0);

  CQDialogForm *strokeFrame = new CQDialogForm;
  strokeFrame->setObjectName("strokeForm");

  strokeLayout->addWidget(strokeFrame);

  strokeFrame->addField("Color", (strokeColor_ = new CQColorChooser));
  strokeFrame->addField("Width", (strokeWidth_ = new CQIntegerSpin));
  strokeFrame->addField("Dash" , (strokeDash_  = new CQLineDash));

  strokeColor_->setColor(QColor(0,0,0));
  strokeWidth_->setValue(1);

  strokeColor_->setStyles(CQColorChooser::Text | CQColorChooser::ColorButton);

  //---

  QGroupBox *fillGroup = new QGroupBox("Fill");
  fillGroup->setObjectName("fillGroup");

  addField("", fillGroup);

  QVBoxLayout *fillLayout = new QVBoxLayout(fillGroup);
  fillLayout->setMargin(0); fillLayout->setSpacing(0);

  CQDialogForm *fillFrame = new CQDialogForm;
  fillFrame->setObjectName("fillForm");

  fillLayout->addWidget(fillFrame);

  fillFrame->addField("Color", (fillColor_ = new CQColorChooser));

  fillColor_->setStyles(CQColorChooser::Text | CQColorChooser::ColorButton);

  //---

  stack_ = new QStackedWidget;
  stack_->setObjectName("stack");

  addField("", stack_);

  arrowFrame_   = new CQDialogForm; arrowFrame_  ->setObjectName("arrowForm");
  circleFrame_  = new CQDialogForm; circleFrame_ ->setObjectName("circleForm");
  ellipseFrame_ = new CQDialogForm; ellipseFrame_->setObjectName("ellipseForm");
  labelFrame_   = new CQDialogForm; labelFrame_  ->setObjectName("labelForm");
  polyFrame_    = new CQDialogForm; polyFrame_   ->setObjectName("polyForm");
  rectFrame_    = new CQDialogForm; rectFrame_   ->setObjectName("rectForm");

  stack_->addWidget(arrowFrame_  );
  stack_->addWidget(circleFrame_ );
  stack_->addWidget(ellipseFrame_);
  stack_->addWidget(labelFrame_  );
  stack_->addWidget(polyFrame_   );
  stack_->addWidget(rectFrame_   );

  arrowFrame_->addField("From", (arrowFrom_  = new CQGnuPlotPositionEdit), true);
  arrowFrame_->addField("To"  , (arrowTo_    = new CQGnuPlotPositionEdit), true);
  arrowFrame_->addStretch();

  circleFrame_->addField("Center"     , (circleCenter_ = new CQGnuPlotPositionEdit));
  circleFrame_->addField("Radius"     , (circleRadius_ = new CQRealSpin));
  circleFrame_->addField("Start Angle", (circleStart_  = new CQAngleSpinBox));
  circleFrame_->addField("End Angle"  , (circleEnd_    = new CQAngleSpinBox));
  circleFrame_->addStretch();

  circleRadius_->setValue(1);
  circleStart_ ->setAngle(CAngle(  0));
  circleEnd_   ->setAngle(CAngle(360));

  ellipseFrame_->addField("Center", (ellipseCenter_ = new CQGnuPlotPositionEdit));
  ellipseFrame_->addField("Angle" , (ellipseAngle_  = new CQAngleSpinBox));
  ellipseFrame_->addStretch();

  labelFrame_->addField("Origin", (labelOrigin_ = new CQGnuPlotPositionEdit));
  labelFrame_->addField("Text"  , (labelText_   = new QLineEdit));
  labelFrame_->addField("Angle" , (labelAngle_  = new CQAngleSpinBox));
  labelFrame_->addStretch();

  polyFrame_->addCheckBox("Polygon");

  rectFrame_->addField("From", (rectFrom_  = new CQGnuPlotPositionEdit));
  rectFrame_->addField("To"  , (rectTo_    = new CQGnuPlotPositionEdit));
  rectFrame_->addStretch();

  addStretch();
}

CGnuPlotTypes::ObjectType
CQGnuPlotCreateDialog::
objectType() const
{
  return CQGnuPlotEnum::objectTypeConv(enum_->objectType());
}

bool
CQGnuPlotCreateDialog::
is2D() const
{
  QList<QRadioButton *> buttons = dimensionGroup_->findChildren<QRadioButton *>();

  return buttons[0]->isChecked();
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

void
CQGnuPlotCreateDialog::
dimensionSlot()
{
  bool is2D = this->is2D();

  arrowFrom_->set2D(is2D);
  arrowTo_  ->set2D(is2D);
}

CGnuPlotPosition
CQGnuPlotCreateDialog::
arrowFrom() const
{
  return arrowFrom_->position();
}

CGnuPlotPosition
CQGnuPlotCreateDialog::
arrowTo() const
{
  return arrowTo_->position();
}

CGnuPlotPosition
CQGnuPlotCreateDialog::
circleCenter() const
{
  return circleCenter_->position();
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

CGnuPlotPosition
CQGnuPlotCreateDialog::
ellipseCenter() const
{
  return ellipseCenter_->position();
}

double
CQGnuPlotCreateDialog::
ellipseAngle() const
{
  return ellipseAngle_->getAngle().degrees();
}

CGnuPlotPosition
CQGnuPlotCreateDialog::
labelOrigin() const
{
  return labelOrigin_->position();
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

CGnuPlotPosition
CQGnuPlotCreateDialog::
rectFrom() const
{
  return rectFrom_->position();
}

CGnuPlotPosition
CQGnuPlotCreateDialog::
rectTo() const
{
  return rectTo_->position();
}
