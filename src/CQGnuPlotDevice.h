#ifndef CQGnuPlotDevice_H
#define CQGnuPlotDevice_H

#include <CGnuPlotDevice.h>
#include <vector>

class CQGnuPlotRenderer;
class CQGnuPlotObject;

class CQGnuPlotDevice : public CGnuPlotDevice {
 public:
  typedef std::vector<CQGnuPlotObject *> Objects;

 public:
  CQGnuPlotDevice();

 ~CQGnuPlotDevice() override;

  CQGnuPlotRenderer *qrenderer() const { return renderer_; }

  CGnuPlotWindow *createWindow() override;

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window) override;

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group) override;

  CGnuPlotLineStyle *createLineStyle() override;

  CGnuPlotArrow     *createArrow    (CGnuPlotGroup *group) override;
  CGnuPlotLabel     *createLabel    (CGnuPlotGroup *group) override;
  CGnuPlotEllipse   *createEllipse  (CGnuPlotGroup *group) override;
  CGnuPlotPolygon   *createPolygon  (CGnuPlotGroup *group) override;
  CGnuPlotRectangle *createRectangle(CGnuPlotGroup *group) override;

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir) override;

  CGnuPlotKey  *createKey(CGnuPlotGroup *group) override;

  CGnuPlotTitle *createTitle(CGnuPlotGroup *group) override;

  CGnuPlotBar    *createBar   (CGnuPlotPlot *plot) override;
  CGnuPlotPie    *createPie   (CGnuPlotPlot *plot) override;
  CGnuPlotBubble *createBubble(CGnuPlotPlot *plot) override;

  const Objects &objects() const { return objects_; }

  void timeout() override;

  void stateChanged(CGnuPlotWindow *, CGnuPlotTypes::ChangeState) override;

  CGnuPlotRenderer *renderer() override;

 private:
  CQGnuPlotRenderer *renderer_;
  Objects            objects_;
};

#endif
