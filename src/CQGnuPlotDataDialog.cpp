#include <CQGnuPlotDataDialog.h>
#include <CQGnuPlotDataTree.h>
#include <CQGnuPlotDataModel.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CQHeaderView.h>
#include <CQGnuPlotEnum.h>
#include <CQEnumCombo.h>
#include <CQIntegerSpin.h>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>

#include <set>

CQGnuPlotDataDialog::
CQGnuPlotDataDialog(CQGnuPlotMainWindow *window, const CGnuPlotFile &file) :
 window_(window)
{
  setWindowTitle("Data Dialog");

  setObjectName("dataDialog");

  enum_ = new CQGnuPlotEnum;

  enum_->setPlotStyle(CQGnuPlotEnum::PlotLinesPoints);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  //------

  QGroupBox *generateGroup = new QGroupBox("Generate");

  QVBoxLayout *generateLayout = new QVBoxLayout(generateGroup);
  generateLayout->setMargin(2); generateLayout->setSpacing(2);

  layout->addWidget(generateGroup);

  //--

  QHBoxLayout *numberLayout = new QHBoxLayout;
  numberLayout->setMargin(2); numberLayout->setSpacing(2);

  numberEdit_ = new CQIntegerSpin;

  QPushButton *generateButton = new QPushButton("Generate");

  connect(generateButton, SIGNAL(clicked()), this, SLOT(generateSlot()));

  numberLayout->addWidget(new QLabel("Num"));
  numberLayout->addWidget(numberEdit_);
  numberLayout->addWidget(generateButton);
  numberLayout->addStretch(1);

  generateLayout->addLayout(numberLayout);

  //------

  tree_ = new CQGnuPlotDataTree(file);

  layout->addWidget(tree_);

  //------

  QGroupBox *filterGroup = new QGroupBox("Filter");

  QVBoxLayout *filterLayout = new QVBoxLayout(filterGroup);
  filterLayout->setMargin(2); filterLayout->setSpacing(2);

  layout->addWidget(filterGroup);

  //--

  QHBoxLayout *usingLayout = new QHBoxLayout;
  usingLayout->setMargin(2); usingLayout->setSpacing(2);

  filterEdit_ = new QLineEdit;

  usingLayout->addWidget(new QLabel("Using"));
  usingLayout->addWidget(filterEdit_);

  filterLayout->addLayout(usingLayout);

  //--

  QHBoxLayout *filterButtonsLayout = new QHBoxLayout;
  filterButtonsLayout->setMargin(2); filterButtonsLayout->setSpacing(2);

  QPushButton *filterButton = new QPushButton("Filter");

  connect(filterButton, SIGNAL(clicked()), this, SLOT(filterSlot()));

  filterButtonsLayout->addWidget(filterButton);
  filterButtonsLayout->addStretch(1);

  filterLayout->addLayout(filterButtonsLayout);

  //------

  QGroupBox *plotGroup = new QGroupBox("Plot");

  QVBoxLayout *plotPlayout = new QVBoxLayout(plotGroup);
  plotPlayout->setMargin(2); plotPlayout->setSpacing(2);

  layout->addWidget(plotGroup);

  //--

  QHBoxLayout *styleLayout = new QHBoxLayout;
  styleLayout->setMargin(2); styleLayout->setSpacing(2);

  CQEnumCombo *styleCombo = new CQEnumCombo(this, enum_, "plotStyle");

  styleLayout->addWidget(new QLabel("Style"));
  styleLayout->addWidget(styleCombo);
  styleLayout->addStretch(1);

  plotPlayout->addLayout(styleLayout);

  //--

  QHBoxLayout *dimensionLayout = new QHBoxLayout;
  dimensionLayout->setMargin(2); dimensionLayout->setSpacing(2);

  dimensionLayout->addWidget(new QLabel("Dimension"));
  dimensionLayout->addWidget(dimension2DRadio_ = new QRadioButton("2D"));
  dimensionLayout->addWidget(dimension3DRadio_ = new QRadioButton("3D"));
  dimensionLayout->addStretch(1);

  dimension2DRadio_->setChecked(true);

  plotPlayout->addLayout(dimensionLayout);

  //--

  QHBoxLayout *plotButtonsLayout = new QHBoxLayout;
  plotButtonsLayout->setMargin(2); plotButtonsLayout->setSpacing(2);

  QPushButton *overlayPlotButton = new QPushButton("Overlay Plot");
  QPushButton *addPlotButton     = new QPushButton("Add Plot");

  connect(overlayPlotButton, SIGNAL(clicked()), this, SLOT(overlayPlotSlot()));
  connect(addPlotButton    , SIGNAL(clicked()), this, SLOT(addPlotSlot()));

  plotButtonsLayout->addWidget(overlayPlotButton);
  plotButtonsLayout->addWidget(addPlotButton);
  plotButtonsLayout->addStretch(1);

  plotPlayout->addLayout(plotButtonsLayout);

  //---

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->setMargin(2); buttonLayout->setSpacing(2);

  buttonLayout->addStretch(1);

  QPushButton *closeButton  = new QPushButton("Close");

  buttonLayout->addWidget(closeButton);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(closeSlot()));

  layout->addLayout(buttonLayout);
}

