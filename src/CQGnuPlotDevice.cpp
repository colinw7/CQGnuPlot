#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotObject.h>

#include <QApplication>

CQGnuPlotDevice::
CQGnuPlotDevice() :
 CGnuPlotDevice("Qt")
{
  renderer_ = new CQGnuPlotRenderer;
}

CQGnuPlotDevice::
~CQGnuPlotDevice()
{
}

CGnuPlotWindow *
CQGnuPlotDevice::
createWindow()
{
  CQGnuPlot *qapp = static_cast<CQGnuPlot *>(plot_);

  CQGnuPlotWindow *window = new CQGnuPlotWindow(qapp);

  const CISize2D &s = qapp->getTerminalSize();

  window->setSize(s);

  window->show();

  return window;
}

CGnuPlotGroup *
CQGnuPlotDevice::
createGroup(CGnuPlotWindow *window)
{
  CQGnuPlotWindow *qwindow = static_cast<CQGnuPlotWindow *>(window);

  CQGnuPlotGroup *group = new CQGnuPlotGroup(qwindow);

  return group;
}

CGnuPlotPlot *
CQGnuPlotDevice::
createPlot(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPlot *plot = new CQGnuPlotPlot(qgroup);

  return plot;
}

CGnuPlotLineStyle *
CQGnuPlotDevice::
createLineStyle()
{
  return new CQGnuPlotLineStyle;
}

CGnuPlotArrow *
CQGnuPlotDevice::
createArrow()
{
  return new CQGnuPlotArrow;
}

CGnuPlotLabel *
CQGnuPlotDevice::
createLabel()
{
  return new CQGnuPlotLabel;
}

CGnuPlotEllipse *
CQGnuPlotDevice::
createEllipse()
{
  return new CQGnuPlotEllipse;
}

CGnuPlotPolygon *
CQGnuPlotDevice::
createPolygon()
{
  return new CQGnuPlotPolygon;
}

CGnuPlotRectangle *
CQGnuPlotDevice::
createRectangle()
{
  return new CQGnuPlotRectangle;
}

void
CQGnuPlotDevice::
timeout()
{
  qApp->processEvents();
}

void
CQGnuPlotDevice::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState)
{
  CQGnuPlotWindow *qwindow = static_cast<CQGnuPlotWindow *>(window);

  qwindow->addProperties();
  qwindow->redraw();
}

CGnuPlotRenderer *
CQGnuPlotDevice::
renderer()
{
  return renderer_;
}
