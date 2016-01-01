#ifndef CQPropertyTree_H
#define CQPropertyTree_H

#include <QTreeWidget>
#include <vector>

class CQPropertyItem;

class CQPropertyTree : public QTreeWidget {
  Q_OBJECT

 public:
  typedef std::vector<QTreeWidgetItem *> Items;

 public:
  CQPropertyTree(QWidget *parent=0);

  void clear();

  CQPropertyItem *addProperty(const QString &path, QObject *object, const QString &name);

  void selectObject(const QObject *obj);

  void getSelectedObjects(std::vector<QObject *> &objs);

 signals:
  void valueChanged(QObject *obj, const QString &name);

  void itemClicked( QObject *obj, const QString &path);
  void itemSelected(QObject *obj, const QString &path);

  void menuExec(QObject *obj, const QPoint &gpos);

 public slots:
  void expandSelected();

  void search(const QString &text);

 private slots:
  void itemClickedSlot(QTreeWidgetItem *item, int column);

  void itemSelectionSlot();

  void customContextMenuSlot(const QPoint &pos);

 private:
  bool selectObject(QTreeWidgetItem *item, const QObject *obj);

  void getItemData(QTreeWidgetItem *item, QObject* &obj, QString &path);

  void searchItemTree(QTreeWidgetItem *item, const QString &text, Items &items);

  void expandItemTree(QTreeWidgetItem *item);

  void showContextMenu(QObject *obj, const QPoint &globalPos);
};

#endif
