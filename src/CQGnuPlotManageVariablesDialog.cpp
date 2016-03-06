#include <CQGnuPlotManageVariablesDialog.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotTreeWidget.h>
#include <CQUtil.h>
#include <CQRealSpin.h>
#include <CQIntegerSpin.h>
#include <CExpr.h>

#include <QApplication>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>

/*! create tree widget delegate
*/
CQGnuPlotVariablesDelegate::
CQGnuPlotVariablesDelegate(CQGnuPlotTreeWidget *tree) :
 QItemDelegate(tree), tree_(tree)
{
}

/*! create editor for tree widget item
*/
QWidget *
CQGnuPlotVariablesDelegate::
createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  CQGnuPlotVariablesDelegate *th = const_cast<CQGnuPlotVariablesDelegate *>(this);

  QWidget *w = 0;

  if (CQGnuPlotVariableItem::isType(item->type())) {
    CQGnuPlotVariableItem *item1 = dynamic_cast<CQGnuPlotVariableItem *>(item);
    assert(item1);

    if (index.column() == 1)
      w = item1->createEditor(parent);
  }
  else
    assert(false);

  if (w) {
    w->updateGeometry();

    if (w->layout())
      w->layout()->invalidate();

    //w->setFixedSize(sizeHint(option, index));

    w->installEventFilter(th);
  }

  return w;
}

/*! get data to display in tree widget item
*/
void
CQGnuPlotVariablesDelegate::
setEditorData(QWidget *, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  if (CQGnuPlotVariableItem::isType(item->type())) {
    CQGnuPlotVariableItem *item1 = dynamic_cast<CQGnuPlotVariableItem *>(item);
    assert(item1);

    QVariant v = index.model()->data(index, Qt::DisplayRole);

    QString value;

    if (! CQUtil::variantToString(v, value))
      std::cerr << "Failed to convert to string" << std::endl;

    item1->setEditorData(value);
  }
  else
    assert(false);
}

/*! store displayed tree widget item data in model
*/
void
CQGnuPlotVariablesDelegate::
setModelData(QWidget *, QAbstractItemModel *model, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  if (CQGnuPlotVariableItem::isType(item->type())) {
    CQGnuPlotVariableItem *item1 = dynamic_cast<CQGnuPlotVariableItem *>(item);
    assert(item1);

    QVariant v = item1->getEditorData();

    model->setData(index, v);
  }
  else
    assert(false);
}

/*! update geometry
*/
void
CQGnuPlotVariablesDelegate::
updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                     const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

/*! draw item
*/
void
CQGnuPlotVariablesDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTreeWidgetItem *item = getModelItem(index);
  assert(item);

  if (CQGnuPlotVariableItem::isType(item->type())) {
    CQGnuPlotVariableItem *item1 = dynamic_cast<CQGnuPlotVariableItem *>(item);
    assert(item1);

    QStyleOptionViewItem option1 = option;

    if (index.column() == 1) {
      if (! item1->paint(this, painter, option1, index))
        QItemDelegate::paint(painter, option1, index);
    }
    else {
      QItemDelegate::paint(painter, option1, index);
    }
  }
  else
    QItemDelegate::paint(painter, option, index);
}

/*! size hint
*/
QSize
CQGnuPlotVariablesDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QSize size = QItemDelegate::sizeHint(option, index);

  size.setHeight(size.height() + 2);

  return size;
}

/*! get tree widget item from index
*/
QTreeWidgetItem *
CQGnuPlotVariablesDelegate::
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
CQGnuPlotVariablesDelegate::
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

void
CQGnuPlotVariablesDelegate::
drawString(QPainter *painter, const QStyleOptionViewItem &option,
           const QString &str, const QModelIndex &index) const
{
  QItemDelegate::drawBackground(painter, option, index);

  QRect rect = option.rect;

  QItemDelegate::drawDisplay(painter, option, rect, str);
}

//------

/*! create custom tree widget item for variable name
*/
CQGnuPlotVariableItem::
CQGnuPlotVariableItem(CQGnuPlotManageVariablesDialog *dialog, QTreeWidgetItem *parent,
                      CExprVariablePtr var) :
 QObject(), QTreeWidgetItem(parent, VARIABLE_ITEM_TYPE), dialog_(dialog), var_(var), widget_(0)
{
  CExprValuePtr value = var_->value();

  type_ = (value.isValid() ? value->getType() : CExprValueType::NONE);

  setText(0, var_->name().c_str());
  setData(1, Qt::EditRole, getEditorData());

  setFlags(flags() | Qt::ItemIsEditable);
}

/*! duplicate custom tree widget item for properties
*/
CQGnuPlotVariableItem::
CQGnuPlotVariableItem(const CQGnuPlotVariableItem &item) :
 QObject(), QTreeWidgetItem(item), dialog_(item.dialog_), var_(item.var_), widget_(0)
{
  CExprValuePtr value = var_->value();

  type_ = (value.isValid() ? value->getType() : CExprValueType::NONE);

  setText(0, var_->name().c_str());
  setData(1, Qt::EditRole, getEditorData());

  setFlags(flags() | Qt::ItemIsEditable);
}

