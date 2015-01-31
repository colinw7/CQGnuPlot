#ifndef CQGnuPlotDevice_H
#define CQGnuPlotDevice_H

#include <CGnuPlotDevice.h>

class CQGnuPlotRenderer;

class CQGnuPlotDevice : public CGnuPlotDevice {
 public:
  CQGnuPlotDevice();

 ~CQGnuPlotDevice() override;

  CQGnuPlotRenderer *qrenderer() const { return renderer_; }

  CGnuPlotWindow *createWindow() override;

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window) override;

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group) override;

  CGnuPlotLineStyle *createLineStyle() override;

  CGnuPlotArrow     *createArrow() override;
  CGnuPlotLabel     *createLabel() override;
  CGnuPlotEllipse   *createEllipse() override;
  CGnuPlotPolygon   *createPolygon() override;
  CGnuPlotRectangle *createRectangle() override;

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir) override;

  void timeout() override;

  void stateChanged(CGnuPlotWindow *, CGnuPlotTypes::ChangeState) override;

  CGnuPlotRenderer *renderer() override;

 private:
  CQGnuPlotRenderer *renderer_;
};

#endif
