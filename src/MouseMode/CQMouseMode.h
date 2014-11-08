#ifndef CQMOUSE_MODE_H
#define CQMOUSE_MODE_H

#include <QObject>
#include <QPoint>
#include <QCursor>
#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTransform>

#include <cassert>

class QMenu;
class CQMenu;
class CQMenuItem;
class QPainter;
class CQToolBar;

class CQMouseModeIFace {
 public:
  CQMouseModeIFace() { }

  virtual void redraw() = 0;
  virtual void redrawOverlay() = 0;

  virtual QPointF pixelToWindow(const QPoint &p) = 0;

  virtual void setCursor(QCursor cursor) = 0;
};

class CQMouseMode;

#define CQMouseModeMgrInst CQMouseModeMgr::getInstance()

class CQMouseModeMgr {
 public:
  static CQMouseModeMgr *getInstance();

  void addMode(uint id, CQMouseMode *mode);

  CQMouseMode *getMode(uint id) const;

  void setCurrentMode(uint id);

  CQMouseMode *getCurrentMode() const;

  void createMenuItems(CQMenu *menu);
  void createMenuItems(QMenu *menu);

  void addToToolBar(CQToolBar *toolbar);

 private:
  CQMouseModeMgr();

 private:
  typedef std::map<uint,CQMouseMode *> ModeMap;

  ModeMap modeMap_;
  uint    currentId_;
};

class CQMouseMode : public QObject {
  Q_OBJECT

 public:
  struct MouseEvent {
    QPoint          pixel;
    QPointF         window;
    Qt::MouseButton button;

    MouseEvent(QMouseEvent *e, const QTransform &itransform) {
      pixel  = e->pos();
      window = itransform.map(QPointF(pixel));
      button = e->button();
    }
  };

 public:
  CQMouseMode(uint id, CQMouseModeIFace *iface);

  virtual ~CQMouseMode() { }

  virtual const char *getTitle() const = 0;

  uint getId() const { return id_; }

  QAction *getAction() const { return action_; }

  virtual CQMenuItem *createMenuItem(CQMenu *) { assert(false); return 0; }

  virtual QAction *createMenuItem(QMenu *) { assert(false); return 0; }

  void mousePress  (const MouseEvent &e);
  void mouseRelease(const MouseEvent &e);
  void mouseDrag   (const MouseEvent &e);
  void mouseMove   (const MouseEvent &e);

  void keyPress(QKeyEvent *e);

  virtual void handleMousePress  (const MouseEvent &e) = 0;
  virtual void handleMouseRelease(const MouseEvent &e) = 0;
  virtual void handleMouseDrag   (const MouseEvent &e) = 0;
  virtual void handleMouseMove   (const MouseEvent &e) = 0;

  virtual void handleKeyPress(QKeyEvent *e) = 0;

  virtual void accept() { }
  virtual void cancel() { }

  virtual void escape();

  virtual void drawOverlay(QPainter *painter);

  virtual QCursor getCursor() const = 0;

  void setOverlayColor(QColor c) { overlayColor_ = c; }

 public slots:
  void menuItemSlot(bool);

 protected:
  uint              id_;
  CQMouseModeIFace *iface_;
  QAction          *action_;
  bool              pressed_;
  Qt::MouseButton   button_;
  bool              moving_;
  QPoint            press_ppos_;
  QPointF           press_wpos_;
  QPoint            prev_ppos_;
  QPointF           prev_wpos_;
  QPoint            curr_ppos_;
  QPointF           curr_wpos_;
  QColor            overlayColor_;
};

#endif
