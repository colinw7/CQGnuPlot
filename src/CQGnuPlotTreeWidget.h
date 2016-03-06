#ifndef CQGnuPlotTreeWidget_H
#define CQGnuPlotTreeWidget_H

#include <QTreeWidget>

class CQGnuPlotTreeWidget : public QTreeWidget {
  Q_OBJECT

 public:
  typedef std::map<int,int> ColumnWidths;

 public:
  CQGnuPlotTreeWidget(QWidget *parent=0);

 public slots:
  void fitSlot();

  void customContextMenuSlot(const QPoint &pos);

 private:
  void updateItemWidths(QTreeWidgetItem *item, ColumnWidths &columnWidths);
};

#endif
