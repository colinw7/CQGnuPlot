#include <CQGnuPlotTreeWidget.h>
#include <QHeaderView>
#include <QMenu>

CQGnuPlotTreeWidget::
CQGnuPlotTreeWidget(QWidget *parent) :
 QTreeWidget(parent)
{
  setObjectName("tree");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  setAlternatingRowColors(true);

  setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(customContextMenuSlot(const QPoint&)));
}

void
CQGnuPlotTreeWidget::
customContextMenuSlot(const QPoint &pos)
{
  //QTreeWidgetItem *item = itemAt(pos);

  QPoint gpos = viewport()->mapToGlobal(pos);

  QMenu menu;

  menu.addAction("Fit Columns", this, SLOT(fitSlot()));

  menu.exec(gpos);
}

void
CQGnuPlotTreeWidget::
fitSlot()
{
  QFontMetrics fm(font());

  int nc = columnCount();

  ColumnWidths columnWidths;

  QTreeWidgetItem *item = headerItem();

  for (int c = 0; c < nc - 1; ++c) {
    QString str = item->text(c);

    columnWidths[c] = qMax(columnWidths[c], fm.width(str) + 10);
  }

  int nr = topLevelItemCount();

  for (int r = 0; r < nr; ++r) {
    QTreeWidgetItem *item = topLevelItem(r);

    updateItemWidths(item, columnWidths);
  }

  for (int c = 0; c < nc - 1; ++c)
    setColumnWidth(c, columnWidths[c]);
}

void
CQGnuPlotTreeWidget::
updateItemWidths(QTreeWidgetItem *item, ColumnWidths &columnWidths)
{
  QFontMetrics fm(font());

  int nc = columnCount();

  for (int c = 0; c < nc - 1; ++c) {
    QString str = item->text(c);

    columnWidths[c] = qMax(columnWidths[c], fm.width(str) + 28);
  }

  int nr = item->childCount();

  for (int r = 0; r < nr; ++r) {
    QTreeWidgetItem *item1 = item->child(r);

    updateItemWidths(item1, columnWidths);
  }
}
