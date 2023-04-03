#ifndef CQGnuPlotLoadFunctionDialog_H
#define CQGnuPlotLoadFunctionDialog_H

#include <CQDialog.h>
#include <CGnuPlotTypes.h>
#include <CQGnuPlotEnum.h>

class QLineEdit;
class CQIntegerSpin;
class CQRealSpin;

class CQGnuPlotLoadFunctionDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotLoadFunctionDialog(QWidget *parent=nullptr);

  void createWidgets(QWidget *frame) override;

  bool is2D() const;

  bool isParametric() const;

  CGnuPlotTypes::PlotStyle plotStyle() const;

  QStringList functions() const;

  int samples() const;

  double xmin() const;
  double xmax() const;

  int lineType() const;

  bool isOverlay() const;

 private slots:
  void dimensionSlot();
  void parametricSlot();
  void overlaySlot();

 private:
  CQGnuPlotEnum* enum_            { 0 };
  QGroupBox*     dimensionGroup_  { 0 };
  QCheckBox*     parametricCheck_ { 0 };
  QLineEdit*     fnEdit_          { 0 };
  CQIntegerSpin* samplesEdit_     { 0 };
  CQRealSpin*    xminEdit_        { 0 };
  CQRealSpin*    xmaxEdit_        { 0 };
  CQIntegerSpin* lineTypeEdit_    { 0 };
  QCheckBox*     overlayCheck_    { 0 };
};

#endif
