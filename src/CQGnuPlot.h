#include <CGnuPlot.h>

#include <QMainWindow>
#include <CQMouseMode.h>
#include <CQPanZoomIFace.h>

class CQGnuPlot;

class CQGnuPlotCanvas : public QWidget {
  Q_OBJECT

 public:
  CQGnuPlotCanvas(CQGnuPlot *plot);

 private:
  void paintEvent(QPaintEvent *);

 private:
  CQGnuPlot *plot_;
};

class CQGnuPlot : public QMainWindow, public CGnuPlot,
                  public CQMouseModeIFace, public CQPanZoomIFace {
  Q_OBJECT

 public:
  CQGnuPlot();

  CQGnuPlotCanvas *canvas() const { return canvas_; }

  void load(const std::string &filename);

  void paint(QPainter *p);

 private:
  void timeout();

  void stateChanged(CGnuPlot::ChangeState);

  void paintPlot(QPainter *p, CGnuPlot::Plot *plot);

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
  CQGnuPlotCanvas *canvas_;
  CQMouseMode     *zoomMode_;
  CQMouseMode     *panMode_;
};
