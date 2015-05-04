#include <CGnuPlotDevice.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotBarObject.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotRectObject.h>
#include <CFontMgr.h>

CGnuPlotDevice::
CGnuPlotDevice(const std::string &name) :
 plot_(0)
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
createLineStyle()
{
  return new CGnuPlotLineStyle;
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
createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir)
{
  return new CGnuPlotAxis(group, id, dir);
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

CGnuPlotBarObject *
CGnuPlotDevice::
createBarObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotBarObject(plot);
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

CGnuPlotPieObject *
CGnuPlotDevice::
createPieObject(CGnuPlotPlot *plot)
{
  return new CGnuPlotPieObject(plot);
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
