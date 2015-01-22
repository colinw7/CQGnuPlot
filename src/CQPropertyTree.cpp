#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPropertyDelegate.h>

#include <QHeaderView>

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
