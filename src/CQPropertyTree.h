#include <QTreeWidget>

class CQPropertyItem;

class CQPropertyTree : public QTreeWidget {
  Q_OBJECT

 public:
  CQPropertyTree(QWidget *parent=0);

  void clear();

  CQPropertyItem *addProperty(const QString &path, QObject *object, const QString &name);

  void selectObject(const QObject *obj);

 signals:
  void valueChanged(QObject *obj, const QString &name);

  void itemClicked( QObject *obj, const QString &path);
  void itemSelected(QObject *obj, const QString &path);

 private slots:
  void itemClickedSlot(QTreeWidgetItem *item, int column);

  void itemSelectionSlot();

 private:
  bool selectObject(QTreeWidgetItem *item, const QObject *obj);

  void getItemData(CQPropertyItem *item, QObject* &obj, QString &path);
};
