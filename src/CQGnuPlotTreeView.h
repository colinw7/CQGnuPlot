#ifndef CQGnuPlotTreeView_H
#define CQGnuPlotTreeView_H

#include <QTreeView>

class CQGnuPlotTreeView : public QTreeView {
  Q_OBJECT

 public:
  typedef std::map<int,int> ColumnWidths;

 public:
  CQGnuPlotTreeView(QWidget *parent=0);

 public slots:
  void fitSlot();

  void customContextMenuSlot(const QPoint &pos);

 private:
  void updateItemWidths(const QModelIndex &ind, ColumnWidths &columnWidths);
};

#endif
