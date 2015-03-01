#ifndef CGnuPlotDevice_H
#define CGnuPlotDevice_H

#include <COrientation.h>
#include <CGnuPlotTypes.h>

#include <string>

class CGnuPlot;
class CGnuPlotArrow;
class CGnuPlotAxis;
class CGnuPlotBar;
class CGnuPlotBubble;
class CGnuPlotColorBox;
class CGnuPlotEllipse;
class CGnuPlotGroup;
class CGnuPlotKey;
class CGnuPlotLabel;
class CGnuPlotLineStyle;
class CGnuPlotPalette;
class CGnuPlotPie;
class CGnuPlotPlot;
class CGnuPlotPolygon;
class CGnuPlotRectangle;
class CGnuPlotRenderer;
class CGnuPlotTitle;
class CGnuPlotWindow;

class CGnuPlotDevice {
 public:
  CGnuPlotDevice(const std::string &name) :
   name_(name) {
  }

  virtual ~CGnuPlotDevice() { }

  CGnuPlot *app() const { return plot_; }
  void setApp(CGnuPlot *plot) { plot_ = plot; }

  const std::string &name() const { return name_; }

  virtual CGnuPlotWindow *createWindow();

  virtual CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  virtual CGnuPlotPlot *createPlot(CGnuPlotGroup *group);

  virtual CGnuPlotLineStyle *createLineStyle();

  virtual CGnuPlotArrow     *createArrow(CGnuPlotGroup *group);
  virtual CGnuPlotLabel     *createLabel(CGnuPlotGroup *group);
  virtual CGnuPlotEllipse   *createEllipse(CGnuPlotGroup *group);
  virtual CGnuPlotPolygon   *createPolygon(CGnuPlotGroup *group);
  virtual CGnuPlotRectangle *createRectangle(CGnuPlotGroup *group);

  virtual CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir);

  virtual CGnuPlotKey *createKey(CGnuPlotGroup *group);

  virtual CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group);

  virtual CGnuPlotPalette *createPalette(CGnuPlotGroup *group);

  virtual CGnuPlotTitle *createTitle(CGnuPlotGroup *group);

  virtual CGnuPlotBar *createBar(CGnuPlotPlot *plot);

  virtual CGnuPlotPie *createPie(CGnuPlotPlot *plot);

  virtual CGnuPlotBubble *createBubble(CGnuPlotPlot *plot);

  virtual void timeout() { }

  virtual void stateChanged(CGnuPlotWindow *, CGnuPlotTypes::ChangeState) { }

  virtual void drawInit(CGnuPlotWindow *) { }
  virtual void drawTerm() { }

  virtual CGnuPlotRenderer *renderer() = 0;

 protected:
  CGnuPlot    *plot_ { 0 };
  std::string  name_;
};

#endif
