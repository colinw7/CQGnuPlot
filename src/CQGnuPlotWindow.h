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
class CQGnuPlotFill;
class CQGnuPlotStroke;
class CQPropertyTree;
class CQPropertyRealEditor;
class CQPropertyIntegerEditor;
class CQGnuPlotDataDialog;
class CQGnuPlotCreateDialog;
class CQGnuPlotManageFunctionsDialog;
class CQGnuPlotManageVariablesDialog;
class CQGnuPlotPaletteDialog;
class CQZoomRegion;
class CQCursor;
class CQRubberBand;
class CQFloatLabel;

class QLabel;
class QTimer;
class QLineEdit;

class CQGnuPlotWindow : public CGnuPlotWindow {
 public:
  typedef std::vector<QObject *> Objects;

  struct ProbeWidget {
    CQRubberBand* line { 0 };
    CQFloatLabel* tip  { 0 };

    ProbeWidget(CQGnuPlotCanvas *canvas);
   ~ProbeWidget();
  };

  typedef std::vector<ProbeWidget *> ProbeWidgets;

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

  virtual CQGnuPlotGroup *getGroupAt(const CPoint2D &) { return 0; }

  virtual void setCurrentGroup(CQGnuPlotGroup *) { }

  virtual void mousePress  (const CGnuPlotMouseEvent &) { }
  virtual void mouseMove   (const CGnuPlotMouseEvent &, bool) { }
  virtual void mouseRelease(const CGnuPlotMouseEvent &) { }

  virtual void mouseWheel(double) { }

  virtual bool mouseTip(const CGnuPlotMouseEvent &, CGnuPlotTipData &) { return false; }

  virtual void keyPress(const CGnuPlotKeyEvent &) { }

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
    ZOOM,
    PROBE
  };

 public:
  CQGnuPlotMainWindow(CQGnuPlot *plot);
 ~CQGnuPlotMainWindow() override;

  CQGnuPlotCanvas *canvas() const { return canvas_; }

  void setSize(const CISize2D &s);

  void updateProperties() override;
  void addGroupProperties(CGnuPlotGroup *group);
  void addPlotProperties(CGnuPlotPlot *plot);
  void addFillProperties(const QString &name, CQGnuPlotFill *qfill);
  void addStrokeProperties(const QString &name, CQGnuPlotStroke *qstroke);

  void paint(QPainter *p) override;

  int pixelWidth () const override;
  int pixelHeight() const override;

  void showPos(const QString &name, double px, double py, double wx, double wy) override;

  int cursorSize() const;
  void setCursorSize(int s);

  bool isTipOutside() const { return tipOutside_; }
  void setTipOutside(bool b) { tipOutside_ = b; }

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &c);

  void initCurrentGroup();

  CQGnuPlotGroup *createTiledGroup();

  CQGnuPlotGroup *currentGroup() const { return currentGroup_; }
  void setCurrentGroup(CQGnuPlotGroup *group) override;

  CQGnuPlotGroup *getGroupAt(const CPoint2D &pos) override;

  bool isShowPixels() const { return showPixels_; }
  void setShowPixels(bool b) { showPixels_ = b; }

  void mousePress  (const CGnuPlotMouseEvent &mouseEvent) override;
  void mouseMove   (const CGnuPlotMouseEvent &mouseEvent, bool pressed) override;
  void mouseRelease(const CGnuPlotMouseEvent &mouseEvent) override;

  void mouseWheel(double d) override;

  bool mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip) override;

  void keyPress(const CGnuPlotKeyEvent &keyEvent) override;

  void selectObject(const QObject *);

  void selectObjects(const Objects &objs) override;

  void deselectAllObjects();

  void redraw() override;

 public slots:
  void addProperties();

  void searchProperties();

 private:
  void scrollLeftRightKey(CQGnuPlotGroup *group, bool left);
  void scrollUpDownKey(CQGnuPlotGroup *group, bool down);

  void zoomInKey (CQGnuPlotGroup *group);
  void zoomOutKey(CQGnuPlotGroup *group);

  void paintPlot(CGnuPlotPlot *plot);

  void setCursor(QCursor cursor);

  QPointF pixelToWindow(const QPoint &p);

  void pixelToWindow(double px, double py, double *wx, double *wy);

  void updateMode(Mode mode);

  CQPropertyRealEditor    *realEdit   (const std::string &str);
  CQPropertyIntegerEditor *integerEdit(const std::string &str);
  CQPropertyRealEditor    *realSlider (const std::string &str);

 private slots:
  void newWindow();

  void loadFile();
  void loadFileSlot();

  void loadFunction();
  void loadFunctionSlot();

  void showData();

  void manageFunctions();
  void manageVariables();

  void editColorPalette();
  void applyPaletteSlot();

  void createObjects();
  void createObjectsSlot();

  void saveSlot();
  void saveAcceptSlot();

  void saveSVG(const QString &filename, int w, int h);
  void savePS (const QString &filename, int w, int h);
  void savePNG(const QString &filename, int w, int h);

  void saveDevice(const QString &filename, const QString &deviceName, int w, int h);

  void xAxisSlot(bool show);
  void yAxisSlot(bool show);
  void gridSlot(bool show);

  void pixelSlot(bool show);

  void selectMode(bool b);
  void moveMode  (bool b);
  void zoomMode  (bool b);
  void probeMode (bool b);

  void pointsSlot();

  void itemSelectedSlot(QObject *obj, const QString &path);

  void treeMenuExec(QObject *obj, const QPoint &gpos);

 private:
  typedef std::map<std::string,CQPropertyRealEditor *>    RealEdits;
  typedef std::map<std::string,CQPropertyIntegerEditor *> IntegerEdits;
  typedef std::map<std::string,CQPropertyRealEditor *>    RealSliders;

  static uint lastId;

  uint                            id_               { 0 };
  CQGnuPlotRenderer*              renderer_         { 0 };
  CQGnuPlotCanvas*                canvas_           { 0 };
  CQPropertyTree*                 tree_             { 0 };
  CQGnuPlotDataDialog*            dataDialog_       { 0 };
  CQGnuPlotCreateDialog*          createDialog_     { 0 };
  CQGnuPlotManageFunctionsDialog* manageFnsDialog_  { 0 };
  CQGnuPlotManageVariablesDialog* manageVarsDialog_ { 0 };
  CQGnuPlotPaletteDialog*         paletteDialog_    { 0 };
  QLineEdit*                      edit_             { 0 };
  RealEdits                       redits_;
  IntegerEdits                    iedits_;
  RealSliders                     rsliders_;
  QTimer*                         propTimer_        { 0 };
  QLabel*                         plotLabel_        { 0 };
  QLabel*                         posLabel_         { 0 };
  CQGnuPlotGroup*                 currentGroup_     { 0 };
  Mode                            mode_             { Mode::SELECT };
  bool                            showPixels_       { false };
  QAction*                        selectAction_     { 0 };
  QAction*                        moveAction_       { 0 };
  QAction*                        zoomAction_       { 0 };
  QAction*                        probeAction_      { 0 };
  QAction*                        pointsAction_     { 0 };
  CQZoomRegion*                   zoomRegion_       { 0 };
  CPoint2D                        zoomPress_        { 0, 0 };
  CPoint2D                        zoomRelease_      { 0, 0 };
  CQGnuPlotGroup*                 zoomGroup_        { 0 };
  CQCursor*                       cursor_           { 0 };
  ProbeWidgets                    probeWidgets_     { 0 };
  bool                            tipOutside_       { false };
  mutable bool                    escape_           { false };
};

#endif
