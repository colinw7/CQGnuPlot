#include <CGnuPlotDevice.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

#include <CGnuPlotArrowObject.h>
#include <CGnuPlotBoxBarObject.h>
#include <CGnuPlotBoxObject.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotFinanceBarObject.h>
#include <CGnuPlotImageObject.h>
#include <CGnuPlotLabelObject.h>
#include <CGnuPlotPathObject.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotPointObject.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotRectObject.h>

#include <CGnuPlotAdjacencyStyleValue.h>
#include <CGnuPlotBivariateStyleValue.h>
#include <CGnuPlotBoxErrorBarsStyleValue.h>
#include <CGnuPlotBoxesStyleValue.h>
#include <CGnuPlotBoxPlotStyleValue.h>
#include <CGnuPlotBubbleStyleValue.h>
#include <CGnuPlotCandlesticksStyleValue.h>
#include <CGnuPlotChordDiagramStyleValue.h>
#include <CGnuPlotCirclesStyleValue.h>
#include <CGnuPlotDelaunayStyleValue.h>
#include <CGnuPlotDendrogramStyleValue.h>
#include <CGnuPlotEllipsesStyleValue.h>
#include <CGnuPlotForceDirectedStyleValue.h>
#include <CGnuPlotHierBubbleStyleValue.h>
#include <CGnuPlotLabelsStyleValue.h>
#include <CGnuPlotPieChartStyleValue.h>
#include <CGnuPlotRadarStyleValue.h>
#include <CGnuPlotSunburstStyleValue.h>
#include <CGnuPlotTreeMapStyleValue.h>
#include <CGnuPlotVectorsStyleValue.h>

#include <CFontMgr.h>

CGnuPlotDevice::
CGnuPlotDevice(const std::string &name) :
 plot_(nullptr)
{
  name_ = CStrUtil::toLower(name);
  size_ = CISize2D(600, 480);
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

CGnuPlotWindow *
CGnuPlotDevice::
createWindow()
{
  return new CGnuPlotWindow(plot_);
}

CGnuPlotMultiplot *
CGnuPlotDevice::
createMultiplot()
{
  return new CGnuPlotMultiplot(plot_);
}

CGnuPlotGroup *
CGnuPlotDevice::
createGroup(CGnuPlotWindow *window)
{
  return new CGnuPlotGroup(window);
}

CGnuPlotPlot *
CGnuPlotDevice::
createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle plotStyle)
{
  return new CGnuPlotPlot(group, plotStyle);
}

CGnuPlotLineStyle *
CGnuPlotDevice::
createLineStyle(CGnuPlot *plot)
{
  return new CGnuPlotLineStyle(plot);
}

CGnuPlotArrow *
CGnuPlotDevice::
createArrow(CGnuPlotGroup *group)
{
  return new CGnuPlotArrow(group);
}

CGnuPlotCircle *
CGnuPlotDevice::
createCircle(CGnuPlotGroup *group)
{
  return new CGnuPlotCircle(group);
}

CGnuPlotEllipse *
CGnuPlotDevice::
createEllipse(CGnuPlotGroup *group)
{
  return new CGnuPlotEllipse(group);
}

CGnuPlotLabel *
CGnuPlotDevice::
createLabel(CGnuPlotGroup *group)
{
  return new CGnuPlotLabel(group);
}

CGnuPlotPolygon *
CGnuPlotDevice::
createPolygon(CGnuPlotGroup *group)
{
  return new CGnuPlotPolygon(group);
}

CGnuPlotRectangle *
CGnuPlotDevice::
createRectangle(CGnuPlotGroup *group)
{
  return new CGnuPlotRectangle(group);
}

CGnuPlotAxis *
CGnuPlotDevice::
createAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data)
{
  return new CGnuPlotAxis(group, data);
}

CGnuPlotKey *
CGnuPlotDevice::
createKey(CGnuPlotGroup *group)
{
  return new CGnuPlotKey(group);
}

CGnuPlotColorBox *
CGnuPlotDevice::
createColorBox(CGnuPlotGroup *group)
{
  return new CGnuPlotColorBox(group);
}

CGnuPlotPalette *
CGnuPlotDevice::
createPalette(CGnuPlotGroup *group)
{
  return new CGnuPlotPalette(group);
}

CGnuPlotTitle *
CGnuPlotDevice::
createTitle(CGnuPlotGroup *group)
{
  return new CGnuPlotTitle(group);
}

CGnuPlotCamera *
CGnuPlotDevice::
createCamera(CGnuPlotGroup *group)
{
  return new CGnuPlotCamera(group);
}

CGnuPlotPm3D *
CGnuPlotDevice::
createPm3D(CGnuPlotGroup *group)
{
  return new CGnuPlotPm3D(group);
}

CGnuPlotTimeStamp *
CGnuPlotDevice::
createTimeStamp(CGnuPlotGroup *group)
{
  return new CGnuPlotTimeStamp(group);
}

//---

CGnuPlotArrowObject *
CGnuPlotDevice::
createArrowObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotArrowObject(plot);
}

CGnuPlotBoxBarObject *
CGnuPlotDevice::
createBoxBarObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotBoxBarObject(plot);
}

