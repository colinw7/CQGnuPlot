#include <CQGnuPlotTreeView.h>
#include <CQHeaderView.h>
#include <QMenu>

CQGnuPlotTreeView::
CQGnuPlotTreeView(QWidget *parent) :
 QTreeView(parent)
{
  setObjectName("tree");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  setAlternatingRowColors(true);

  setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(customContextMenuSlot(const QPoint&)));
}

void
CQGnuPlotTreeView::
customContextMenuSlot(const QPoint &pos)
{
  QPoint gpos = viewport()->mapToGlobal(pos);

  QMenu menu;

  menu.addAction("Fit Columns", this, SLOT(fitSlot()));

  menu.exec(gpos);
}

void
CQGnuPlotTreeView::
fitSlot()
{
  QFontMetrics fm(font());

  ColumnWidths columnWidths;

  QHeaderView *header = this->header();

  CQHeaderView *qheader = qobject_cast<CQHeaderView *>(header);

  for (int c = 0; c < header->count(); ++c) {
    QString str = header->model()->headerData(c, Qt::Horizontal).toString();

    columnWidths[c] = qMax(columnWidths[c], fm.horizontalAdvance(str) + 10);

    QWidget *w = (qheader ? qheader->widget(c) : nullptr);

    if (w)
      columnWidths[c] = qMax(columnWidths[c], w->sizeHint().width() + 8);
  }

  int nr = model()->rowCount();

  for (int r = 0; r < nr; ++r) {
    for (int c = 0; c < model()->columnCount(); ++c) {
      QModelIndex ind = model()->index(r, c);

      QString str = model()->data(ind).toString();

      columnWidths[c] = qMax(columnWidths[c], fm.horizontalAdvance(str) + 28);
    }
  }

  for (int r = 0; r < nr; ++r) {
    QModelIndex ind = model()->index(r, 0);

    updateItemWidths(ind, columnWidths);
  }

  for (int c = 0; c < header->count() - 1; ++c)
    setColumnWidth(c, columnWidths[c]);
}

void
CQGnuPlotTreeView::
updateItemWidths(const QModelIndex &ind, ColumnWidths &columnWidths)
{
  QFontMetrics fm(font());

  int nr = model()->rowCount(ind);

  for (int r = 0; r < nr; ++r) {
    for (int c = 0; c < model()->columnCount(ind); ++c) {
      QModelIndex ind1 = model()->index(r, c, ind);

      QString str = model()->data(ind1).toString();

      columnWidths[c] = qMax(columnWidths[c], fm.horizontalAdvance(str) + 28);
    }
  }

  for (int r = 0; r < nr; ++r) {
    QModelIndex ind = model()->index(r, 0, ind);

    updateItemWidths(ind, columnWidths);
  }
}
