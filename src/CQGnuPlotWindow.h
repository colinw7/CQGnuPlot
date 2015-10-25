#ifndef CQGnuPlotWindow_H
#define CQGnuPlotWindow_H

#include <CQGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotTipData.h>

#include <QMainWindow>

class CQGnuPlot;
class CQGnuPlotGroup;
class CQGnuPlotPlot;
class CQGnuPlotCanvas;
class CQGnuPlotRenderer;
class CQPropertyTree;
class CQPropertyRealEditor;
class CQPropertyIntegerEditor;
class CQZoomRegion;
class CQCursor;

class QLabel;
class QTimer;
class QLineEdit;

class CQGnuPlotWindow : public CGnuPlotWindow {
 public:
  typedef std::vector<QObject *> Objects;

 public:
  CQGnuPlotWindow(CQGnuPlot *plot=0);

  void setApp(CQGnuPlot *plot);
  CQGnuPlot *qapp() const { return plot_; }

  virtual void redraw() { }

  virtual void selectObjects(const Objects &) { }

  void highlightObject(QObject *obj);

  virtual void paint(QPainter *) { }

  virtual int pixelWidth () const { return 100; }
  virtual int pixelHeight() const { return 100; }

  virtual void updateProperties() { }

  virtual CQGnuPlotGroup *getGroupAt(const QPoint &) { return 0; }

  virtual void setCurrentGroup(CQGnuPlotGroup *) { }

  virtual void mousePress  (const QPoint &) { }
  virtual void mouseMove   (const QPoint &, bool) { }
  virtual void mouseRelease(const QPoint &) { }

  virtual void keyPress(int, Qt::KeyboardModifiers) { }

  virtual bool mouseTip(const QPoint &, CGnuPlotTipData &) { return false; }

  virtual void showPos(const QString &, double, double, double, double) { }

 protected:
  CQGnuPlot* plot_ { 0 };
};

//---

class CQGnuPlotMainWindow : public QMainWindow, public CQGnuPlotWindow {
  Q_OBJECT

  Q_PROPERTY(int    cursorSize      READ cursorSize      WRITE setCursorSize)
  Q_PROPERTY(bool   tipOutside      READ isTipOutside    WRITE setTipOutside)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

 public:
  enum class Mode {
    SELECT,
    MOVE,
    ZOOM
  };

 public:
  CQGnuPlotMainWindow(CQGnuPlot *plot);
 ~CQGnuPlotMainWindow() override;

  CQGnuPlotCanvas *canvas() const { return canvas_; }

  void setSize(const CISize2D &s);

  void updateProperties();
  void addGroupProperties(CGnuPlotGroup *group);
  void addPlotProperties(CGnuPlotPlot *plot);

  void paint(QPainter *p);

  int pixelWidth () const;
  int pixelHeight() const;

  void showPos(const QString &name, double px, double py, double wx, double wy);

  int cursorSize() const;
  void setCursorSize(int s);

  bool isTipOutside() const { return tipOutside_; }
  void setTipOutside(bool b) { tipOutside_ = b; }

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &c);

  CQGnuPlotGroup *currentGroup() const { return currentGroup_; }
  void setCurrentGroup(CQGnuPlotGroup *group);

  CQGnuPlotGroup *getGroupAt(const QPoint &pos);

  bool isShowPixels() const { return showPixels_; }
  void setShowPixels(bool b) { showPixels_ = b; }

  void mousePress  (const QPoint &qp);
  void mouseMove   (const QPoint &qp, bool pressed);
  void mouseRelease(const QPoint &qp);

  void keyPress(int key, Qt::KeyboardModifiers modifiers);

  bool mouseTip(const QPoint &qp, CGnuPlotTipData &tip);

  void selectObject(const QObject *);

  void selectObjects(const Objects &objs);

  void deselectAllObjects();

  void redraw();

 public slots:
  void addProperties();

  void searchProperties();

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
  void savePNG();

  void xAxisSlot(bool show);
  void yAxisSlot(bool show);
  void gridSlot(bool show);

  void pixelSlot(bool show);

  void selectMode(bool b);
  void moveMode  (bool b);
  void zoomMode  (bool b);

  void itemSelectedSlot(QObject *obj, const QString &path);

 private:
  typedef std::map<std::string,CQPropertyRealEditor *>    RealEdits;
  typedef std::map<std::string,CQPropertyIntegerEditor *> IntegerEdits;
  typedef std::map<std::string,CQPropertyRealEditor *>    RealSliders;

  static uint lastId;

  uint               id_           { 0 };
  CQGnuPlotRenderer* renderer_     { 0 };
  CQGnuPlotCanvas*   canvas_       { 0 };
  CQPropertyTree*    tree_         { 0 };
  QLineEdit*         edit_         { 0 };
  RealEdits          redits_;
  IntegerEdits       iedits_;
  RealSliders        rsliders_;
  QTimer*            propTimer_    { 0 };
  QLabel*            plotLabel_    { 0 };
  QLabel*            posLabel_     { 0 };
  CQGnuPlotGroup*    currentGroup_ { 0 };
  Mode               mode_         { Mode::SELECT };
  bool               showPixels_   { false };
  QAction*           selectAction_ { 0 };
  QAction*           moveAction_   { 0 };
  QAction*           zoomAction_   { 0 };
  CQZoomRegion*      zoomRegion_   { 0 };
  QPoint             zoomPress_    { 0, 0 };
  QPoint             zoomRelease_  { 0, 0 };
  CQGnuPlotGroup*    zoomGroup_    { 0 };
  CQCursor*          cursor_       { 0 };
  bool               tipOutside_   { false };
  mutable bool       escape_       { false };
};

#endif
