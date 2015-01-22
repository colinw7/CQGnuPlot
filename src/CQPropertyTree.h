#include <QTreeWidget>

class CQPropertyItem;

class CQPropertyTree : public QTreeWidget {
  Q_OBJECT

 public:
  CQPropertyTree(QWidget *parent=0);

  void clear();

  CQPropertyItem *addProperty(const QString &path, QObject *object, const QString &name);

 signals:
  void valueChanged(QObject *obj, const QString &name);
};
