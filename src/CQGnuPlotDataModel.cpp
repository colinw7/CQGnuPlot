#include <CQGnuPlotDataModel.h>

CQGnuPlotDataModel::
CQGnuPlotDataModel(const CGnuPlotFile &file) :
 file_(file)
{
  init();
}

CQGnuPlotDataModel::
~CQGnuPlotDataModel()
{
}

void
CQGnuPlotDataModel::
setFile(const CGnuPlotFile &file)
{
  file_ = file;

  init();
}

void
CQGnuPlotDataModel::
init()
{
  rows1_.clear();
  rows2_.clear();

  if      (isHierarchical1()) {
    int ns = file_.numSubSets(0);

    for (int subSet = 0; subSet < ns; ++subSet)
      rows1_.push_back(Row(-1, subSet));
  }
  else if (isHierarchical2()) {
    int ns = file_.numSets();

    for (int set = 0; set < ns; ++set) {
      rows1_.push_back(Row(set, -1));
      rows2_.push_back(Rows());

      int ns1 = file_.numSubSets(set);

      for (int subSet = 0; subSet < ns1; ++subSet)
        rows2_[set].push_back(Row(set, subSet));
    }
  }
}

void
CQGnuPlotDataModel::
reload()
{
#if QT_VERSION < 0x050000
  QAbstractItemModel::reset();
#else
  QAbstractItemModel::beginResetModel();
  QAbstractItemModel::endResetModel();
#endif
}

int
CQGnuPlotDataModel::
columnCount(const QModelIndex &) const
{
  return file_.numFields(0, 0, 0);
}

QVariant
CQGnuPlotDataModel::
headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
  if (section < 0 || section >= file_.numColumnHeaders())
    return QVariant();

  if (role == Qt::DisplayRole) {
    if (! isWidgetHeaders())
      return QVariant(file_.columnHeader(section).c_str());
    else
      return QVariant();
  }
  else if (role == Qt::EditRole) {
    if (isWidgetHeaders())
      return QVariant(file_.columnHeader(section).c_str());
    else
      return QVariant();
  }
  else
    return QVariant();
}

QVariant
CQGnuPlotDataModel::
data(const QModelIndex &index, int role) const
{
  if (! index.isValid())
    return QVariant();

  if      (! isHierarchical1() && ! isHierarchical2()) {
    if (role == Qt::DisplayRole)
      return QString(file_.field(0, 0, index.row(), index.column()).str().c_str());
    else
      return QVariant();
  }
  else if (isHierarchical1()) {
    int set = 0;

    QModelIndex parent = index.parent();

    if (parent.isValid()) {
      int subSet = parent.row();

      if (role == Qt::DisplayRole) {
        Row *row = static_cast<Row *>(index.internalPointer());

        assert(row->subSet == subSet);

        return QString(file_.field(0, subSet, index.row(), index.column()).str().c_str());
      }
      else
        return QVariant();
    }
    else {
      if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
          int nl = file_.numLines(set, index.row());

          return QString("SubSet %1 (%2 Lines)").arg(index.row()).arg(nl);
        }
        else
          return QVariant();
      }
      else
        return QVariant();
    }
  }
  else {
    QModelIndex parent = index.parent();

    if (parent.isValid()) {
      int set = parent.row();

      QModelIndex parent1 = parent.parent();

      if (parent1.isValid()) {
        int subSet = set;

        set = parent1.row();

        if (role == Qt::DisplayRole)
          return QString(file_.field(set, subSet, index.row(), index.column()).str().c_str());
        else
          return QVariant();
      }
      else {
        if (role == Qt::DisplayRole) {
          if (index.column() == 0) {
            int nl = file_.numLines(set, index.row());

            return QString("SubSet %1 (%2 Lines)").arg(index.row()).arg(nl);
          }
          else
            return QVariant();
        }
        else
          return QVariant();
      }
    }
    else {
      if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
          int ns = file_.numSubSets(index.row());

          return QString("Set %1 (%2 SubSets)").arg(index.row()).arg(ns);
        }
        else
          return QVariant();
      }
      else
        return QVariant();
    }
  }
}

QModelIndex
CQGnuPlotDataModel::
index(int row, int column, const QModelIndex &parent) const
{
  if      (! isHierarchical1() && ! isHierarchical2()) {
    return createIndex(row, column, nullptr);
  }
  else if (isHierarchical1()) {
    if (parent.isValid()) {
      int subSet = parent.row();

      assert(subSet >= 0 && subSet < int(rows1_.size()));

      return createIndex(row, column, (void *) &rows1_[subSet]);
    }
    else
      return createIndex(row, column, nullptr);
  }
  else {
    if (parent.isValid()) {
      int set = parent.row();

      QModelIndex parent1 = parent.parent();

      if (parent1.isValid()) {
        int subSet = set;

        set = parent1.row();

        assert(set >= 0 && set < int(rows2_.size()));
        assert(subSet >= 0 && subSet < int(rows2_[set].size()));

        return createIndex(row, column, (void *) &rows2_[set][subSet]);
      }
      else {
        assert(set >= 0 && set < int(rows1_.size()));

        return createIndex(row, column, (void *) &rows1_[set]);
      }
    }
    else
      return createIndex(row, column, nullptr);
  }
}

QModelIndex
CQGnuPlotDataModel::
parent(const QModelIndex &index) const
{
  if (! index.isValid())
    return QModelIndex();

  if      (! isHierarchical1() && ! isHierarchical2()) {
    return QModelIndex();
  }
  else if (isHierarchical1()) {
    Row *row = static_cast<Row *>(index.internalPointer());

    if (row) {
      return createIndex(row->subSet, 0, nullptr);
    }
    else
      return QModelIndex();
  }
  else {
    Row *row = static_cast<Row *>(index.internalPointer());

    if (row) {
      if (row->subSet < 0)
        return createIndex(row->set, 0, nullptr);
      else {
        assert(row->set >= 0 && row->set < int(rows1_.size()));

        return createIndex(row->subSet, 0, (void *) &rows1_[row->set]);
      }
    }
    else
      return QModelIndex();
  }
}

int
CQGnuPlotDataModel::
rowCount(const QModelIndex &parent) const
{
  if      (! isHierarchical1() && ! isHierarchical2()) {
    if (parent.isValid())
      return 0;

    return file_.numLines(0, 0);
  }
  else if (isHierarchical1()) {
    if (parent.isValid()) {
      QModelIndex parent1 = parent.parent();

      if (parent1.isValid())
        return 0;

      int subSet = parent.row();

      return file_.numLines(0, subSet);
    }

    return file_.numSubSets(0);
  }
  else {
    if (parent.isValid()) {
      int set = parent.row();

      QModelIndex parent1 = parent.parent();

      if (parent1.isValid()) {
        int subSet = set;

        set = parent1.row();

        QModelIndex parent2 = parent1.parent();

        if (parent2.isValid())
          return 0;

        return file_.numLines(set, subSet);
      }

      return file_.numSubSets(set);
    }

    return file_.numSets();
  }
}

bool
CQGnuPlotDataModel::
isHierarchical1() const
{
  if (isHierarchical2()) return false;

  return (file_.numSubSets(0) > 1);
}

bool
CQGnuPlotDataModel::
isHierarchical2() const
{
  return (file_.numSets() > 1);
}
