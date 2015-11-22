#ifndef CGnuPlotDevice_H
#define CGnuPlotDevice_H

#include <CISize2D.h>
#include <CFont.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotAxis.h>

#include <string>
#include <memory>

class CParseLine;

class CGnuPlot;
class CGnuPlotArrow;
class CGnuPlotAxis;
class CGnuPlotCamera;
class CGnuPlotCircle;
class CGnuPlotColorBox;
class CGnuPlotEllipse;
class CGnuPlotGroup;
class CGnuPlotKey;
class CGnuPlotLabel;
class CGnuPlotLineStyle;
class CGnuPlotPalette;
class CGnuPlotPlot;
class CGnuPlotPm3D;
class CGnuPlotPolygon;
class CGnuPlotRectangle;
class CGnuPlotTimeStamp;

class CGnuPlotArrowObject;
class CGnuPlotBoxBarObject;
class CGnuPlotEndBar;
class CGnuPlotBoxObject;
class CGnuPlotBubbleObject;
class CGnuPlotEllipseObject;
class CGnuPlotErrorBarObject;
class CGnuPlotFinanceBarObject;
class CGnuPlotImageObject;
class CGnuPlotLabelObject;
class CGnuPlotPathObject;
class CGnuPlotPieObject;
class CGnuPlotPointObject;
class CGnuPlotPolygonObject;
class CGnuPlotRectObject;

class CGnuPlotFill;
class CGnuPlotStroke;
class CGnuPlotMark;

class CGnuPlotRenderer;
class CGnuPlotTitle;
class CGnuPlotWindow;
class CGnuPlotMultiplot;

typedef std::shared_ptr<CGnuPlotWindow> CGnuPlotWindowP;

class CGnuPlotDevice {
 public:
  typedef std::vector<CGnuPlotWindowP> Windows;
  typedef CGnuPlotTypes::AxisType      AxisType;

 public:
  CGnuPlotDevice(const std::string &name);

  virtual ~CGnuPlotDevice() { }

  CGnuPlot *app() const { return plot_; }
  void setApp(CGnuPlot *plot) { plot_ = plot; }

  const std::string &name() const { return name_; }

  const CISize2D &size() const { return size_; }
  void setSize(const CISize2D &s) { size_ = s; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &v) { font_ = v; }

  double fontScale() const { return fontScale_; }
  void setFontScale(double r) { fontScale_ = r; }

  bool isDashed() const { return dashed_; }
  void setDashed(bool b) { dashed_ = b; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  CGnuPlotWindowP multiWindow() const { return multiWindow_; }
  void setMultiWindow(const CGnuPlotWindowP &w) { multiWindow_ = w; }
  void resetMultiWindow() { multiWindow_ = 0; }

  const Windows &windows() const { return windows_; }

  void addWindow(CGnuPlotWindowP window) {
    assert(window);

    windows_.push_back(window);
  }

  void removeWindow(CGnuPlotWindow *window) {
    assert(window);

    Windows windows;

    for (auto &w : windows_) {
      if (w.get() != window)
        windows.push_back(w);
    }

    windows_ = windows;
  }

  virtual void redraw() { }

  virtual CGnuPlotWindow *createWindow();

  virtual CGnuPlotMultiplot *createMultiplot();

  virtual CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  virtual CGnuPlotPlot *createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle plotStyle);

  virtual CGnuPlotLineStyle *createLineStyle(CGnuPlot *plot);

  virtual CGnuPlotArrow     *createArrow(CGnuPlotGroup *group);
  virtual CGnuPlotCircle    *createCircle(CGnuPlotGroup *group);
  virtual CGnuPlotEllipse   *createEllipse(CGnuPlotGroup *group);
  virtual CGnuPlotLabel     *createLabel(CGnuPlotGroup *group);
  virtual CGnuPlotPolygon   *createPolygon(CGnuPlotGroup *group);
  virtual CGnuPlotRectangle *createRectangle(CGnuPlotGroup *group);

  virtual CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data);

  virtual CGnuPlotKey *createKey(CGnuPlotGroup *group);

  virtual CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group);

  virtual CGnuPlotPalette *createPalette(CGnuPlotGroup *group);

  virtual CGnuPlotTitle *createTitle(CGnuPlotGroup *group);

  virtual CGnuPlotCamera *createCamera(CGnuPlotGroup *group);

  virtual CGnuPlotPm3D *createPm3D(CGnuPlotGroup *group);

  virtual CGnuPlotTimeStamp *createTimeStamp(CGnuPlotGroup *group);

  virtual CGnuPlotArrowObject      *createArrowObject     (CGnuPlotPlot *plot);
  virtual CGnuPlotBoxBarObject     *createBoxBarObject    (CGnuPlotPlot *plot);
  virtual CGnuPlotEndBar           *createEndBar          (CGnuPlotPlot *plot);
  virtual CGnuPlotBoxObject        *createBoxObject       (CGnuPlotPlot *plot);
  virtual CGnuPlotBubbleObject     *createBubbleObject    (CGnuPlotPlot *plot);
  virtual CGnuPlotEllipseObject    *createEllipseObject   (CGnuPlotPlot *plot);
  virtual CGnuPlotErrorBarObject   *createErrorBarObject  (CGnuPlotPlot *plot);
  virtual CGnuPlotImageObject      *createImageObject     (CGnuPlotPlot *plot);
  virtual CGnuPlotFinanceBarObject *createFinanceBarObject(CGnuPlotPlot *plot);
  virtual CGnuPlotLabelObject      *createLabelObject     (CGnuPlotPlot *plot);
  virtual CGnuPlotPathObject       *createPathObject      (CGnuPlotPlot *plot);
  virtual CGnuPlotPieObject        *createPieObject       (CGnuPlotPlot *plot);
  virtual CGnuPlotPointObject      *createPointObject     (CGnuPlotPlot *plot);
  virtual CGnuPlotPolygonObject    *createPolygonObject   (CGnuPlotPlot *plot);
  virtual CGnuPlotRectObject       *createRectObject      (CGnuPlotPlot *plot);

  virtual CGnuPlotFill   *createFill  (CGnuPlotPlot *plot);
  virtual CGnuPlotStroke *createStroke(CGnuPlotPlot *plot);
  virtual CGnuPlotMark   *createMark  (CGnuPlotPlot *plot);

  virtual void timeout() { }

  virtual void waitForMouse(int) { }

  virtual void stateChanged(CGnuPlotWindow *, CGnuPlotTypes::ChangeState) { }

  virtual void drawInit(CGnuPlotWindow *) { }
  virtual void drawTerm() { }

  virtual CGnuPlotRenderer *renderer() = 0;

  virtual bool parseArgs(CParseLine &);

  virtual void show(std::ostream &os) const;

 protected:
  CGnuPlot*       plot_      { 0 };
  std::string     name_;
  CISize2D        size_      { 600, 480 };
  bool            enhanced_  { true };
  CFontPtr        font_;
  double          fontScale_ { 1 };
  bool            dashed_    { false };
  bool            lineWidth_ { 0 };
  Windows         windows_;
  CGnuPlotWindowP multiWindow_;
};

#endif
