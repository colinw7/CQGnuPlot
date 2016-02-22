#include <CQEnumCombo.h>
#include <CQUtil.h>
#include <iostream>

CQEnumCombo::
CQEnumCombo(QWidget *parent, const QObject *obj, const QString &name) :
 QComboBox(parent), obj_(obj), name_(name)
{
  init();

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePropertySlot()));
}

CQEnumCombo::
CQEnumCombo(const QObject *obj, const QString &name) :
 QComboBox(), obj_(obj), name_(name)
{
  init();

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePropertySlot()));
}

void
CQEnumCombo::
init()
{
  setObjectName("enumCombo");

  CQUtil::PropInfo propInfo;
  QString          typeName;

  if (! CQUtil::getPropInfo(obj_, name_, &propInfo))
    return;

  if (! propInfo.isEnumType())
    return;

  const QStringList &names = propInfo.enumNames();

  for (int i = 0; i < names.length(); ++i)
    addItem(names[i], QVariant(i));

  QVariant var;

  if (! CQUtil::getProperty(obj_, name_, var))
    var = QVariant(0);

  int ind = var.toInt();

  if (ind >= 0)
    setCurrentIndex(ind);
}

void
CQEnumCombo::
setItemIcon(int i, const QIcon &icon)
{
  QComboBox::setItemIcon(i, icon);
}

void
CQEnumCombo::
setItemText(int i, const QString &text)
{
  QComboBox::setItemText(i, text);
}

void
CQEnumCombo::
setItemData(int i, const QString &text, const QIcon &icon)
{
  QComboBox::setItemText(i, text);
  QComboBox::setItemIcon(i, icon);
}

void
CQEnumCombo::
updatePropertySlot()
{
  CQUtil::PropInfo propInfo;
  QString          typeName;

  if (! CQUtil::getPropInfo(obj_, name_, &propInfo))
    return;

  if (! propInfo.isEnumType())
    return;

  QVariant v(currentIndex());

  if (! CQUtil::setProperty(obj_, name_, v)) {
    std::cerr << "Failed to set property " << name_.toStdString() << std::endl;
  }
}
