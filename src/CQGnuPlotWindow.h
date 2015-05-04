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
class CQGnuPlotObject;
class CQPropertyTree;
class CQPropertyRealEditor;
class CQPropertyIntegerEditor;
class QLabel;
class QTimer;

class CQGnuPlotWindow : public QMainWindow, public CGnuPlotWindow,
                        public CQMouseModeIFace, public CQPanZoomIFace {
  Q_OBJECT

  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

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

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &c);

  CQGnuPlotGroup *currentGroup() const { return currentGroup_; }
  void setCurrentGroup(CQGnuPlotGroup *group);

  CQGnuPlotGroup *getGroupAt(const QPoint &pos);

  void mousePress(const QPoint &qp);
  void mouseMove (const QPoint &qp);
  bool mouseTip  (const QPoint &qp, CQGnuPlot::TipRect &tip);

  void selectObject(const QObject *);

  void selectObjects(const std::vector<CQGnuPlotObject *> &objs);

  void deselectAllObjects();

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

  CQPropertyRealEditor *realEdit(const std::string &str);

 private slots:
  void xAxisSlot(bool show);
  void yAxisSlot(bool show);

  void itemSelectedSlot(QObject *obj, const QString &path);

 private:
  typedef std::map<std::string,CQPropertyRealEditor *> RealEdits;
  typedef std::vector<CQPropertyIntegerEditor *>       IntegerEdits;

  static uint lastId;

  uint                     id_           { 0 };
  CQGnuPlot               *plot_         { 0 };
  CQGnuPlotRenderer       *renderer_     { 0 };
  CQGnuPlotCanvas         *canvas_       { 0 };
  CQMouseMode             *zoomMode_     { 0 };
  CQMouseMode             *panMode_      { 0 };
  CQPropertyTree          *tree_         { 0 };
  RealEdits                redits_;      // TODO: lookup in cache
  IntegerEdits             iedits_;      // TODO: lookup in cache
  QTimer                  *propTimer_    { 0 };
  QLabel                  *plotLabel_    { 0 };
  QLabel                  *posLabel_     { 0 };
  CQGnuPlotGroup          *currentGroup_ { 0 };
};

#endif
