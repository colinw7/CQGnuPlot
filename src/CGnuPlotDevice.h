#ifndef CGnuPlotDevice_H
#define CGnuPlotDevice_H

#include <COrientation.h>
#include <CGnuPlotTypes.h>

#include <string>

class CGnuPlot;
class CGnuPlotArrow;
class CGnuPlotAxis;
class CGnuPlotEllipse;
class CGnuPlotGroup;
class CGnuPlotLabel;
class CGnuPlotLineStyle;
class CGnuPlotPlot;
class CGnuPlotPolygon;
class CGnuPlotRectangle;
class CGnuPlotRenderer;
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

  virtual CGnuPlotWindow *createWindow() = 0;

  virtual CGnuPlotGroup *createGroup(CGnuPlotWindow *window) = 0;

  virtual CGnuPlotPlot *createPlot(CGnuPlotGroup *group) = 0;

  virtual CGnuPlotLineStyle *createLineStyle() = 0;

  virtual CGnuPlotArrow     *createArrow() = 0;
  virtual CGnuPlotLabel     *createLabel() = 0;
  virtual CGnuPlotEllipse   *createEllipse() = 0;
  virtual CGnuPlotPolygon   *createPolygon() = 0;
  virtual CGnuPlotRectangle *createRectangle() = 0;

  virtual CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id,
                                   COrientation dir) = 0;

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
