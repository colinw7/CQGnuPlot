#include <CQGnuPlotFilename.h>
#include <CQPixmapCache.h>

#include <QFileDialog>
#include <QDir>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>

CQGnuPlotFilename::
CQGnuPlotFilename(QWidget *parent) :
 QFrame(parent), save_(false), pattern_("*.*")
{
  QHBoxLayout *layout = new QHBoxLayout(this);

  edit_   = new QLineEdit;
  button_ = new QToolButton;

  button_->setIcon(CQPixmapCacheInst->getIcon("FILE_DIALOG"));

  layout->addWidget(edit_);
  layout->addWidget(button_);

  connect(button_, SIGNAL(clicked()), this, SLOT(fileSlot()));
}

void
CQGnuPlotFilename::
fileSlot()
{
  QString dir = QDir::current().dirName();

  QString fileName;

  if (isSave()) {
    fileName = QFileDialog::getSaveFileName(this, "Save File", dir, "Files (" + pattern_ + ")");
  }
  else {
    fileName = QFileDialog::getOpenFileName(this, "Open File", dir, "Files (" + pattern_ + ")");
  }

  if (! fileName.length())
    return;

  edit_->setText(fileName);
}

QString
CQGnuPlotFilename::
name() const
{
  return edit_->text();
}
