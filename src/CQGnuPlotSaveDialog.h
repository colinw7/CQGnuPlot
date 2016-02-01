#ifndef CQGnuPlotSaveDialog_H
#define CQGnuPlotSaveDialog_H

#include <CQDialog.h>

class CQGnuPlotFilename;
class CQIntegerSpin;

class CQGnuPlotSaveDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotSaveDialog(QWidget *parent=0);

  void createWidgets(QWidget *frame) override;

  bool isPNG() const;
  bool isSVG() const;

  QString fileName() const;

  int width () const;
  int height() const;

 private slots:
  void typeSlot();

 private:
  QGroupBox*         typeGroup_  { 0 };
  CQGnuPlotFilename* file_       { 0 };
  CQIntegerSpin*     widthEdit_  { 0 };
  CQIntegerSpin*     heightEdit_ { 0 };
};

#endif
