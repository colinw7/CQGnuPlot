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
  CQGnuPlotLoadFunctionDialog(QWidget *parent=0);

  void createWidgets(QWidget *frame) override;

  bool is2D() const;

  CGnuPlotTypes::PlotStyle plotStyle() const;

  QStringList functions() const;

  int samples() const;

  double xmin() const;
  double xmax() const;

  int lineType() const;

 private slots:
  void dimensionSlot();

 private:
  CQGnuPlotEnum* enum_           { 0 };
  QGroupBox*     dimensionGroup_ { 0 };
  QLineEdit*     fnEdit_         { 0 };
  CQIntegerSpin* samplesEdit_    { 0 };
  CQRealSpin*    xminEdit_       { 0 };
  CQRealSpin*    xmaxEdit_       { 0 };
  CQIntegerSpin* lineTypeEdit_   { 0 };
};

#endif
