#ifndef CQGnuPlotDataDialog_H
#define CQGnuPlotDataDialog_H

#include <QFrame>

class CQGnuPlotMainWindow;
class CGnuPlotFile;
class CQGnuPlotDataTree;
class CQGnuPlotEnum;
class CQIntegerSpin;
class QLineEdit;
class QRadioButton;

class CQGnuPlotDataDialog : public QFrame {
  Q_OBJECT

 public:
  CQGnuPlotDataDialog(CQGnuPlotMainWindow *window, const CGnuPlotFile &file);

 private slots:
  void generateSlot();
  void filterSlot();
  void plotSlot();
  void closeSlot();

 private:
  CQGnuPlotMainWindow* window_           { 0 };
  CQGnuPlotDataTree*   tree_             { 0 };
  CQIntegerSpin*       numberEdit_       { 0 };
  QLineEdit*           filterEdit_       { 0 };
  CQGnuPlotEnum*       enum_             { 0 };
  QRadioButton*        dimension2DRadio_ { 0 };
  QRadioButton*        dimension3DRadio_ { 0 };
};

#endif
