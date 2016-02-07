#ifndef CQGnuPlotDataTree_H
#define CQGnuPlotDataTree_H

#include <QTreeView>

class CGnuPlotFile;
class CQGnuPlotDataModel;
class CQHeaderView;

class CQGnuPlotDataTree : public QTreeView {
  Q_OBJECT

 public:
  CQGnuPlotDataTree(const CGnuPlotFile &file);

  CQGnuPlotDataModel *model() const { return model_; }

  CQHeaderView *header() const { return header_; }

  void load(const CGnuPlotFile &file);

 public slots:
  void reload();

 private slots:
  void scrolled();

 private:
  CQGnuPlotDataModel *model_;
  CQHeaderView       *header_;
};

#endif
