#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPropertyDelegate.h>

#include <QHeaderView>
#include <iostream>

CQPropertyTree::
CQPropertyTree(QWidget *parent) :
 QTreeWidget(parent)
{
  setObjectName("propertyTree");
  setColumnCount(2);

  setHeaderLabels(QStringList() << "Name" << "Value");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

  CQPropertyDelegate *delegate = new CQPropertyDelegate(this);

  setItemDelegate(delegate);

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          this, SLOT(itemClickedSlot(QTreeWidgetItem *, int)));

}

void
CQPropertyTree::
clear()
{
  QTreeWidget::clear();
}

CQPropertyItem *
CQPropertyTree::
addProperty(const QString &path, QObject *object, const QString &name)
{
  QStringList pathParts = path.split('/', QString::SkipEmptyParts);

  QTreeWidgetItem *parent = 0;

  for (int i = 0; i < pathParts.size(); ++i) {
    if (i == 0) {
      QList<QTreeWidgetItem *> items = findItems(pathParts[i], Qt::MatchExactly, 0);

      if (items.empty()) {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << pathParts[i] << "");

        items.push_back(item);

        addTopLevelItem(item);
      }

      parent = items[0];
    }
    else {
      int j  = 0;
      int nc = parent->childCount();

      for ( ; j < nc; ++j) {
        QTreeWidgetItem *child = parent->child(j);

        if (child->text(0) == pathParts[i]) {
          parent = child;
          break;
        }
      }

      if (j >= nc) {
        QTreeWidgetItem *child = new QTreeWidgetItem(parent, QStringList() << pathParts[i] << "");

        parent = child;
      }
    }
  }

  CQPropertyItem *item = new CQPropertyItem(parent, name, "", "", object);

  connect(item, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SIGNAL(valueChanged(QObject *, const QString &)));

  if (! parent)
    addTopLevelItem(item);

  return item;
}

void
CQPropertyTree::
itemClickedSlot(QTreeWidgetItem *item, int /*column*/)
{
  QString path;

  QTreeWidgetItem *item1 = item;

  while (item1) {
    if (path.length())
      path = item1->data(0, Qt::DisplayRole).toString() + "/" + path;
    else
      path = item1->data(0, Qt::DisplayRole).toString();

    item1 = item1->parent();
  }

  item1 = item;

  int n = item1->childCount();

  while (n > 0) {
    item1 = item1->child(0);

    n = item1->childCount();
  }

  QObject *obj = 0;

  if (CQPropertyItem::isType(item1->type())) {
    CQPropertyItem *item2 = static_cast<CQPropertyItem *>(item1);

    obj = item2->getObject();
  }

  emit itemClicked(obj, path);
}
