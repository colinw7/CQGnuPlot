#ifndef CQGnuPlotWindow_H
#define CQGnuPlotWindow_H

#include <CQGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotSVGRenderer.h>

#include <QMainWindow>

class CQGnuPlot;
class CQGnuPlotGroup;
class CQGnuPlotPlot;
class CQGnuPlotCanvas;
class CQGnuPlotRenderer;
class CQGnuPlotObject;
class CQPropertyTree;
class CQPropertyRealEditor;
class CQPropertyIntegerEditor;
class CQZoomRegion;

class QLabel;
class QTimer;

class CQGnuPlotWindow : public QMainWindow, public CGnuPlotWindow {
  Q_OBJECT

  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

  Q_PROPERTY(bool hidden3D READ hidden3D WRITE setHidden3D)

 public:
  enum class Mode {
    SELECT,
    ZOOM
  };

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

  void showPos(const QString &name, double wx, double wy);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &c);

  CQGnuPlotGroup *currentGroup() const { return currentGroup_; }
  void setCurrentGroup(CQGnuPlotGroup *group);

  CQGnuPlotGroup *getGroupAt(const QPoint &pos);

  void mousePress  (const QPoint &qp);
  void mouseMove   (const QPoint &qp, bool pressed);
  void mouseRelease(const QPoint &qp);

  void keyPress(int key, Qt::KeyboardModifiers modifiers);

  bool mouseTip  (const QPoint &qp, CQGnuPlot::TipRect &tip);

  void selectObject(const QObject *);

  void selectObjects(const std::vector<CQGnuPlotObject *> &objs);

  void deselectAllObjects();

  void redraw();

 public slots:
  void addProperties();

 private:
  void paintPlot(CGnuPlotPlot *plot);

  void setCursor(QCursor cursor);

  QPointF pixelToWindow(const QPoint &p);

  void pixelToWindow(double px, double py, double *wx, double *wy);

  CQPropertyRealEditor    *realEdit   (const std::string &str);
  CQPropertyIntegerEditor *integerEdit(const std::string &str);
  CQPropertyRealEditor    *realSlider (const std::string &str);

 private slots:
  void saveSVG();

  void xAxisSlot(bool show);
  void yAxisSlot(bool show);
  void gridSlot(bool show);

  void selectMode(bool b);
  void zoomMode  (bool b);

  void itemSelectedSlot(QObject *obj, const QString &path);

 private:
  typedef std::map<std::string,CQPropertyRealEditor *>    RealEdits;
  typedef std::map<std::string,CQPropertyIntegerEditor *> IntegerEdits;
  typedef std::map<std::string,CQPropertyRealEditor *>    RealSliders;

  static uint lastId;

  uint               id_           { 0 };
  CQGnuPlot*         plot_         { 0 };
  CQGnuPlotRenderer* renderer_     { 0 };
  CQGnuPlotCanvas*   canvas_       { 0 };
  CQPropertyTree*    tree_         { 0 };
  RealEdits          redits_;
  IntegerEdits       iedits_;
  RealSliders        rsliders_;
  QTimer*            propTimer_    { 0 };
  QLabel*            plotLabel_    { 0 };
  QLabel*            posLabel_     { 0 };
  CQGnuPlotGroup*    currentGroup_ { 0 };
  Mode               mode_         { Mode::SELECT };
  QAction*           selectAction_ { 0 };
  QAction*           zoomAction_   { 0 };
  CQZoomRegion*      zoomRegion_   { 0 };
  QPoint             zoomPress_    { 0, 0 };
  QPoint             zoomRelease_  { 0, 0 };
  CQGnuPlotGroup*    zoomGroup_    { 0 };
  mutable bool       escape_       { false };
};

#endif
