#ifndef CQCursor_H
#define CQCursor_H

#include <QCursor>
#include <QIcon>

class CQCursor {
 public:
  enum Type {
    SHAPE,
    BITMAP,
    PIXMAP,
    ICON
  };

 public:
  CQCursor(Qt::CursorShape shape=Qt::ArrowCursor);
  CQCursor(const QBitmap &bitmap, const QBitmap &mask, int hotX=-1, int hotY=-1);
  CQCursor(const QPixmap &pixmap, int hotX=-1, int hotY=-1);
  CQCursor(const QIcon &icon, int hotX=-1, int hotY=-1);
  CQCursor(const QString &filename, int hotX=-1, int hotY=-1);

  const QCursor &cursor() const;

  void reset() { set_ = false; }

 private:
  Type    type_;
  QCursor cursor_;
  QIcon   icon_;
  int     hotX_;
  int     hotY_;
  bool    set_;
};

#define CQCursorMgrInst CQCursorMgr::instance()

class CQCursorMgr {
 public:
  static CQCursorMgr *instance() {
    static CQCursorMgr *mgr;

    if (! mgr)
      mgr = new CQCursorMgr;

    return mgr;
  }

  CQCursor *createCursor(Qt::CursorShape shape=Qt::ArrowCursor) {
    return addCursor(new CQCursor(shape));
  }

  CQCursor *createCursor(const QBitmap &bitmap, const QBitmap &mask, int hotX=-1, int hotY=-1) {
    return addCursor(new CQCursor(bitmap, mask, hotX, hotY));
  }

  CQCursor *createCursor(const QPixmap &pixmap, int hotX=-1, int hotY=-1) {
    return addCursor(new CQCursor(pixmap, hotX, hotY));
  }

  CQCursor *createCursor(const QIcon &icon, int hotX=-1, int hotY=-1) {
    return addCursor(new CQCursor(icon, hotX, hotY));
  }

  CQCursor *createCursor(const QString &filename, int hotX=-1, int hotY=-1) {
    CQCursor *cursor = findNamedCursor(filename);

    if (! cursor)
      cursor = addNamedCursor(filename, new CQCursor(filename, hotX, hotY));

    return cursor;
  }

  int size() const { return size_; }

  void setSize(int s) {
    size_ = s;

    for (const auto &c : cursors_)
      c->reset();
  }

 private:
  CQCursorMgr() :
   size_(32) {
  }

  CQCursor *findNamedCursor(const QString &name) const {
    auto p = namedCursors_.find(name);
    if (p == namedCursors_.end()) return 0;

    return (*p).second;
  }

  CQCursor *addNamedCursor(const QString &name, CQCursor *cursor) {
    namedCursors_[name] = cursor;

    return addCursor(cursor);
  }

  CQCursor *addCursor(CQCursor *cursor) {
    cursors_.push_back(cursor);

    return cursor;
  }

 private:
  typedef std::vector<CQCursor *>      Cursors;
  typedef std::map<QString,CQCursor *> NamedCursors;

  Cursors      cursors_;
  NamedCursors namedCursors_;
  int          size_;
};

#endif
