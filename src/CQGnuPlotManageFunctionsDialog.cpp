#include <CQGnuPlotManageFunctionsDialog.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotTreeWidget.h>
#include <CParseLine.h>
#include <CExpr.h>

#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

CQGnuPlotManageFunctionsDialog::
CQGnuPlotManageFunctionsDialog(CQGnuPlotMainWindow *window) :
 CQDialog(window), window_(window)
{
  setWindowTitle("Manage Functions");
}

void
CQGnuPlotManageFunctionsDialog::
createWidgets(QWidget *)
{
  tree_ = new CQGnuPlotTreeWidget;

  tree_->setColumnCount(2);
  tree_->setHeaderLabels(QStringList() << "Name" << "Definition");
  tree_->setSortingEnabled(true);

  tree_->setUniformRowHeights(true);

  tree_->header()->setStretchLastSection(true);

  tree_->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  tree_->setAlternatingRowColors(true);

  addField("", tree_, /*stretch*/false);

  //---

  QGroupBox *groupBox = new QGroupBox;

  //groupBox->setFrameStyle(QFrame::Panel | QFrame::Raised);

  QGridLayout *frameLayout = new QGridLayout(groupBox);
  frameLayout->setMargin(0); frameLayout->setSpacing(2);

  nameEdit_ = new QLineEdit;
  exprEdit_ = new QLineEdit;

  QPushButton *addButton = new QPushButton("Add");

  connect(addButton, SIGNAL(clicked()), this, SLOT(addSlot()));

  frameLayout->addWidget(new QLabel("Name"    ), 0, 0);
  frameLayout->addWidget(nameEdit_             , 0, 1);
  frameLayout->addWidget(new QLabel("Function"), 1, 0);
  frameLayout->addWidget(exprEdit_             , 1, 1);
  frameLayout->addWidget(addButton             , 0, 2);

  addField("", groupBox);

  //---

  QPushButton *updateButton = this->addButton("Update");

  connect(updateButton, SIGNAL(clicked()), this, SLOT(updateFunctions()));

  updateFunctions();
}

void
CQGnuPlotManageFunctionsDialog::
updateFunctions()
{
  CExpr *expr = window_->app()->expr();

  tree_->clear();

  CExpr::StringArray names;

  expr->getFunctionNames(names);

  QFontMetrics fm(font());

  int nf = 0;
  int w  = fm.width("Name") + fm.width("Definition") + 32;

  for (const auto &n : names) {
    CExprFunctionPtr function = expr->getFunction(n);

    if (function->isBuiltin()) continue;

    std::string name = function->name();

    std::string value;

    if (dynamic_cast<CExprUserFunction *>(function.get()))
      value = dynamic_cast<CExprUserFunction *>(function.get())->proc();

    QStringList inames;

    inames << name .c_str();
    inames << value.c_str();

    QTreeWidgetItem *item = new QTreeWidgetItem(tree_, inames);

    tree_->addTopLevelItem(item);

    ++nf;

    w = std::max(w, fm.width(name.c_str()) + fm.width(value.c_str()) + 32);
  }

  nf = std::max(nf, 8);

  size_ = QSize(w, (nf + 2)*fm.height());
}

void
CQGnuPlotManageFunctionsDialog::
addSlot()
{
  std::string identifier = nameEdit_->text().toStdString();
  std::string exprStr    = exprEdit_->text().toStdString();

  CParseLine line(identifier);

  line.skipSpace();

  if (! line.isValid())
    return;

  int pos = line.pos();

  if (! line.skipCIdentifier())
    return;

  std::string name = line.substr(pos, line.pos() - pos);

  if (! line.skipSpaceAndChar('('))
    return;

  std::vector<std::string> args;

  pos = line.pos();

  if (! line.skipCIdentifier())
    return;

  std::string arg = line.substr(pos, line.pos() - pos);

  args.push_back(arg);

  while (line.skipSpaceAndChar(',')) {
    pos = line.pos();

    if (! line.skipCIdentifier())
      return;

    std::string arg1 = line.substr(pos, line.pos() - pos);

    args.push_back(arg1);
  }

  if (! line.skipSpaceAndChar(')'))
    return;

  CExpr *expr = window_->app()->expr();

  expr->addFunction(identifier, args, exprStr);

  //---

  updateFunctions();
}

QSize
CQGnuPlotManageFunctionsDialog::
sizeHint() const
{
  return size_;
}
