#include <CQGnuPlotDataDialog.h>
#include <CQGnuPlotDataTree.h>
#include <CQGnuPlotDataModel.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotFilename.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotIndexData.h>
#include <CGnuPlotEveryData.h>
#include <CGnuPlotWhereData.h>
#include <CQHeaderView.h>
#include <CQGnuPlotEnum.h>
#include <CQEnumCombo.h>
#include <CQIntegerSpin.h>
#include <CGnuPlotValueKey.h>
#include <CGnuPlotLineStyle.h>

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

  QGroupBox *loadGroup = new QGroupBox("File Data");

  QVBoxLayout *loadLayout = new QVBoxLayout(loadGroup);
  loadLayout->setMargin(2); loadLayout->setSpacing(2);

  layout->addWidget(loadGroup);

  //--

  QHBoxLayout *fileLayout = new QHBoxLayout;
  fileLayout->setMargin(2); fileLayout->setSpacing(2);

  fileEdit_ = new CQGnuPlotFilename;

  fileEdit_->setName(file.fileName().c_str());
  fileEdit_->setPattern("Text (*.txt);; CSV (*.csv);; File (*.*);; All (*)");

  QPushButton *loadButton = new QPushButton("Load");

  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadSlot()));

  fileLayout->addWidget(new QLabel("File"));
  fileLayout->addWidget(fileEdit_);
  fileLayout->addWidget(loadButton);

  loadLayout->addLayout(fileLayout);

  //------

  QHBoxLayout *numberLayout = new QHBoxLayout;
  numberLayout->setMargin(2); numberLayout->setSpacing(2);

  numberEdit_ = new CQIntegerSpin;

  numberEdit_->setValue(100);

  QPushButton *generateButton = new QPushButton("Generate");

  connect(generateButton, SIGNAL(clicked()), this, SLOT(generateSlot()));

  numberLayout->addWidget(generateButton);
  numberLayout->addWidget(numberEdit_);
  numberLayout->addWidget(new QLabel("Points"));
  numberLayout->addStretch(1);

  loadLayout->addLayout(numberLayout);

  //------

  QHBoxLayout *formatLayout = new QHBoxLayout;
  formatLayout->setMargin(2); formatLayout->setSpacing(2);

  formatLayout->addWidget(csvCheck_ = new QCheckBox("CSV"));
  formatLayout->addWidget(headerCheck_ = new QCheckBox("Header"));
  formatLayout->addStretch(1);

  loadLayout->addLayout(formatLayout);

  //------

  QHBoxLayout *separatorLayout = new QHBoxLayout;
  separatorLayout->setMargin(2); separatorLayout->setSpacing(2);

  separatorLayout->addWidget(new QLabel("Separator"));
  separatorLayout->addWidget(separatorEdit_ = new QLineEdit);
  separatorLayout->addStretch(1);

  loadLayout->addLayout(separatorLayout);

  char sepChar = file.separator();

  std::string sepStr(&sepChar, 1);

  separatorEdit_->setText(sepStr.c_str());

  //------

  QHBoxLayout *missingLayout = new QHBoxLayout;
  missingLayout->setMargin(2); missingLayout->setSpacing(2);

  missingLayout->addWidget(new QLabel("Missing"));
  missingLayout->addWidget(missingEdit_ = new QLineEdit);
  missingLayout->addStretch(1);

  loadLayout->addLayout(missingLayout);

  missingEdit_->setText(file.missingStr().c_str());

  //------

  tree_ = new CQGnuPlotDataTree(file);

  layout->addWidget(tree_);

  //------

  QGroupBox *filterGroup = new QGroupBox("Filter Data");

  QVBoxLayout *filterLayout = new QVBoxLayout(filterGroup);
  filterLayout->setMargin(2); filterLayout->setSpacing(2);

  layout->addWidget(filterGroup);

  //--

  QHBoxLayout *usingLayout = new QHBoxLayout;
  usingLayout->setMargin(2); usingLayout->setSpacing(2);

  usingLayout->addWidget(new QLabel("Using"));
  usingLayout->addWidget(usingEdit_ = new QLineEdit);

  filterLayout->addLayout(usingLayout);

  usingEdit_->setText(window_->qapp()->usingString().c_str());

  //--

  QHBoxLayout *indexLayout = new QHBoxLayout;
  indexLayout->setMargin(2); indexLayout->setSpacing(2);

  indexLayout->addWidget(new QLabel("Index"));
  indexLayout->addWidget(indexEdit_ = new QLineEdit);

  filterLayout->addLayout(indexLayout);

  //--

  QHBoxLayout *everyLayout = new QHBoxLayout;
  everyLayout->setMargin(2); everyLayout->setSpacing(2);

  everyLayout->addWidget(new QLabel("Every"));
  everyLayout->addWidget(everyEdit_ = new QLineEdit);

  filterLayout->addLayout(everyLayout);

  //--

  QHBoxLayout *whereLayout = new QHBoxLayout;
  whereLayout->setMargin(2); whereLayout->setSpacing(2);

  whereLayout->addWidget(new QLabel("Where"));
  whereLayout->addWidget(whereEdit_ = new QLineEdit);

  filterLayout->addLayout(whereLayout);

  //--

  QHBoxLayout *filterOptionsLayout = new QHBoxLayout;
  filterOptionsLayout->setMargin(2); filterOptionsLayout->setSpacing(2);

  filterOptionsLayout->addWidget(averageCheck_ = new QCheckBox("Average"));
  filterOptionsLayout->addWidget(summedCheck_  = new QCheckBox("Summed" ));
  filterOptionsLayout->addStretch(1);

  filterLayout->addLayout(filterOptionsLayout);

  //------

  QGroupBox *plotGroup = new QGroupBox("Plot Style");

  QVBoxLayout *plotPlayout = new QVBoxLayout(plotGroup);
  plotPlayout->setMargin(2); plotPlayout->setSpacing(2);

  layout->addWidget(plotGroup);

  //--

  enum_->setPlotStyle(CQGnuPlotEnum::plotStyleConv(window_->qapp()->plotStyle()));

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

  QHBoxLayout *lineStyleLayout = new QHBoxLayout;
  lineStyleLayout->setMargin(2); lineStyleLayout->setSpacing(2);

  lineStyleLayout->addWidget(new QLabel("Line Style"));
  lineStyleLayout->addWidget(lineTypeEdit_ = new CQIntegerSpin);
  lineStyleLayout->addStretch(1);

  lineTypeEdit_->setValue(-1);

  plotPlayout->addLayout(lineStyleLayout);

  //--

  QHBoxLayout *plotButtonsLayout = new QHBoxLayout;
  plotButtonsLayout->setMargin(2); plotButtonsLayout->setSpacing(2);

  QPushButton *filterButton      = new QPushButton("Filter");
  QPushButton *overlayPlotButton = new QPushButton("Overlay Plot");
  QPushButton *addPlotButton     = new QPushButton("Add Plot");

  connect(filterButton     , SIGNAL(clicked()), this, SLOT(filterSlot()));
  connect(overlayPlotButton, SIGNAL(clicked()), this, SLOT(overlayPlotSlot()));
  connect(addPlotButton    , SIGNAL(clicked()), this, SLOT(addPlotSlot()));

  plotButtonsLayout->addWidget(filterButton);
  plotButtonsLayout->addWidget(overlayPlotButton);
  plotButtonsLayout->addWidget(addPlotButton);
  plotButtonsLayout->addStretch(1);

  layout->addLayout(plotButtonsLayout);

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
loadSlot()
{
  QString filename = fileEdit_->name();

  CGnuPlotFile dataFile;

  //dataFile_.setBinary(binary);
  //dataFile_.setMatrix(matrix);
  //dataFile_.setBinaryFormat(binaryFormat);

  // if (! sizes.empty())
  //   dataFile_.setBinarySize(sizes[0]);

  if (! window_->app()->readDataFile(filename.toStdString(), dataFile))
    return;

  dataFile.setCsv(csvCheck_->isChecked());

  dataFile.setColumnHeaders(headerCheck_->isChecked());

  QString sepStr = separatorEdit_->text();

  if (sepStr.length()) {
    char c = '\0';

    if      (sepStr == "whitespace")
      c = ' ';
    else if (sepStr == "tab" || sepStr == "\\t")
      c = '\t';
    else if (sepStr == "comma")
      c = ',';
    else
      c = sepStr[0].toLatin1();

    dataFile.setSeparator(c);
  }

  dataFile.processFile();

  tree_->load(dataFile);
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
  CExpr *expr = window_->app()->expr();

  missingStr_ = missingEdit_->text().toStdString();

  //---

  Smooth smooth = Smooth::None;

  if      (averageCheck_->isChecked())
    smooth = Smooth::Average;
  else if (summedCheck_->isChecked())
    smooth = Smooth::Summed;

  //---

  QString usingStr = usingEdit_->text();
  QString indexStr = indexEdit_->text();
  QString everyStr = everyEdit_->text();
  QString whereStr = whereEdit_->text();

  CGnuPlotUsingCols usingCols(window_->app());
  CGnuPlotIndexData indexData;
  CGnuPlotEveryData everyData;
  CGnuPlotWhereData whereData;

  usingCols.parse(usingStr.toStdString());
  indexData.parse(expr, indexStr.toStdString());
  everyData.parse(expr, everyStr.toStdString());
  whereData.parse(whereStr.toStdString());

  CQGnuPlotDataModel *model = tree_->model();

  //const CGnuPlotFile &file = model->file();

  CGnuPlotFile newFile;

  CGnuPlotFile::Set newSet;

  // single set, single subset
  if      (! model->isHierarchical1() && ! model->isHierarchical2()) {
    int setNum    = 0;
    int subSetNum = 0;

    CGnuPlotFile::SubSet newSubSet;

    for (int lineNum = 0; lineNum < model->rowCount(); ++lineNum) {
      bool lineActive = (everyStr != "" ? everyData.validPointIndex(lineNum) : true);
      if (! lineActive) continue;

      CGnuPlotPlot::Values values;

      for (int c = 0; c < model->columnCount(); ++c) {
        QModelIndex ind = model->index(lineNum, c);

        QVariant var = model->data(ind);

        std::string field = var.toString().toStdString();

        bool missing;

        CExprValuePtr value = fieldToValue(field, missing);

        if (missing)
          value->setMissing(true);

        values.push_back(value);
      }

      //---

      if (whereStr != "") {
        if (! whereData.isValid(expr, setNum, lineNum, values))
          continue;
      }

      //---

      CGnuPlot::Values newValues;
      bool             bad = false;
      CGnuPlot::Params params;

      if (usingStr != "")
        usingCols.decodeValues(0, setNum, lineNum, values, bad, newValues, params);
      else
        newValues = values;

      if (bad)
        continue;

      //---

      CGnuPlotFile::SubSetLine newSubSetLine;

      newSubSetLine.num = lineNum;

      CGnuPlotFile::Fields fields;

      for (const auto &v : newValues) {
        std::string str;

        v->getStringValue(str);

        newSubSetLine.fields.push_back(str);
      }

      newSubSet.lines.push_back(newSubSetLine);
    }

    if (smooth != Smooth::None)
      newSubSet.lines = smoothLines(newSubSet.lines, smooth);

    newSubSet.num = subSetNum;

    newSet.subSets.push_back(newSubSet);

    newSet.num = setNum;

    newFile.addSet(newSet);
  }
  // single set, multiple subsets
  else if (model->isHierarchical1()) {
    int setNum = 0;

    for (int subSetNum = 0; subSetNum < model->rowCount(); ++subSetNum) {
      bool subSetActive = (everyStr != "" ? everyData.validBlockIndex(subSetNum) : true);
      if (! subSetActive) continue;

      CGnuPlotFile::SubSet newSubSet;

      QModelIndex ind = model->index(subSetNum, 0);

      for (int lineNum = 0; lineNum < model->rowCount(ind); ++lineNum) {
        bool lineActive = (everyStr != "" ? everyData.validPointIndex(lineNum) : true);
        if (! lineActive) continue;

        CGnuPlotPlot::Values values;

        for (int c = 0; c < model->columnCount(); ++c) {
          QModelIndex ind1 = model->index(lineNum, c, ind);

          QVariant var = model->data(ind1);

          std::string field = var.toString().toStdString();

          bool missing;

          CExprValuePtr value = fieldToValue(field, missing);

          if (missing)
            value->setMissing(true);

          values.push_back(value);
        }

        //---

        if (whereStr != "") {
          if (! whereData.isValid(expr, setNum, lineNum, values))
            continue;
        }

        //---

        CGnuPlot::Values newValues;
        bool             bad = false;
        CGnuPlot::Params params;

        if (usingStr != "")
          usingCols.decodeValues(0, setNum, lineNum, values, bad, newValues, params);
        else
          newValues = values;

        if (bad)
          continue;

        //---

        CGnuPlotFile::SubSetLine newSubSetLine;

        newSubSetLine.num = lineNum;

        CGnuPlotFile::Fields fields;

        for (const auto &v : newValues) {
          std::string str;

          v->getStringValue(str);

          newSubSetLine.fields.push_back(str);
        }

        newSubSet.lines.push_back(newSubSetLine);
      }

      if (smooth != Smooth::None)
        newSubSet.lines = smoothLines(newSubSet.lines, smooth);

      newSubSet.num = subSetNum;

      newSet.subSets.push_back(newSubSet);
    }

    newSet.num = setNum;

    newFile.addSet(newSet);
  }
  // multiple sets, multiple subsets
  else {
    for (int setNum = 0; setNum < model->rowCount(); ++setNum) {
      bool setActive = (indexStr != "" ? indexData.validIndex(setNum) : true);
      if (! setActive) continue;

      CGnuPlotFile::SubSet newSubSet;

      QModelIndex ind = model->index(setNum, 0);

      for (int subSetNum = 0; subSetNum < model->rowCount(ind); ++subSetNum) {
        bool subSetActive = (everyStr != "" ? everyData.validBlockIndex(subSetNum) : true);
        if (! subSetActive) continue;

        QModelIndex ind1 = model->index(subSetNum, 0, ind);

        for (int lineNum = 0; lineNum < model->rowCount(ind1); ++lineNum) {
          bool lineActive = (everyStr != "" ? everyData.validPointIndex(lineNum) : true);
          if (! lineActive) continue;

          CGnuPlotPlot::Values values;

          for (int c = 0; c < model->columnCount(); ++c) {
            QModelIndex ind2 = model->index(lineNum, c, ind1);

            QVariant var = model->data(ind2);

            std::string field = var.toString().toStdString();

            bool missing;

            CExprValuePtr value = fieldToValue(field, missing);

            if (missing)
              value->setMissing(true);

            values.push_back(value);
          }

          //---

          if (whereStr != "") {
            if (! whereData.isValid(expr, setNum, lineNum, values))
              continue;
          }

          //---

          CGnuPlot::Values newValues;
          bool             bad = false;
          CGnuPlot::Params params;

          if (usingStr != "")
            usingCols.decodeValues(0, setNum, lineNum, values, bad, newValues, params);
          else
            newValues = values;

          if (bad)
            continue;

          //---

          CGnuPlotFile::SubSetLine newSubSetLine;

          newSubSetLine.num = lineNum;

          CGnuPlotFile::Fields fields;

          for (const auto &v : newValues) {
            std::string str;

            v->getStringValue(str);

            newSubSetLine.fields.push_back(str);
          }

          newSubSet.lines.push_back(newSubSetLine);
        }

        if (smooth != Smooth::None)
          newSubSet.lines = smoothLines(newSubSet.lines, smooth);

        newSubSet.num = subSetNum;

        newSet.subSets.push_back(newSubSet);
      }

      newSet.num = setNum;

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
  CExpr *expr = window_->app()->expr();

  missingStr_ = missingEdit_->text().toStdString();

  //---

  Smooth smooth = Smooth::None;

  if      (averageCheck_->isChecked())
    smooth = Smooth::Average;
  else if (summedCheck_->isChecked())
    smooth = Smooth::Summed;

  //---

  // TODO: filter on plot

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

  QString usingStr = usingEdit_->text();
  QString indexStr = indexEdit_->text();
  QString everyStr = everyEdit_->text();
  QString whereStr = whereEdit_->text();

  CGnuPlotUsingCols usingCols(window_->app());
  CGnuPlotIndexData indexData;
  CGnuPlotEveryData everyData;
  CGnuPlotWhereData whereData;

  usingCols.parse(usingStr.toStdString());
  indexData.parse(expr, indexStr.toStdString());
  everyData.parse(expr, everyStr.toStdString());
  whereData.parse(whereStr.toStdString());

  CQGnuPlotDataModel *model = tree_->model();

  //---

  CQHeaderView *header = tree_->header();

  typedef std::set<int> Active;

  Active active;

  for (int c = 0; c < model->columnCount(); ++c) {
    QCheckBox *cb = qobject_cast<QCheckBox *>(header->widget(c));

    if (cb && cb->isChecked())
      active.insert(c);
  }

  //---

  bool is2D = dimension2DRadio_->isChecked();

  const CGnuPlotFile &file = model->file();

  CGnuPlotTypes::PlotStyle style = CQGnuPlotEnum::plotStyleConv(enum_->plotStyle());

  // single set, single subset
  if      (! model->isHierarchical1() && ! model->isHierarchical2()) {
    int setNum = 0;

    CGnuPlotPlotP plot = group->createNewPlot(style);

    initPlotStyle(plot);

    plot->setSetNum  (setNum);
    plot->setDataFile(file);

    plot->set2D(  is2D);
    plot->set3D(! is2D);

    plot->init();

    //---

    ValuesDataList valuesDataList;

    for (int lineNum = 0; lineNum < model->rowCount(); ++lineNum) {
      bool lineActive = (everyStr != "" ? everyData.validPointIndex(lineNum) : true);
      if (! lineActive) continue;

      CGnuPlotPlot::Values values;
      bool                 missing = false;

      for (int c = 0; c < model->columnCount(); ++c) {
        if (active.find(c) == active.end())
          continue;

        QModelIndex ind = model->index(lineNum, c);

        QVariant var = model->data(ind);

        std::string field = var.toString().toStdString();

        bool missing;

        CExprValuePtr value = fieldToValue(field, missing);

        if (missing)
          value->setMissing(true);

        values.push_back(value);
      }

      if (missing)
        continue;

      //---

      if (whereStr != "") {
        if (! whereData.isValid(expr, setNum, lineNum, values))
          continue;
      }

      //---

      CGnuPlot::Values newValues;
      bool             bad = false;
      CGnuPlot::Params params;

      if (usingStr != "")
        usingCols.decodeValues(0, setNum, lineNum, values, bad, newValues, params);
      else
        newValues = values;

      if (bad)
        continue;

      //---

      bool discontinuity = false;

      valuesDataList.push_back(ValuesData(newValues, discontinuity, bad, params));
    }

    if (smooth != Smooth::None)
      valuesDataList = smoothValuesList(valuesDataList, smooth);

    for (const auto &vd : valuesDataList) {
      if (is2D)
        plot->addPoint2D(vd.values, vd.discontinuity, vd.bad, vd.params);
      else
        plot->addPoint3D(0, vd.values, vd.discontinuity, vd.bad, vd.params);
    }
  }
  // single set, multiple subsets
  else if (model->isHierarchical1()) {
    int setNum = 0;

    CGnuPlotPlotP plot = group->createNewPlot(style);

    initPlotStyle(plot);

    plot->setSetNum  (setNum);
    plot->setDataFile(file);

    plot->set2D(  is2D);
    plot->set3D(! is2D);

    plot->init();

    //---

    for (int subSetNum = 0; subSetNum < model->rowCount(); ++subSetNum) {
      bool subSetActive = (everyStr != "" ? everyData.validBlockIndex(subSetNum) : true);
      if (! subSetActive) continue;

      QModelIndex ind = model->index(subSetNum, 0);

      //---

      ValuesDataList valuesDataList;

      for (int lineNum = 0; lineNum < model->rowCount(ind); ++lineNum) {
        bool lineActive = (everyStr != "" ? everyData.validPointIndex(lineNum) : true);
        if (! lineActive) continue;

        CGnuPlotPlot::Values values;

        for (int c = 0; c < model->columnCount(); ++c) {
          if (active.find(c) == active.end())
            continue;

          QModelIndex ind1 = model->index(lineNum, c, ind);

          QVariant var = model->data(ind1);

          std::string field = var.toString().toStdString();

          bool missing;

          CExprValuePtr value = fieldToValue(field, missing);

          if (missing)
            value->setMissing(true);

          values.push_back(value);
        }

        //---

        if (whereStr != "") {
          if (! whereData.isValid(expr, setNum, lineNum, values))
            continue;
        }

        //---

        CGnuPlot::Values newValues;
        bool             bad = false;
        CGnuPlot::Params params;

        if (usingStr != "")
          usingCols.decodeValues(0, setNum, lineNum, values, bad, newValues, params);
        else
          newValues = values;

        if (bad)
          continue;

        //---

        bool discontinuity = (subSetNum > 0 && lineNum == 0);

        valuesDataList.push_back(ValuesData(newValues, discontinuity, bad, params));
      }

      if (smooth != Smooth::None)
        valuesDataList = smoothValuesList(valuesDataList, smooth);

      for (const auto &vd : valuesDataList) {
        if (is2D)
          plot->addPoint2D(vd.values, vd.discontinuity, vd.bad, vd.params);
        else
          plot->addPoint3D(subSetNum, vd.values, vd.discontinuity, vd.bad, vd.params);
      }
    }
  }
  // multiple sets, multiple subsets
  else {
    CGnuPlotPlotP plot;

    // for 3D we want stack of plots (single set)
    if (! is2D) {
      int setNum = 0;

      plot = group->createNewPlot(style);

      initPlotStyle(plot);

      plot->setSetNum  (setNum);
      plot->setDataFile(file);

      plot->set2D(false);
      plot->set3D(true);

      plot->init();
    }

    //---

    for (int setNum = 0; setNum < model->rowCount(); ++setNum) {
      bool setActive = (indexStr != "" ? indexData.validIndex(setNum) : true);
      if (! setActive) continue;

      //---

      if (is2D) {
        plot = group->createNewPlot(style);

        initPlotStyle(plot);

        plot->setSetNum  (setNum);
        plot->setDataFile(file);

        plot->set2D(true);
        plot->set3D(false);

        plot->init();
      }

      //---

      QModelIndex ind = model->index(setNum, 0);

      for (int subSetNum = 0; subSetNum < model->rowCount(ind); ++subSetNum) {
        bool subSetActive = (everyStr != "" ? everyData.validBlockIndex(subSetNum) : true);
        if (! subSetActive) continue;

        QModelIndex ind1 = model->index(subSetNum, 0, ind);

        //---

        ValuesDataList valuesDataList;

        for (int lineNum = 0; lineNum < model->rowCount(ind1); ++lineNum) {
          bool lineActive = (everyStr != "" ? everyData.validPointIndex(lineNum) : true);
          if (! lineActive) continue;

          CGnuPlotPlot::Values values;

          for (int c = 0; c < model->columnCount(); ++c) {
            if (active.find(c) == active.end())
              continue;

            QModelIndex ind2 = model->index(lineNum, c, ind1);

            QVariant var = model->data(ind2);

            std::string field = var.toString().toStdString();

            bool missing;

            CExprValuePtr value = fieldToValue(field, missing);

            if (missing)
              value->setMissing(true);

            values.push_back(value);
          }

          //---

          if (whereStr != "") {
            if (! whereData.isValid(expr, setNum, lineNum, values))
              continue;
          }

          //---

          CGnuPlot::Values newValues;
          bool             bad = false;
          CGnuPlot::Params params;

          if (usingStr != "")
            usingCols.decodeValues(0, setNum, lineNum, values, bad, newValues, params);
          else
            newValues = values;

          if (bad)
            continue;

          //---

          bool discontinuity = (subSetNum > 0 && lineNum == 0);

          valuesDataList.push_back(ValuesData(newValues, discontinuity, bad, params));
        }

        if (smooth != Smooth::None)
          valuesDataList = smoothValuesList(valuesDataList, smooth);

        for (const auto &vd : valuesDataList) {
          if (is2D)
            plot->addPoint2D(vd.values, vd.discontinuity, vd.bad, vd.params);
          else
            plot->addPoint3D(subSetNum, vd.values, vd.discontinuity, vd.bad, vd.params);
        }
      }
    }
  }
}

void
CQGnuPlotDataDialog::
initPlotStyle(CGnuPlotPlotP plot)
{
  int it = lineTypeEdit_->value();

  if (it < 0)
    return;

  CGnuPlotLineStyle lineStyle(window_->app());

  lineStyle.setLineType(it);

  plot->setLineStyle(lineStyle);
}

CExprValuePtr
CQGnuPlotDataDialog::
fieldToValue(const std::string &field, bool &missing) const
{
  missing = false;

  CExprValuePtr value;

  auto len = field.size();

  CExpr *expr = window_->app()->expr();

  double r;

  if      (field == missingStr_) {
    value = expr->createRealValue(CMathGen::getNaN());

    missing = true;
  }
  else if (len && field[0] == '\"' && field[len - 1] == '\"')
    value = expr->createStringValue(field.substr(1, len - 2));
  else if (CGnuPlot::fieldToReal(field, r))
    value = expr->createRealValue(r);
  else
    value = expr->createStringValue(field);

  return value;
}

CGnuPlotFile::SubSetLines
CQGnuPlotDataDialog::
smoothLines(const CGnuPlotFile::SubSetLines &lines, Smooth /*smooth*/) const
{
  typedef std::map<std::string, CGnuPlotFile::SubSetLines> KeyLines;

  KeyLines keyLines;

  for (const auto &l : lines) {
    if (l.fields.empty()) continue;

    const std::string &str = l.fields[0].str();

    keyLines[str].push_back(l);
  }

  CGnuPlotFile::SubSetLines newLines;

  for (const auto &kl : keyLines)
    newLines.push_back(kl.second[0]);

  return newLines;
}

CQGnuPlotDataDialog::ValuesDataList
CQGnuPlotDataDialog::
smoothValuesList(const ValuesDataList &valuesDataList, Smooth smooth) const
{
  CExpr *expr = window_->app()->expr();

  typedef std::map<double, ValuesDataList> KeyValuesDataList;

  KeyValuesDataList keyValuesDataList;

  for (const auto &vd : valuesDataList) {
    if (vd.values.empty()) continue;

    double r;

    vd.values[0]->getRealValue(r);

    keyValuesDataList[r].push_back(vd);
  }

  ValuesDataList newValuesDataList;

  for (const auto &kv : keyValuesDataList) {
    const ValuesDataList &vd = kv.second;

    std::vector<double> sum;

    int nv = vd[0].values.size();

    sum.resize(nv);

    for (uint j = 0; j < vd.size(); ++j) {
      const ValuesData &v = vd[j];

      for (uint k = 0; k < v.values.size(); ++k) {
        double r = 0.0;

        v.values[k]->getRealValue(r);

        sum[k] += r;
      }
    }

    if (smooth == Smooth::Average) {
      for (int k = 0; k < nv; ++k)
        sum[k] /= vd.size();
    }

    ValuesData newValuesData;

    for (uint j = 0; j < vd.size(); ++j)
      newValuesData.values.push_back(expr->createRealValue(sum[j]));

    newValuesDataList.push_back(newValuesData);
  }

  return newValuesDataList;
}

CGnuPlot::Matrix
CQGnuPlotDataDialog::
smoothValuesList(const CGnuPlot::Matrix &valuesList, Smooth smooth) const
{
  CExpr *expr = window_->app()->expr();

  typedef std::map<double, CGnuPlot::Matrix> KeyMatrix;

  KeyMatrix keyMatrix;

  for (const auto &v : valuesList) {
    if (v.empty()) continue;

    double r;

    v[0]->getRealValue(r);

    keyMatrix[r].push_back(v);
  }

  CGnuPlot::Matrix newValuesList;

  for (const auto &km : keyMatrix) {
    const CGnuPlot::Matrix &m = km.second;

    std::vector<double> sum;

    int nv = m[0].size();

    sum.resize(nv);

    for (uint j = 0; j < m.size(); ++j) {
      const CGnuPlot::Values &v = m[j];

      for (uint k = 0; k < v.size(); ++k) {
        double r = 0.0;

        v[k]->getRealValue(r);

        sum[k] += r;
      }
    }

    if (smooth == Smooth::Average) {
      for (int k = 0; k < nv; ++k)
        sum[k] /= m.size();
    }

    CGnuPlot::Values newValues;

    for (uint j = 0; j < m.size(); ++j)
      newValues.push_back(expr->createRealValue(sum[j]));

    newValuesList.push_back(newValues);
  }

  return newValuesList;
}

void
CQGnuPlotDataDialog::
closeSlot()
{
  this->close();
}

QSize
CQGnuPlotDataDialog::
sizeHint() const
{
  QFontMetrics fm(font());

  return QSize(fm.horizontalAdvance(QString("X").repeated(60)), fm.height()*40);
}
