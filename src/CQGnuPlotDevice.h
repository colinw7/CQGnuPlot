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

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style) override;

  CGnuPlotLineStyle *createLineStyle() override;

  CGnuPlotArrow     *createArrow    (CGnuPlotGroup *group) override;
  CGnuPlotCircle    *createCircle   (CGnuPlotGroup *group) override;
  CGnuPlotEllipse   *createEllipse  (CGnuPlotGroup *group) override;
  CGnuPlotLabel     *createLabel    (CGnuPlotGroup *group) override;
  CGnuPlotPolygon   *createPolygon  (CGnuPlotGroup *group) override;
  CGnuPlotRectangle *createRectangle(CGnuPlotGroup *group) override;

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir) override;

  CGnuPlotKey  *createKey(CGnuPlotGroup *group) override;

  CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group) override;

  CGnuPlotPalette *createPalette(CGnuPlotGroup *group) override;

  CGnuPlotTitle  *createTitle (CGnuPlotGroup *group) override;
  CGnuPlotCamera *createCamera(CGnuPlotGroup *group) override;

  CGnuPlotBarObject    *createBarObject   (CGnuPlotPlot *plot) override;
  CGnuPlotBubbleObject *createBubbleObject(CGnuPlotPlot *plot) override;
  CGnuPlotPieObject    *createPieObject   (CGnuPlotPlot *plot) override;
  CGnuPlotRectObject   *createRectObject  (CGnuPlotPlot *plot) override;

  const Objects &objects() const { return objects_; }

  void timeout() override;

  void stateChanged(CGnuPlotWindow *, CGnuPlotTypes::ChangeState) override;

  CGnuPlotRenderer *renderer() override;

  bool parseArgs(CParseLine &line) override;

  void show(std::ostream &os) const override;

 private:
  CQGnuPlotRenderer *renderer_;
  Objects            objects_;
};

#endif
