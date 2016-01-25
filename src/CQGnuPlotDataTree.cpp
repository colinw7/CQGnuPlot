#include <CQGnuPlotDataTree.h>
#include <CQGnuPlotDataModel.h>
#include <CQHeaderView.h>
#include <QScrollBar>
#include <QCheckBox>

class CQGnuPlotDataTreeWidgetFactory : public CQHeaderViewWidgetFactory {
 public:
  CQGnuPlotDataTreeWidgetFactory(CQGnuPlotDataTree *tree) :
   tree_(tree) {
  }

  QWidget *createWidget(int i);

 private:
  CQGnuPlotDataTree *tree_;
};

CQGnuPlotDataTree::
CQGnuPlotDataTree(const CGnuPlotFile &file) :
 QTreeView()
{
  setObjectName("dataTree");

  model_ = new CQGnuPlotDataModel(file);

  model_->setWidgetHeaders(true);

  header_ = new CQHeaderView;

  header_->setWidgetFactory(new CQGnuPlotDataTreeWidgetFactory(this));

  setModel(model_);

  setHeader(header_);

  connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrolled()));
  connect(verticalScrollBar  (), SIGNAL(valueChanged(int)), this, SLOT(scrolled()));
}

void
CQGnuPlotDataTree::
reload()
{
  model_->reload();

  header_->initWidgets();
}

void
CQGnuPlotDataTree::
scrolled()
{
  header_->fixWidgetPositions();
}

//------

QWidget *
CQGnuPlotDataTreeWidgetFactory::
createWidget(int i)
{
  QCheckBox *cb = new QCheckBox;

  QVariant var = tree_->model()->headerData(i, Qt::Horizontal, Qt::EditRole);

  QString str = var.toString();

  if (str == "")
    str = QString("Col %1").arg(i);

  cb->setText(str);

  cb->setChecked(true);

  return cb;
}
