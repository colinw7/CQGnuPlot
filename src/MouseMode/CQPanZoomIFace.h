#ifndef CQPAN_ZOOM_IFACE_H
#define CQPAN_ZOOM_IFACE_H

#include <CBBox2D.h>

class CQPanZoomIFace {
 public:
  CQPanZoomIFace() { }

  // Pan
  virtual void panBy(double dx, double dy) = 0;
  virtual void panTo(const CPoint2D &c) = 0;

  virtual void panLeft () = 0;
  virtual void panRight() = 0;
  virtual void panUp   () = 0;
  virtual void panDown () = 0;
  virtual void resetPan() = 0;

  // Zoom
  virtual void zoomTo (const CBBox2D &bbox) = 0;
  virtual void zoomIn (const CPoint2D &c) = 0;
  virtual void zoomOut(const CPoint2D &c) = 0;

  virtual void zoomIn   () = 0;
  virtual void zoomOut  () = 0;
  virtual void resetZoom() = 0;
};

#endif
