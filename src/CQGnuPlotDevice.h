#ifndef CQGnuPlotDevice_H
#define CQGnuPlotDevice_H

#include <CGnuPlotDevice.h>
#include <QObject>
#include <QPointer>
#include <vector>

class CQGnuPlotRenderer;

class CQGnuPlotDevice : public QObject, public CGnuPlotDevice {
  Q_OBJECT

 public:
  typedef QPointer<QObject>    ObjectP;
  typedef std::vector<ObjectP> Objects;

 public:
  CQGnuPlotDevice();

 ~CQGnuPlotDevice() override;

  CQGnuPlotRenderer *qrenderer() const { return renderer_; }

  void redraw() override;

  CGnuPlotWindow *createWindow() override;

  CGnuPlotMultiplot *createMultiplot() override;

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window) override;

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style) override;

  CGnuPlotLineStyle *createLineStyle(CGnuPlot *plot) override;

  CGnuPlotArrow     *createArrow    (CGnuPlotGroup *group) override;
  CGnuPlotCircle    *createCircle   (CGnuPlotGroup *group) override;
  CGnuPlotEllipse   *createEllipse  (CGnuPlotGroup *group) override;
  CGnuPlotLabel     *createLabel    (CGnuPlotGroup *group) override;
  CGnuPlotPolygon   *createPolygon  (CGnuPlotGroup *group) override;
  CGnuPlotRectangle *createRectangle(CGnuPlotGroup *group) override;

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data) override;

  CGnuPlotKey  *createKey(CGnuPlotGroup *group) override;

  CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group) override;

  CGnuPlotPalette *createPalette(CGnuPlotGroup *group) override;

  CGnuPlotTitle  *createTitle (CGnuPlotGroup *group) override;
  CGnuPlotCamera *createCamera(CGnuPlotGroup *group) override;
  CGnuPlotPm3D   *createPm3D  (CGnuPlotGroup *group) override;

  CGnuPlotTimeStamp *createTimeStamp(CGnuPlotGroup *group) override;

  CGnuPlotBoxPlot *createBoxPlot(CGnuPlotPlot *plot) override;

  CGnuPlotArrowObject      *createArrowObject     (CGnuPlotPlot *plot) override;
  CGnuPlotBoxBarObject     *createBoxBarObject    (CGnuPlotPlot *plot) override;
  CGnuPlotEndBar           *createEndBar          (CGnuPlotPlot *plot) override;
  CGnuPlotBoxObject        *createBoxObject       (CGnuPlotPlot *plot) override;
  CGnuPlotBubbleObject     *createBubbleObject    (CGnuPlotPlot *plot) override;
  CGnuPlotEllipseObject    *createEllipseObject   (CGnuPlotPlot *plot) override;
  CGnuPlotErrorBarObject   *createErrorBarObject  (CGnuPlotPlot *plot) override;
  CGnuPlotFinanceBarObject *createFinanceBarObject(CGnuPlotPlot *plot) override;
  CGnuPlotImageObject      *createImageObject     (CGnuPlotPlot *plot) override;
  CGnuPlotLabelObject      *createLabelObject     (CGnuPlotPlot *plot) override;
  CGnuPlotPathObject       *createPathObject      (CGnuPlotPlot *plot) override;
  CGnuPlotPieObject        *createPieObject       (CGnuPlotPlot *plot) override;
  CGnuPlotPointObject      *createPointObject     (CGnuPlotPlot *plot) override;
  CGnuPlotPolygonObject    *createPolygonObject   (CGnuPlotPlot *plot) override;
  CGnuPlotRectObject       *createRectObject      (CGnuPlotPlot *plot) override;

  CGnuPlotFill   *createFill  (CGnuPlotPlot *plot) override;
  CGnuPlotStroke *createStroke(CGnuPlotPlot *plot) override;
  CGnuPlotMark   *createMark  (CGnuPlotPlot *plot) override;

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
