#ifndef CQGnuPlotCreateDialog_H
#define CQGnuPlotCreateDialog_H

#include <CQDialog.h>
#include <CQGnuPlotEnum.h>
#include <CGnuPlotPosition.h>
#include <CLineDash.h>

class CQEnumCombo;
class CQColorChooser;
class CQGnuPlotPositionEdit;
class CQPoint2DEdit;
class CQAngleSpinBox;
class CQIntegerSpin;
class CQRealSpin;
class CQLineDash;
class QStackedWidget;
class QLineEdit;

class CQGnuPlotCreateDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotCreateDialog(QWidget *parent=0);

  void createWidgets(QWidget *frame) override;

  CGnuPlotTypes::ObjectType objectType() const;

  bool is2D() const;

  CRGBA     strokeColor() const;
  int       strokeWidth() const;
  CLineDash strokeDash() const;

  CRGBA fillColor() const;

  CGnuPlotPosition arrowFrom() const;
  CGnuPlotPosition arrowTo  () const;

  CPoint2D circleCenter() const;
  double   circleRadius() const;
  double   circleStart () const;
  double   circleEnd   () const;

  CPoint2D ellipseCenter() const;
  double   ellipseAngle () const;

  CPoint2D    labelOrigin() const;
  std::string labelText  () const;
  double      labelAngle () const;

  CPoint2D rectFrom () const;
  CPoint2D rectTo   () const;

 private slots:
  void typeSlot(int);

  void dimensionSlot();

 private:
  CQGnuPlotEnum*         enum_           { 0 };
  CQEnumCombo*           typeCombo_      { 0 };
  QGroupBox*             dimensionGroup_ { 0 };
  CQColorChooser*        strokeColor_    { 0 };
  CQIntegerSpin*         strokeWidth_    { 0 };
  CQLineDash*            strokeDash_     { 0 };
  CQColorChooser*        fillColor_      { 0 };
  QStackedWidget*        stack_          { 0 };
  CQDialogForm*          arrowFrame_     { 0 };
  CQDialogForm*          circleFrame_    { 0 };
  CQDialogForm*          ellipseFrame_   { 0 };
  CQDialogForm*          labelFrame_     { 0 };
  CQDialogForm*          polyFrame_      { 0 };
  CQDialogForm*          rectFrame_      { 0 };
  CQGnuPlotPositionEdit* arrowFrom_      { 0 };
  CQGnuPlotPositionEdit* arrowTo_        { 0 };
  CQPoint2DEdit*         circleCenter_   { 0 };
  CQRealSpin*            circleRadius_   { 0 };
  CQAngleSpinBox*        circleStart_    { 0 };
  CQAngleSpinBox*        circleEnd_      { 0 };
  CQPoint2DEdit*         ellipseCenter_  { 0 };
  CQAngleSpinBox*        ellipseAngle_   { 0 };
  CQPoint2DEdit*         labelOrigin_    { 0 };
  QLineEdit*             labelText_      { 0 };
  CQAngleSpinBox*        labelAngle_     { 0 };
  CQPoint2DEdit*         rectFrom_       { 0 };
  CQPoint2DEdit*         rectTo_         { 0 };
};

#endif
