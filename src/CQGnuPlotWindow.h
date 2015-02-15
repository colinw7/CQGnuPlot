#ifndef CQGnuPlotWindow_H
#define CQGnuPlotWindow_H

#include <CQGnuPlot.h>
#include <CGnuPlotWindow.h>

#include <QMainWindow>
#include <CQMouseMode.h>
#include <CQPanZoomIFace.h>

class CQGnuPlot;
class CQGnuPlotGroup;
class CQGnuPlotPlot;
class CQGnuPlotCanvas;
class CQGnuPlotRenderer;
class CQPropertyTree;
class CQPropertyRealEditor;
class QLabel;
class QTimer;

class CQGnuPlotWindow : public QMainWindow, public CGnuPlotWindow,
                        public CQMouseModeIFace, public CQPanZoomIFace {
  Q_OBJECT

  Q_PROPERTY(bool   enable3D READ enable3D WRITE setEnable3D)
  Q_PROPERTY(double rotateX  READ rotateX  WRITE setRotateX)
  Q_PROPERTY(double rotateY  READ rotateY  WRITE setRotateY)
  Q_PROPERTY(double rotateZ  READ rotateZ  WRITE setRotateZ)
  Q_PROPERTY(double xmin3D   READ xmin3D   WRITE setXMin3D )
  Q_PROPERTY(double xmax3D   READ xmax3D   WRITE setXMax3D )
  Q_PROPERTY(double ymin3D   READ ymin3D   WRITE setYMin3D )
  Q_PROPERTY(double ymax3D   READ ymax3D   WRITE setYMax3D )
  Q_PROPERTY(double near3D   READ near3D   WRITE setNear3D )
  Q_PROPERTY(double far3D    READ far3D    WRITE setFar3D  )

  Q_PROPERTY(bool hidden3D  READ hidden3D  WRITE setHidden3D )
  Q_PROPERTY(bool surface3D READ surface3D WRITE setSurface3D)
  Q_PROPERTY(bool contour3D READ contour3D WRITE setContour3D)

 public:
  CQGnuPlotWindow(CQGnuPlot *plot);
 ~CQGnuPlotWindow() override;

  CQGnuPlot *qapp() const { return plot_; }

  CQGnuPlotCanvas *canvas() const { return canvas_; }

  void setSize(const CISize2D &s);

  void updateProperties();
  void addGroupProperties(CGnuPlotGroup *group);
  void addPlotProperties(CGnuPlotPlot *plot);

  void paint(QPainter *p);

  void showPos(double wx, double wy);

  uint getZoomModeId() const { return ZOOM_MOUSE_MODE_ID + 1000*id_; }
  uint getPanModeId () const { return PAN_MOUSE_MODE_ID  + 1000*id_; }

  bool enable3D() { return camera().enabled; }
  void setEnable3D(bool b);

  double rotateX() { return camera().rotateX; }
  double rotateY() { return camera().rotateY; }
  double rotateZ() { return camera().rotateZ; }

  void setRotateX(double a);
  void setRotateY(double a);
  void setRotateZ(double a);

  double xmin3D() { return camera().xmin; }
  double xmax3D() { return camera().xmax; }
  double ymin3D() { return camera().ymin; }
  double ymax3D() { return camera().ymax; }
  double near3D() { return camera().near; }
  double far3D () { return camera().far ; }

  void setXMin3D(double x);
  void setXMax3D(double x);
  void setYMin3D(double y);
  void setYMax3D(double y);
  void setNear3D(double z);
  void setFar3D (double z);

  CQGnuPlotGroup *currentGroup() const { return currentGroup_; }
  void setCurrentGroup(CQGnuPlotGroup *group);

  CQGnuPlotGroup *getGroupAt(const QPoint &pos);

  void mousePress(const QPoint &qp);
  void mouseMove (const QPoint &qp);
  bool mouseTip  (const QPoint &qp, CQGnuPlot::TipRect &tip);

  void selectObject(const QObject *);

  void redraw();

 public slots:
  void addProperties();

 private:
  void paintPlot(CGnuPlotPlot *plot);

  void redrawOverlay();

  void setCursor(QCursor cursor);

  // Mouse Mode

  // Pan
  void panBy(double dx, double dy);
  void panTo(const CPoint2D &c);

  void panLeft ();
  void panRight();
  void panUp   ();
  void panDown ();
  void resetPan();

  // Zoom
  void zoomTo (const CBBox2D &bbox);
  void zoomIn (const CPoint2D &c);
  void zoomOut(const CPoint2D &c);

  void zoomIn   ();
  void zoomOut  ();
  void resetZoom();

  QPointF pixelToWindow(const QPoint &p);

  void pixelToWindow(double px, double py, double *wx, double *wy);

 private slots:
  void xAxisSlot(bool show);
  void yAxisSlot(bool show);

  void itemSelectedSlot(QObject *obj, const QString &path);

 private:
  static uint lastId;

  uint                  id_           { 0 };
  CQGnuPlot            *plot_         { 0 };
  CQGnuPlotRenderer    *renderer_     { 0 };
  CQGnuPlotCanvas      *canvas_       { 0 };
  CQMouseMode          *zoomMode_     { 0 };
  CQMouseMode          *panMode_      { 0 };
  CQPropertyTree       *tree_         { 0 };
  CQPropertyRealEditor *redit_[4]     { 0, 0, 0, 0 };
  QTimer               *propTimer_    { 0 };
  QLabel               *plotLabel_    { 0 };
  QLabel               *posLabel_     { 0 };
  CQGnuPlotGroup       *currentGroup_ { 0 };
};

#endif
