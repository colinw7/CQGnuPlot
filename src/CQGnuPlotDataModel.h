#ifndef CQGnuPlotDataModel_H
#define CQGnuPlotDataModel_H

#include <CGnuPlotFile.h>
#include <QAbstractItemModel>

class CQGnuPlotDataModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  struct Row {
    Row(int iset=-1, int isubSet=-1) :
     set(iset), subSet(isubSet) {
    }

    int set    { -1 };
    int subSet { -1 };
  };

 public:
  CQGnuPlotDataModel(const CGnuPlotFile &file);
 ~CQGnuPlotDataModel();

  const CGnuPlotFile &file() const { return file_; }
  void setFile(const CGnuPlotFile &file);

  bool isWidgetHeaders() const { return widgetHeaders_; }
  void setWidgetHeaders(bool b) { widgetHeaders_ = b; }

  void reload();

  bool isHierarchical1() const;
  bool isHierarchical2() const;

  int columnCount(const QModelIndex &parent=QModelIndex()) const;

  QVariant headerData(int section, Qt::Orientation orientation=Qt::Horizontal,
                      int role=Qt::DisplayRole) const;

  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

  QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;

  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent = QModelIndex()) const;

 private:
  void init();

 private:
  typedef std::vector<Row>  Rows;
  typedef std::vector<Rows> SetRows;

  CGnuPlotFile file_;
  bool         widgetHeaders_ { false };
  Rows         rows1_;
  SetRows      rows2_;
};

#endif
