#ifndef CQGnuPlotLoadFileDialog_H
#define CQGnuPlotLoadFileDialog_H

#include <CQDialog.h>
#include <CGnuPlotTypes.h>
#include <CQGnuPlotEnum.h>
#include <QFrame>

class QLineEdit;
class QToolButton;
class CQIntegerSpin;
class CQRealSpin;

class CQGnuPlotFilename : public QFrame {
  Q_OBJECT

 public:
  CQGnuPlotFilename(QWidget *parent=0);

  QString name() const;

 private slots:
  void fileSlot();

 private:
  QLineEdit   *edit_;
  QToolButton *button_;
};

//----

class CQGnuPlotLoadFileDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotLoadFileDialog(QWidget *parent=0);

  void createWidgets(QWidget *frame) override;

  bool is2D() const;
  bool isBinary() const;

  CGnuPlotTypes::PlotStyle plotStyle() const;

  QString fileName() const;

  double xmin() const;
  double xmax() const;

  QString usingStr() const;

  int lineType() const;

 private slots:
  void dimensionSlot();
  void binarySlot();

 private:
  CQGnuPlotEnum*     enum_           { 0 };
  QGroupBox*         dimensionGroup_ { 0 };
  QCheckBox*         binaryCheck_    { 0 };
  CQGnuPlotFilename* file_           { 0 };
  CQRealSpin*        xminEdit_       { 0 };
  CQRealSpin*        xmaxEdit_       { 0 };
  QLineEdit*         usingEdit_      { 0 };
  CQIntegerSpin*     lineTypeEdit_   { 0 };
};

#endif
