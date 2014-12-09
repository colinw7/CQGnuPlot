#ifndef CQPropertyDelegate_H
#define CQPropertyDelegate_H

#include <QItemDelegate>

class QTreeWidget;
class QTreeWidgetItem;

// Tree Widget Item Delegate class to handle custom editing of tree widget items
class CQPropertyDelegate : public QItemDelegate {
 public:
  CQPropertyDelegate(QTreeWidget *tree);

  // Override to create editor
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  // Override to get content from editor
  void setEditorData(QWidget *editor, const QModelIndex &index) const;

  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;

  QTreeWidgetItem *getModelItem(const QModelIndex &index) const;

  QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const;

  QWidget *createEdit(QWidget *parent, const QString &text) const;

 private:
  QTreeWidget *tree_;
};

#endif
