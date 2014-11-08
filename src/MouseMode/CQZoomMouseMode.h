#ifndef CQ_ZOOM_MOUSE_MODE_H
#define CQ_ZOOM_MOUSE_MODE_H

#include <CQMouseMode.h>
#include <CQPanZoomIFace.h>
#include <CBBox2D.h>

#define ZOOM_MOUSE_MODE_ID 1001

class CQMenuItem;

class CQZoomMouseMode : public CQMouseMode {
  Q_OBJECT

 public:
  CQZoomMouseMode(CQMouseModeIFace *iface, CQPanZoomIFace *zoomIFace);

 ~CQZoomMouseMode() { }

  const char *getTitle() const { return "Zoom"; }

  CQMenuItem *createMenuItem(CQMenu *menu);

  QAction *createMenuItem(QMenu *menu);

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  void handleKeyPress(QKeyEvent *e);

  void accept();
  void cancel();

  void escape();

  void drawOverlay(QPainter *painter);

  QCursor getCursor() const;

 private:
  CQPanZoomIFace *zoomIFace_;
  QCursor         zoomCursor_;
};

#endif
