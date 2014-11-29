#ifndef CQ_PAN_MOUSE_MODE_H
#define CQ_PAN_MOUSE_MODE_H

#include <CQMouseMode.h>
#include <CQPanZoomIFace.h>
#include <CBBox2D.h>

class CQMenuItem;

class CQPanMouseMode : public CQMouseMode {
  Q_OBJECT

 public:
  CQPanMouseMode(CQMouseModeIFace *iface, CQPanZoomIFace *panIFace);

 ~CQPanMouseMode() { }

  const char *getTitle() const { return "Pan"; }

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
  CQPanZoomIFace *panIFace_;
  QCursor         panCursor_;
};

#endif