CGnuPlotEndBar *
CGnuPlotDevice::
createEndBar(CGnuPlotPlot *plot)
{
  return new CGnuPlotEndBar(plot);
}

CGnuPlotBoxObject *
CGnuPlotDevice::
createBoxObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotBoxObject(plot);
}

CGnuPlotBubbleObject *
CGnuPlotDevice::
createBubbleObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotBubbleObject(plot);
}

CGnuPlotEllipseObject *
CGnuPlotDevice::
createEllipseObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotEllipseObject(plot);
}

CGnuPlotErrorBarObject *
CGnuPlotDevice::
createErrorBarObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotErrorBarObject(plot);
}

CGnuPlotFinanceBarObject *
CGnuPlotDevice::
createFinanceBarObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotFinanceBarObject(plot);
}

CGnuPlotImageObject *
CGnuPlotDevice::
createImageObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotImageObject(plot);
}

CGnuPlotLabelObject *
CGnuPlotDevice::
createLabelObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotLabelObject(plot);
}

CGnuPlotPathObject *
CGnuPlotDevice::
createPathObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotPathObject(plot);
}

CGnuPlotPieObject *
CGnuPlotDevice::
createPieObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotPieObject(plot);
}

CGnuPlotPointObject *
CGnuPlotDevice::
createPointObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotPointObject(plot);
}

CGnuPlotPolygonObject *
CGnuPlotDevice::
createPolygonObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotPolygonObject(plot);
}

CGnuPlotRectObject *
CGnuPlotDevice::
createRectObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotRectObject(plot);
}

CGnuPlotFill *
CGnuPlotDevice::
createFill(CGnuPlotPlot *plot)
{
  return new CGnuPlotFill(plot);
}

CGnuPlotStroke *
CGnuPlotDevice::
createStroke(CGnuPlotPlot *plot)
{
  return new CGnuPlotStroke(plot);
}

CGnuPlotMark *
CGnuPlotDevice::
createMark(CGnuPlotPlot *plot)
{
  return new CGnuPlotMark(plot);
}

//---

CGnuPlotAdjacencyStyleValue *
CGnuPlotDevice::
createAdjacencyStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotAdjacencyStyleValue(plot);
}

CGnuPlotBivariateStyleValue *
CGnuPlotDevice::
createBivariateStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotBivariateStyleValue(plot);
}

CGnuPlotBoxErrorBarsStyleValue *
CGnuPlotDevice::
createBoxErrorBarsStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotBoxErrorBarsStyleValue(plot);
}

CGnuPlotBoxesStyleValue *
CGnuPlotDevice::
createBoxesStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotBoxesStyleValue(plot);
}

CGnuPlotBoxPlotStyleValue *
CGnuPlotDevice::
createBoxPlotStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotBoxPlotStyleValue(plot);
}

CGnuPlotBubbleStyleValue *
CGnuPlotDevice::
createBubbleStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotBubbleStyleValue(plot);
}

CGnuPlotCandlesticksStyleValue *
CGnuPlotDevice::
createCandlesticksStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotCandlesticksStyleValue(plot);
}

CGnuPlotChordDiagramStyleValue *
CGnuPlotDevice::
createChordDiagramStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotChordDiagramStyleValue(plot);
}

CGnuPlotDendrogramStyleValue *
CGnuPlotDevice::
createDendrogramStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotDendrogramStyleValue(plot);
}

CGnuPlotDelaunayStyleValue *
CGnuPlotDevice::
createDelaunayStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotDelaunayStyleValue(plot);
}

CGnuPlotForceDirectedStyleValue *
CGnuPlotDevice::
createForceDirectedStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotForceDirectedStyleValue(plot);
}

CGnuPlotHierBubbleStyleValue *
CGnuPlotDevice::
createHierBubbleStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotHierBubbleStyleValue(plot);
}

CGnuPlotLabelsStyleValue *
CGnuPlotDevice::
createLabelsStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotLabelsStyleValue(plot);
}

CGnuPlotPieChartStyleValue *
CGnuPlotDevice::
createPieChartStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotPieChartStyleValue(plot);
}

CGnuPlotCirclesStyleValue *
CGnuPlotDevice::
createCirclesStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotCirclesStyleValue(plot);
}

CGnuPlotEllipsesStyleValue *
CGnuPlotDevice::
createEllipsesStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotEllipsesStyleValue(plot);
}

CGnuPlotRadarStyleValue *
CGnuPlotDevice::
createRadarStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotRadarStyleValue(plot);
}

CGnuPlotSunburstStyleValue *
CGnuPlotDevice::
createSunburstStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotSunburstStyleValue(plot);
}

CGnuPlotTreeMapStyleValue *
CGnuPlotDevice::
createTreeMapStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotTreeMapStyleValue(plot);
}

CGnuPlotVectorsStyleValue *
CGnuPlotDevice::
createVectorsStyleValue(CGnuPlotPlot *plot)
{
  return new CGnuPlotVectorsStyleValue(plot);
}

//---

bool
CGnuPlotDevice::
parseArgs(CParseLine &)
{
  return false;
}

void
CGnuPlotDevice::
show(std::ostream &) const
{
}
