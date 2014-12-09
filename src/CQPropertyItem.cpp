#include <CQPropertyItem.h>
#include <CQPropertyEditor.h>
#include <CQUtil.h>

#include <QVariant>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>

#include <iostream>

/*! create custom tree widget item for properties
*/
CQPropertyItem::
CQPropertyItem(QTreeWidgetItem *parent, const QString &name, const QString &info,
               const QString &type, QObject *object) :
 QObject(), QTreeWidgetItem(parent, PROPERTY_ITEM_ID), name_(name), info_(info),
 type_(type), object_(object), widget_(0), editor_(0)
{
  setText(0, name_ + info_);
  setData(1, Qt::EditRole, getEditorData());
  //setText(2, type_);

  setFlags(flags() | Qt::ItemIsEditable);
}

/*! duplicate custom tree widget item for properties
*/
CQPropertyItem::
CQPropertyItem(const CQPropertyItem &item) :
 QObject(), QTreeWidgetItem(item), name_(item.name_), info_(item.info_),
 type_(item.type_), object_(item.object_), widget_(0), editor_(0)
{
  setText(0, name_ + info_);
  setData(1, Qt::EditRole, getEditorData());
  //setText(1, type_);

  setFlags(flags() | Qt::ItemIsEditable);
}

/*! get property value
*/
QString
CQPropertyItem::
getEditorData() const
{
  CQUtil::PropInfo propInfo;
  QString          typeName;

  if (CQUtil::getPropInfo(object_, name_, &propInfo))
    typeName = propInfo.typeName();

  QVariant value;
  QString  valueStr;

  if (CQUtil::getProperty(object_, name_, value))
    valueStr = CQUtil::variantToString(value);

  if (propInfo.isEnumType()) {
    int ind = value.toInt();

    const QStringList &names = propInfo.enumNames();

    if (ind >= 0 && ind < names.count())
      valueStr = names[ind];
  }

  return valueStr;
}

/*! create editor widget for property
*/
QWidget *
CQPropertyItem::
createEditor(QWidget *parent)
{
  CQUtil::PropInfo propInfo;
  QString          typeName;

  if (CQUtil::getPropInfo(object_, name_, &propInfo))
    typeName = propInfo.typeName();

  QVariant value;
  QString  valueStr;

  if (CQUtil::getProperty(object_, name_, value))
    valueStr = CQUtil::variantToString(value);

  if (propInfo.isEnumType()) {
    int ind = value.toInt();

    const QStringList &names = propInfo.enumNames();

    if (ind >= 0 && ind < names.count())
      valueStr = names[ind];
  }

  CQPropertyEditorFactory *editor = editor_;

  if (! editor)
    editor = CQPropertyEditorMgr::instance()->getEditor(typeName);

  if      (editor) {
    widget_ = editor->createEdit(parent);

    editor->setValue(widget_, valueStr);

    editor->connect(widget_, this, SLOT(updateValue()));
  }
  // enum - create combobox
  else if (propInfo.isEnumType()) {
    const QStringList &names = propInfo.enumNames();

    QComboBox *combo = new QComboBox(parent);

    combo->addItems(names);
    combo->setCurrentIndex(combo->findText(valueStr));

    connect(combo, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(updateValue()));

    widget_ = combo;
  }
  // bool - create toggle
  else if (typeName == "bool") {
    QCheckBox *check = new QCheckBox(parent);

    QVariant vBool(valueStr);

    vBool.convert(QVariant::Bool);

    check->setChecked(vBool.toBool());

    check->setText(check->isChecked() ? "true" : "false");

    check->setAutoFillBackground(true);
    //check->setLayoutDirection(Qt::RightToLeft);

    connect(check, SIGNAL(stateChanged(int)), this, SLOT(updateValue()));

    widget_ = check;
  }
  // anything else - create line edit
  else {
    QLineEdit *edit = new QLineEdit(parent);

    edit->setText(valueStr);

    connect(edit, SIGNAL(editingFinished()), this, SLOT(updateValue()));

    widget_ = edit;
  }

  return widget_;
}

/*! set property from string
*/
void
CQPropertyItem::
setEditorData(const QString &value)
{
  CQUtil::PropInfo propInfo;

  if (CQUtil::getPropInfo(object_, name_, &propInfo) && propInfo.isWritable()) {
    if (! CQUtil::setProperty(object_, name_, value))
      std::cerr << "Failed to set property " << name_.toStdString() << std::endl;

    emit valueChanged(object_, name_);
  }
}

/*! set property from variant
*/
void
CQPropertyItem::
setEditorData(const QVariant &value)
{
  CQUtil::PropInfo propInfo;

  if (CQUtil::getPropInfo(object_, name_, &propInfo) && propInfo.isWritable()) {
    if (! CQUtil::setProperty(object_, name_, value))
      std::cerr << "Failed to set property " << name_.toStdString() << std::endl;

    emit valueChanged(object_, name_);
  }
}

/*! update property on widget change
*/
void
CQPropertyItem::
updateValue()
{
  QString typeName;

  CQUtil::PropInfo propInfo;

  if (CQUtil::getPropInfo(object_, name_, &propInfo))
    typeName = propInfo.typeName();

  CQPropertyEditorFactory *editor = editor_;

  if (! editor)
    editor = CQPropertyEditorMgr::instance()->getEditor(typeName);

  if      (editor) {
    QVariant value = editor->getValue(widget_);

    setEditorData(value);
  }
  else if (propInfo.isEnumType()) {
    QComboBox *combo = qobject_cast<QComboBox *>(widget_);
    assert(combo);

    QString text = combo->currentText();

    setEditorData(text);
  }
  else if (typeName == "bool") {
    QCheckBox *check = qobject_cast<QCheckBox *>(widget_);
    assert(check);

    check->setText(check->isChecked() ? "true" : "false");

    QString text = (check->isChecked() ? "1" : "0");

    setEditorData(text);
  }
  else {
    QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);
    assert(edit);

    QString text = edit->text();

    setEditorData(text);
  }
}