// handle click
bool
CQGnuPlotVariableItem::
click()
{
  return false;
}

/*! get property value
*/
QVariant
CQGnuPlotVariableItem::
getEditorData() const
{
  CExprValuePtr value = var_->value();

  std::string str;

  (void) value->getStringValue(str);

  return QVariant(str.c_str());
}

/*! create editor widget for property
*/
QWidget *
CQGnuPlotVariableItem::
createEditor(QWidget *parent)
{
  CExprValuePtr value = var_->value();

  if      (type_ == CExprValueType::REAL) {
    CQRealSpin *spin = new CQRealSpin(parent);

    double r;

    (void) value->getRealValue(r);

    spin->setValue(r);

    connect(spin, SIGNAL(valueChanged(double)), this, SLOT(updateValue()));

    widget_ = spin;
  }
  else if (type_ == CExprValueType::INTEGER) {
    CQIntegerSpin *spin = new CQIntegerSpin(parent);

    long i;

    (void) value->getIntegerValue(i);

    spin->setValue(i);

    connect(spin, SIGNAL(valueChanged(int)), this, SLOT(updateValue()));

    widget_ = spin;
  }
  else {
    QLineEdit *edit = new QLineEdit(parent);

    std::string str;

    (void) value->getStringValue(str);

    edit->setText(str.c_str());

    connect(edit, SIGNAL(editingFinished()), this, SLOT(updateValue()));

    widget_ = edit;
  }

  return widget_;
}

/*! set property from string
*/
void
CQGnuPlotVariableItem::
setEditorData(const QString &str)
{
  CExpr *expr = dialog_->window()->app()->expr();

  CExprValuePtr value = expr->createStringValue(str.toStdString());

  var_->setValue(value);

  emit valueChanged(var_->name().c_str());
}

/*! set property from variant
*/
void
CQGnuPlotVariableItem::
setEditorData(const QVariant &v)
{
  CExpr *expr = dialog_->window()->app()->expr();

  QString str = v.toString();

  CExprValuePtr value = expr->createStringValue(str.toStdString());

  var_->setValue(value);

  emit valueChanged(var_->name().c_str());
}

/*! update property on widget change
*/
void
CQGnuPlotVariableItem::
updateValue()
{
  if      (type_ == CExprValueType::REAL) {
    CQRealSpin *spin = qobject_cast<CQRealSpin *>(widget_);
    assert(spin);

    double value = spin->value();

    setEditorData(QVariant(value));
  }
  else if (type_ == CExprValueType::INTEGER) {
    CQIntegerSpin *spin = qobject_cast<CQIntegerSpin *>(widget_);
    assert(spin);

    int value = spin->value();

    setEditorData(QVariant(value));
  }
  else {
    QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);
    assert(edit);

    QString value = edit->text();

    setEditorData(QVariant(value));
  }
}

bool
CQGnuPlotVariableItem::
paint(const CQGnuPlotVariablesDelegate *delegate, QPainter *painter,
      const QStyleOptionViewItem &option, const QModelIndex &index)
{
  CExprValuePtr value = var_->value();

  std::string str;

  (void) value->getStringValue(str);

  delegate->drawString(painter, option, str.c_str(), index);

  return true;
}

//------

CQGnuPlotManageVariablesDialog::
CQGnuPlotManageVariablesDialog(CQGnuPlotMainWindow *window) :
 CQDialog(window), window_(window)
{
  setWindowTitle("Manage Variables");
}

void
CQGnuPlotManageVariablesDialog::
createWidgets(QWidget *)
{
  tree_ = new CQGnuPlotTreeWidget;

  tree_->setColumnCount(2);
  tree_->setHeaderLabels(QStringList() << "Name" << "Definition");
  tree_->setSortingEnabled(true);

  tree_->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

  CQGnuPlotVariablesDelegate *delegate = new CQGnuPlotVariablesDelegate(tree_);

  tree_->setItemDelegate(delegate);

  addField("", tree_, /*stretch*/false);

  QPushButton *updateButton = addButton("Update");

  connect(updateButton, SIGNAL(clicked()), this, SLOT(updateVariables()));

  updateVariables();
}

void
CQGnuPlotManageVariablesDialog::
updateVariables()
{
  CExpr *expr = window_->app()->expr();

  tree_->clear();

  CExpr::StringArray names;

  expr->getVariableNames(names);

  QFontMetrics fm(font());

  int nv = 0;
  int w  = 0;

  for (const auto &name : names) {
    CExprVariablePtr var = expr->getVariable(name);

    std::string value;

    CExprValuePtr v = var->value();

    (void) v->getStringValue(value);

    QTreeWidgetItem *item = new CQGnuPlotVariableItem(this, 0, var);

    tree_->addTopLevelItem(item);

    ++nv;

    w = std::max(w, fm.width(name.c_str()) + fm.width(value.c_str() + 32));
  }

  size_ = QSize(w, (nv + 2)*fm.height());
}

QSize
CQGnuPlotManageVariablesDialog::
sizeHint() const
{
  return size_;
}