void
CQGnuPlotDataDialog::
generateSlot()
{
  int n = numberEdit_->value();

  CGnuPlotFile         newFile;
  CGnuPlotFile::Set    newSet;
  CGnuPlotFile::SubSet newSubSet;

  for (int i = 0; i < n; ++i) {
    CGnuPlotFile::Fields fields;

    std::string str1 = std::to_string(i);
    std::string str2 = std::to_string(i);

    CGnuPlotFile::SubSetLine newSubSetLine;

    newSubSetLine.fields.push_back(str1);
    newSubSetLine.fields.push_back(str2);

    newSubSet.lines.push_back(newSubSetLine);
  }

  newSet.subSets.push_back(newSubSet);

  newFile.addSet(newSet);

  tree_->model()->setFile(newFile);

  tree_->reload();
}

void
CQGnuPlotDataDialog::
filterSlot()
{
  QString filterStr = filterEdit_->text();

  if (filterStr == "")
    return;

  CGnuPlotUsingCols usingCols;

  usingCols.init(filterStr.toStdString());

  CQGnuPlotDataModel *model = tree_->model();

  const CGnuPlotFile &file = model->file();

  CGnuPlotFile newFile;

  for (int setNum = 0; setNum < file.numSets(); ++setNum) {
    CGnuPlotFile::Set newSet;

    if      (! model->isHierarchical1() && ! model->isHierarchical2()) {
      CGnuPlotFile::SubSet newSubSet;

      for (int r = 0; r < model->rowCount(); ++r) {
        CGnuPlotPlot::Values values;

        for (int c = 0; c < model->columnCount(); ++c) {
          QModelIndex ind = model->index(r, c);

          QVariant var = model->data(ind);

          std::string field = var.toString().toStdString();

          CExprValuePtr value;

          double real = 0.0;

          int len = field.size();

          if      (len && field[0] == '\"' && field[len - 1] == '\"')
            value = CExprInst->createStringValue(field.substr(1, len - 2));
          else if (CGnuPlot::fieldToReal(field, real))
            value = CExprInst->createRealValue(real);
          else
            value = CExprInst->createStringValue(field);

          values.push_back(value);
        }

        bool             bad;
        CGnuPlot::Values newValues;
        CGnuPlot::Params params;

        usingCols.decodeValues(0, setNum, r, values, bad, newValues, params);

        CGnuPlotFile::SubSetLine newSubSetLine;

        newSubSetLine.num = r;

        CGnuPlotFile::Fields fields;

        for (const auto &v : newValues) {
          std::string str;

          v->getStringValue(str);

          newSubSetLine.fields.push_back(str);
        }

        newSubSet.lines.push_back(newSubSetLine);
      }

      newSet.num = setNum;

      newSet.subSets.push_back(newSubSet);

      newFile.addSet(newSet);
    }
    else if (model->isHierarchical1()) {
      for (int subSetNum = 0; subSetNum < model->rowCount(); ++subSetNum) {
        CGnuPlotFile::SubSet newSubSet;

        QModelIndex ind = model->index(subSetNum, 0);

        for (int r = 0; r < model->rowCount(ind); ++r) {
          CGnuPlotPlot::Values values;

          for (int c = 0; c < model->columnCount(); ++c) {
            QModelIndex ind1 = model->index(r, c, ind);

            QVariant var = model->data(ind1);

            std::string field = var.toString().toStdString();

            CExprValuePtr value;

            double real = 0.0;

            int len = field.size();

            if      (len && field[0] == '\"' && field[len - 1] == '\"')
              value = CExprInst->createStringValue(field.substr(1, len - 2));
            else if (CGnuPlot::fieldToReal(field, real))
              value = CExprInst->createRealValue(real);
            else
              value = CExprInst->createStringValue(field);

            values.push_back(value);
          }

          bool             bad;
          CGnuPlot::Values newValues;
          CGnuPlot::Params params;

          usingCols.decodeValues(0, setNum, r, values, bad, newValues, params);

          CGnuPlotFile::SubSetLine newSubSetLine;

          newSubSetLine.num = r;

          CGnuPlotFile::Fields fields;

          for (const auto &v : newValues) {
            std::string str;

            v->getStringValue(str);

            newSubSetLine.fields.push_back(str);
          }

          newSubSet.lines.push_back(newSubSetLine);
        }

        newSet.num = setNum;

        newSet.subSets.push_back(newSubSet);
      }

      newFile.addSet(newSet);
    }
  }

  //---

  CQGnuPlotDataDialog *dialog = new CQGnuPlotDataDialog(window_, newFile);

  dialog->show();
}

