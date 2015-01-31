#ifndef CGnuPlotSVGDevice_H
#define CGnuPlotSVGDevice_H

#include <CGnuPlotDevice.h>
#include <fstream>

class CGnuPlotSVGRenderer;

class CGnuPlotSVGDevice : public CGnuPlotDevice {
 public:
  CGnuPlotSVGDevice();

 ~CGnuPlotSVGDevice() override;

  CGnuPlotSVGRenderer *svgRenderer() const { return renderer_; }

  CGnuPlotWindow *createWindow() override;

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window) override;

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group) override;

  CGnuPlotLineStyle *createLineStyle() override;

  CGnuPlotArrow     *createArrow() override;
  CGnuPlotLabel     *createLabel() override;
  CGnuPlotEllipse   *createEllipse() override;
  CGnuPlotPolygon   *createPolygon() override;
  CGnuPlotRectangle *createRectangle() override;

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id,
                           COrientation dir) override;

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state) override;

  void timeout() override;

  void drawInit(CGnuPlotWindow *window) override;
  void drawTerm() override;

  std::ostream &os() { return *os_; }

  CGnuPlotRenderer *renderer() override;

 private:
  CGnuPlotSVGRenderer *renderer_;
  std::ostream        *os_;
  std::ofstream        file_;
};

#endif
