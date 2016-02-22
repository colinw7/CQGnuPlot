#ifndef CQEnumCombo_H
#define CQEnumCombo_H

#include <QComboBox>

#if 0
#define Q_ENUM_CLASS(ENUM, NAME) \
class CQEnumComboEnum##NAME : public QObject { \
  Q_OBJECT \
\
  Q_ENUMS(ENUM) \
\
  Q_PROPERTY(ENUM value READ value WRITE setValue) \
\
 public: \
  CQEnumComboEnum##NAME() { } \
\
  ENUM value() const { return t_; } \
  void setValue(const ENUM &t) { t_ = t; }\
\
 private: \
  ENUM t_; \
};
#endif

class CQEnumCombo : public QComboBox {
  Q_OBJECT

 public:
  CQEnumCombo(QWidget *parent, const QObject *obj, const QString &name);
  CQEnumCombo(const QObject *obj, const QString &name);

  void setItemIcon(int i, const QIcon &icon);
  void setItemText(int i, const QString &text);

  void setItemData(int i, const QString &text, const QIcon &icon);

 private:
  void init();

 private slots:
  void updatePropertySlot();

 private:
  const QObject *obj_;
  QString        name_;
};

#endif