void
CQGnuPlotDataDialog::
overlayPlotSlot()
{
  doPlot(false);
}

void
CQGnuPlotDataDialog::
addPlotSlot()
{
  doPlot(true);
}

void
CQGnuPlotDataDialog::
doPlot(bool add)
{
  CGnuPlotGroup *group;

  if (add) {
    group = window_->createTiledGroup();

    if (! group)
      return;
  }
  else {
    window_->initCurrentGroup();

    group = window_->currentGroup();

    if (! group)
      return;
  }

  CQGnuPlotDataModel *model = tree_->model();

  CQHeaderView *header = tree_->header();

  typedef std::set<int> Active;

  Active active;

  for (int c = 0; c < model->columnCount(); ++c) {
    QCheckBox *cb = qobject_cast<QCheckBox *>(header->widget(c));

    if (cb && cb->isChecked())
      active.insert(c);
  }

  bool is2D = dimension2DRadio_->isChecked();

  const CGnuPlotFile &file = model->file();

  CGnuPlotTypes::PlotStyle style = CQGnuPlotEnum::plotStyleConv(enum_->plotStyle());

  if      (! model->isHierarchical1() && ! model->isHierarchical2()) {
    CGnuPlotPlotP plot = group->createNewPlot(style);

    plot->setSetNum  (0);
    plot->setDataFile(file);

    plot->set2D(  is2D);
    plot->set3D(! is2D);

    plot->init();

    //---

    for (int r = 0; r < model->rowCount(); ++r) {
      CGnuPlotPlot::Values values;

      for (int c = 0; c < model->columnCount(); ++c) {
        if (active.find(c) == active.end())
          continue;

        QModelIndex ind = model->index(r, c);

        QVariant var = model->data(ind);

        std::string field = var.toString().toStdString();

        CExprValuePtr value;

        double real = 0.0;

        int len = field.size();

        if      (len && field[0] == '\"' && field[len - 1] == '\"')
          value = CExprInst->createStringValue(field.substr(1, len - 2));
        else if (CGnuPlot::fieldToReal(field, real))
          value = CExprInst->createRealValue(real);
        else
          value = CExprInst->createStringValue(field);

        values.push_back(value);
      }

      if (is2D)
        plot->addPoint2D(values);
      else
        plot->addPoint3D(0, values);
    }
  }
  else if (model->isHierarchical1()) {
    CGnuPlotPlotP plot = group->createNewPlot(style);

    plot->setSetNum  (0);
    plot->setDataFile(file);

    plot->set2D(  is2D);
    plot->set3D(! is2D);

    plot->init();

    //---

    for (int subSetNum = 0; subSetNum < model->rowCount(); ++subSetNum) {
      QModelIndex ind = model->index(subSetNum, 0);

      for (int r = 0; r < model->rowCount(ind); ++r) {
        CGnuPlotPlot::Values values;

        for (int c = 0; c < model->columnCount(); ++c) {
          if (active.find(c) == active.end())
            continue;

          QModelIndex ind1 = model->index(r, c, ind);

          QVariant var = model->data(ind1);

          std::string field = var.toString().toStdString();

          CExprValuePtr value;

          double real = 0.0;

          int len = field.size();

          if      (len && field[0] == '\"' && field[len - 1] == '\"')
            value = CExprInst->createStringValue(field.substr(1, len - 2));
          else if (CGnuPlot::fieldToReal(field, real))
            value = CExprInst->createRealValue(real);
          else
            value = CExprInst->createStringValue(field);

          values.push_back(value);
        }

        bool discontinuity = (subSetNum > 0 && r == 0);

        if (is2D)
          plot->addPoint2D(values, discontinuity);
        else
          plot->addPoint3D(subSetNum, values, discontinuity);
      }
    }
  }
  else {
    CGnuPlotPlotP plot;

    if (! is2D) {
      plot = group->createNewPlot(style);

      plot->setSetNum  (0);
      plot->setDataFile(file);

      plot->set2D(false);
      plot->set3D(true);

      plot->init();
    }

    //---

    for (int setNum = 0; setNum < model->rowCount(); ++setNum) {
      if (is2D) {
        plot = group->createNewPlot(style);

        plot->setSetNum  (setNum);
        plot->setDataFile(file);

        plot->set2D(true);
        plot->set3D(false);

        plot->init();
      }

      //---

      QModelIndex ind = model->index(setNum, 0);

      for (int subSetNum = 0; subSetNum < model->rowCount(); ++subSetNum) {
        QModelIndex ind1 = model->index(subSetNum, 0, ind);

        for (int r = 0; r < model->rowCount(ind1); ++r) {
          CGnuPlotPlot::Values values;

          for (int c = 0; c < model->columnCount(); ++c) {
            if (active.find(c) == active.end())
              continue;

            QModelIndex ind2 = model->index(r, c, ind1);

            QVariant var = model->data(ind2);

            std::string field = var.toString().toStdString();

            CExprValuePtr value;

            double real = 0.0;

            int len = field.size();

            if      (len && field[0] == '\"' && field[len - 1] == '\"')
              value = CExprInst->createStringValue(field.substr(1, len - 2));
            else if (CGnuPlot::fieldToReal(field, real))
              value = CExprInst->createRealValue(real);
            else
              value = CExprInst->createStringValue(field);

            values.push_back(value);
          }

          bool discontinuity = (subSetNum > 0 && r == 0);

          if (is2D)
            plot->addPoint2D(values, discontinuity);
          else
            plot->addPoint3D(subSetNum, values, discontinuity);
        }
      }
    }
  }
}

void
CQGnuPlotDataDialog::
closeSlot()
{
  this->close();
}
