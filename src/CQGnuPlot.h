#include <CGnuPlot.h>

#include <QMainWindow>
#include <CQMouseMode.h>
#include <CQPanZoomIFace.h>

class CQGnuPlotWindow;
class CQGnuPlotRenderer;

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

 public:
  CQGnuPlotWindow(CQGnuPlot *plot);
 ~CQGnuPlotWindow();

  CQGnuPlotCanvas *canvas() const { return canvas_; }

  void paint(QPainter *p);

  void showPos(double wx, double wy);

  uint getZoomModeId() const { return ZOOM_MOUSE_MODE_ID + 1000*id_; }
  uint getPanModeId () const { return PAN_MOUSE_MODE_ID  + 1000*id_; }

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
