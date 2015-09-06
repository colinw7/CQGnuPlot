#include <CQCursor.h>
#include <QFileInfo>

CQCursor::
CQCursor(Qt::CursorShape shape) :
 type_(SHAPE), cursor_(shape), set_(true)
{
}

CQCursor::
CQCursor(const QBitmap &bitmap, const QBitmap &mask, int hotX, int hotY) :
 type_(BITMAP), cursor_(bitmap, mask, hotX, hotY), hotX_(hotX), hotY_(hotY), set_(true)
{
}

CQCursor::
CQCursor(const QPixmap &pixmap, int hotX, int hotY) :
 type_(PIXMAP), cursor_(pixmap, hotX, hotY), hotX_(hotX), hotY_(hotY), set_(true)
{
}

CQCursor::
CQCursor(const QIcon &icon, int hotX, int hotY) :
 type_(ICON), cursor_(QPixmap(), hotX, hotY), icon_(icon), hotX_(hotX), hotY_(hotY), set_(false)
{
}

CQCursor::
CQCursor(const QString &filename, int hotX, int hotY) :
 type_(SHAPE), cursor_(Qt::ArrowCursor), hotX_(hotX), hotY_(hotY)
{
  QString rootDir;

  if (getenv("DEV_DIR")) {
    rootDir = getenv("DEV_DIR");

    rootDir += "/cursors";
  }
  else
    rootDir = ".";

  QString pathName = rootDir + "/" + filename;

  if (! QFileInfo(pathName).exists())
    return;

  icon_ = QIcon(pathName);

  if (icon_.isNull())
    return;

  type_ = ICON;
  set_  = false;
}

const QCursor &
CQCursor::
cursor() const
{
  if (! set_) {
    CQCursor *th = const_cast<CQCursor *>(this);

    if (type_ == ICON) {
      int s = CQCursorMgrInst->size();

      th->cursor_ = QCursor(icon_.pixmap(QSize(s, s)), hotX_, hotY_);

      th->set_ = true;
    }
  }

  return cursor_;
}
