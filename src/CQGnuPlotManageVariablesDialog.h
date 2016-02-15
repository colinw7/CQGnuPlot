#ifndef CQGnuPlotManageVariablesDialog_H
#define CQGnuPlotManageVariablesDialog_H

#include <CQDialog.h>
#include <QItemDelegate>
#include <QTreeWidgetItem>

#include <CExpr.h>

class CQGnuPlotManageVariablesDialog;
class CQGnuPlotMainWindow;
class QTreeWidget;

// Tree Widget Item Delegate class to handle custom editing of tree widget items
class CQGnuPlotVariablesDelegate : public QItemDelegate {
  Q_OBJECT

 public:
  CQGnuPlotVariablesDelegate(QTreeWidget *tree);

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

  void drawString(QPainter *painter, const QStyleOptionViewItem &option,
                  const QString &str, const QModelIndex &index) const;

 private:
  QTreeWidget *tree_;
};

//----

class CQGnuPlotVariableItem : public QObject, public QTreeWidgetItem {
  Q_OBJECT

 private:
  //! unique id
  enum { VARIABLE_ITEM_TYPE = QTreeWidgetItem::UserType + 142 };

 public:
  //! type check for casting
  static bool isType(int type) { return type == VARIABLE_ITEM_TYPE; }

  //! constructor
  CQGnuPlotVariableItem(CQGnuPlotManageVariablesDialog *dialog,
                        QTreeWidgetItem *item, CExprVariablePtr var);

  //! copy constructor
  CQGnuPlotVariableItem(const CQGnuPlotVariableItem &item);

  //! clone (required by QTreeWidgetItem interface)
  QTreeWidgetItem *clone() const { return new CQGnuPlotVariableItem(*this); }

  //! handle click
  bool click();

  //! create editor widget (required by QTreeWidgetItem interface)
  QWidget *createEditor(QWidget *parent);

  //! set widget property from string
  void setEditorData(const QString &value);
  //! set widget property from variant
  void setEditorData(const QVariant &value);

  //! get editor widget text
  QVariant getEditorData() const;

  //! paint item
  bool paint(const CQGnuPlotVariablesDelegate *delegate, QPainter *painter,
             const QStyleOptionViewItem &option, const QModelIndex &index);

 signals:
  void valueChanged(const QString &name);

 private slots:
  //! called when the editor widget is changed
  void updateValue();

 private:
  //! private data
  CQGnuPlotManageVariablesDialog* dialog_; //! dialog
  CExprVariablePtr                var_;    //! variable
  CExprValueType                  type_;   //! value type
  QWidget*                        widget_; //! editor widget
};

//----

class CQGnuPlotManageVariablesDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotManageVariablesDialog(CQGnuPlotMainWindow *window);

  CQGnuPlotMainWindow *window() const { return window_; }

  void createWidgets(QWidget *frame) override;

  QSize sizeHint() const;

 private slots:
  void updateVariables();

 private:
  CQGnuPlotMainWindow *window_;
  QTreeWidget         *tree_;
  QSize                size_;
};

#endif
