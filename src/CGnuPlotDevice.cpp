#include <CGnuPlotDevice.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotBar.h>
#include <CGnuPlotPie.h>
#include <CGnuPlotBubble.h>

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
createPlot(CGnuPlotGroup *group)
{
  return new CGnuPlotPlot(group);
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

CGnuPlotBar *
CGnuPlotDevice::
createBar(CGnuPlotPlot *plot)
{
  return new CGnuPlotBar(plot);
}

CGnuPlotPie *
CGnuPlotDevice::
createPie(CGnuPlotPlot *plot)
{
  return new CGnuPlotPie(plot);
}

CGnuPlotBubble *
CGnuPlotDevice::
createBubble(CGnuPlotPlot *plot)
{
  return new CGnuPlotBubble(plot);
}

