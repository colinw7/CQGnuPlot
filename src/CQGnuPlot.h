#include <CGnuPlot.h>

#include <QMainWindow>
#include <CQMouseMode.h>
#include <CQPanZoomIFace.h>

class CQGnuPlotWindow;
class CQGnuPlotRenderer;
class CQPropertyTree;

class CQGnuPlotCanvas : public QWidget {
  Q_OBJECT

 public:
  CQGnuPlotCanvas(CQGnuPlotWindow *window);
 ~CQGnuPlotCanvas();

 private:
  void paintEvent(QPaintEvent *);

  void mouseMoveEvent(QMouseEvent *);

 private:
  CQGnuPlotWindow *window_;
};

class CQGnuPlot : public CGnuPlot {
 public:
  CQGnuPlot();
 ~CQGnuPlot();

  CGnuPlotWindow *createWindow();

  void timeout();

  void load(const std::string &filename);
};

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

  Q_PROPERTY(double marginLeft   READ marginLeft   WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight  WRITE setMarginRight )
  Q_PROPERTY(double marginTop    READ marginTop    WRITE setMarginTop   )
  Q_PROPERTY(double marginBottom READ marginBottom WRITE setMarginBottom)

  Q_PROPERTY(bool hidden3D          READ hidden3D          WRITE setHidden3D         )
  Q_PROPERTY(bool surface3D         READ surface3D         WRITE setSurface3D        )
  Q_PROPERTY(bool contour3D         READ contour3D         WRITE setContour3D        )
  Q_PROPERTY(int  trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

 public:
  CQGnuPlotWindow(CQGnuPlot *plot);
 ~CQGnuPlotWindow();

  CQGnuPlotCanvas *canvas() const { return canvas_; }

  void addPropeties(CQPropertyTree *tree);

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

 private:
  void stateChanged(CGnuPlot::ChangeState);

  void paintPlot(CGnuPlot::Plot *plot);

  void drawPoint(QPainter *p, const CGnuPlot::Point &point);
  void drawLine(QPainter *p, const CGnuPlot::Point &point1, const CGnuPlot::Point &point2);

  // Mouse Mode
  void redraw();
  void redrawOverlay();

  QPointF pixelToWindow(const QPoint &p);

  void setCursor(QCursor cursor);

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

 private slots:
  void xAxisSlot(bool show);
  void yAxisSlot(bool show);

 private:
  static uint lastId;

  uint               id_;
  CQGnuPlot         *plot_;
  CQGnuPlotCanvas   *canvas_;
  CQGnuPlotRenderer *renderer_;
  CQMouseMode       *zoomMode_;
  CQMouseMode       *panMode_;
};
