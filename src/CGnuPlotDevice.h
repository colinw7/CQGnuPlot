#ifndef CGnuPlotDevice_H
#define CGnuPlotDevice_H

#include <COrientation.h>
#include <CISize2D.h>
#include <CFont.h>
#include <CGnuPlotTypes.h>

#include <string>

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
class CGnuPlotPolygon;
class CGnuPlotRectangle;
class CGnuPlotBarObject;
class CGnuPlotRectObject;
class CGnuPlotBubbleObject;
class CGnuPlotPieObject;
class CGnuPlotRenderer;
class CGnuPlotTitle;
class CGnuPlotWindow;

class CGnuPlotDevice {
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

  virtual CGnuPlotWindow *createWindow();

  virtual CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  virtual CGnuPlotPlot *createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle plotStyle);

  virtual CGnuPlotLineStyle *createLineStyle();

  virtual CGnuPlotArrow     *createArrow(CGnuPlotGroup *group);
  virtual CGnuPlotCircle    *createCircle(CGnuPlotGroup *group);
  virtual CGnuPlotEllipse   *createEllipse(CGnuPlotGroup *group);
  virtual CGnuPlotLabel     *createLabel(CGnuPlotGroup *group);
  virtual CGnuPlotPolygon   *createPolygon(CGnuPlotGroup *group);
  virtual CGnuPlotRectangle *createRectangle(CGnuPlotGroup *group);

  virtual CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir);

  virtual CGnuPlotKey *createKey(CGnuPlotGroup *group);

  virtual CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group);

  virtual CGnuPlotPalette *createPalette(CGnuPlotGroup *group);

  virtual CGnuPlotTitle *createTitle(CGnuPlotGroup *group);

  virtual CGnuPlotCamera *createCamera(CGnuPlotGroup *group);

  virtual CGnuPlotBarObject    *createBarObject   (CGnuPlotPlot *plot);
  virtual CGnuPlotBubbleObject *createBubbleObject(CGnuPlotPlot *plot);
  virtual CGnuPlotPieObject    *createPieObject   (CGnuPlotPlot *plot);
  virtual CGnuPlotRectObject   *createRectObject  (CGnuPlotPlot *plot);

  virtual void timeout() { }

  virtual void stateChanged(CGnuPlotWindow *, CGnuPlotTypes::ChangeState) { }

  virtual void drawInit(CGnuPlotWindow *) { }
  virtual void drawTerm() { }

  virtual CGnuPlotRenderer *renderer() = 0;

  virtual bool parseArgs(CParseLine &);

  virtual void show(std::ostream &os) const;

 protected:
  CGnuPlot    *plot_      { 0 };
  std::string  name_;
  CISize2D     size_      { 600, 480 };
  bool         enhanced_  { true };
  CFontPtr     font_;
  double       fontScale_ { 1 };
  bool         dashed_    { false };
  bool         lineWidth_ { 0 };
};

#endif