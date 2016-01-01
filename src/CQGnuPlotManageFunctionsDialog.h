#ifndef CQGnuPlotManageFunctionsDialog_H
#define CQGnuPlotManageFunctionsDialog_H

#include <CQDialog.h>

class QTreeWidget;
class QLineEdit;

class CQGnuPlotManageFunctionsDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotManageFunctionsDialog(QWidget *parent=0);

  void createWidgets(QWidget *frame) override;

  QSize sizeHint() const;

 private slots:
  void addSlot();

  void updateFunctions();

 private:
  QTreeWidget *tree_;
  QLineEdit   *nameEdit_;
  QLineEdit   *exprEdit_;
  QSize        size_;
};

#endif
