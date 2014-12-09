#include <CQPropertyDelegate.h>
#include <CQPropertyItem.h>

#include <QApplication>
#include <QTreeWidget>
#include <QLineEdit>

#include <cassert>

/*! create tree widget delegate
*/
CQPropertyDelegate::
CQPropertyDelegate(QTreeWidget *tree) :
 QItemDelegate(tree), tree_(tree)
{
}

/*! create editor for tree widget item
*/
QWidget *
CQPropertyDelegate::
createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  CQPropertyDelegate *th = const_cast<CQPropertyDelegate *>(this);

  QWidget *w = 0;

  if (CQPropertyItem::isType(item->type())) {
    CQPropertyItem *item1 = dynamic_cast<CQPropertyItem *>(item);
    assert(item1);

    if (index.column() == 1)
      w = item1->createEditor(parent);
  }
  else
    assert(false);

  if (w)
    w->installEventFilter(th);

  return w;
}

/*! get data to display in tree widget item
*/
void
CQPropertyDelegate::
setEditorData(QWidget *, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  if (CQPropertyItem::isType(item->type())) {
    CQPropertyItem *item1 = dynamic_cast<CQPropertyItem *>(item);
    assert(item1);

    QString value = index.model()->data(index, Qt::DisplayRole).toString();

    item1->setEditorData(value);
  }
  else
    assert(false);
}

/*! store displayed tree widget item data in model
*/
void
CQPropertyDelegate::
setModelData(QWidget *, QAbstractItemModel *model, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  if (CQPropertyItem::isType(item->type())) {
    CQPropertyItem *item1 = dynamic_cast<CQPropertyItem *>(item);
    assert(item1);

    QString value = item1->getEditorData();

    model->setData(index, value);
  }
  else
    assert(false);
}

/*! update geometry
*/
void
CQPropertyDelegate::
updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                     const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

/*! draw item
*/
void
CQPropertyDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  QItemDelegate::paint(painter, option, index);
}

/*! size hint
*/
QSize
CQPropertyDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QSize size = QItemDelegate::sizeHint(option, index);

  size.setHeight(size.height() + 2);

  return size;
}

/*! get tree widget item from index
*/
QTreeWidgetItem *
CQPropertyDelegate::
getModelItem(const QModelIndex &index) const
{
  QTreeWidgetItem *item;

  if (! index.parent().isValid())
    item = tree_->topLevelItem(index.row());
  else {
    QTreeWidgetItem *parent = getModelItem(index.parent());
    assert(parent);

    item = parent->child(index.row());
  }

  return item;
}

/*! create edit widget
*/
QWidget *
CQPropertyDelegate::
createEdit(QWidget *parent, const QString &text) const
{
  QLineEdit *edit = new QLineEdit(parent);

  edit->setText(text);

  edit->setMouseTracking(false);
  edit->setFocusPolicy(Qt::NoFocus);
  edit->setAcceptDrops(false);
  edit->setFrame(false);
  edit->setReadOnly(true);

  // set background of preview text to window background
  QPalette plt = QApplication::palette();
  QColor bgColor = plt.color(QPalette::Window);
  QString styleStr;
  styleStr.sprintf("background: #%2x%2x%2x", bgColor.red(), bgColor.green(), bgColor.blue());
  edit->setStyleSheet(styleStr);

  return edit;
}
