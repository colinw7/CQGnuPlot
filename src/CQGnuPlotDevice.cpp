#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotAnnotation.h>
#include <CQGnuPlotArrow.h>
#include <CQGnuPlotEllipse.h>
#include <CQGnuPlotLabel.h>
#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotRectangle.h>
#include <CQGnuPlotAxis.h>
#include <CQGnuPlotKey.h>
#include <CQGnuPlotTitle.h>
#include <CQGnuPlotBar.h>
#include <CQGnuPlotPie.h>
#include <CQGnuPlotBubble.h>

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

  objects_.push_back(group);

  return group;
}

CGnuPlotPlot *
CQGnuPlotDevice::
createPlot(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPlot *plot = new CQGnuPlotPlot(qgroup);

  objects_.push_back(plot);

  qgroup->qwindow()->updateProperties();

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
createArrow(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotArrow *arrow = new CQGnuPlotArrow(qgroup);

  objects_.push_back(arrow);

  return arrow;
}

CGnuPlotLabel *
CQGnuPlotDevice::
createLabel(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotLabel *label = new CQGnuPlotLabel(qgroup);

  objects_.push_back(label);

  return label;
}

CGnuPlotEllipse *
CQGnuPlotDevice::
createEllipse(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotEllipse *ellipse = new CQGnuPlotEllipse(qgroup);

  objects_.push_back(ellipse);

  return ellipse;
}

CGnuPlotPolygon *
CQGnuPlotDevice::
createPolygon(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPolygon *poly = new CQGnuPlotPolygon(qgroup);

  objects_.push_back(poly);

  return poly;
}

CGnuPlotRectangle *
CQGnuPlotDevice::
createRectangle(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotRectangle *rect = new CQGnuPlotRectangle(qgroup);

  objects_.push_back(rect);

  return rect;
}

CGnuPlotAxis *
CQGnuPlotDevice::
createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotAxis *axis = new CQGnuPlotAxis(qgroup, id, dir);

  objects_.push_back(axis);

  return axis;
}

CGnuPlotKey *
CQGnuPlotDevice::
createKey(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotKey *key = new CQGnuPlotKey(qgroup);

  objects_.push_back(key);

  return key;
}

CGnuPlotTitle *
CQGnuPlotDevice::
createTitle(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotTitle *title = new CQGnuPlotTitle(qgroup);

  objects_.push_back(title);

  return title;
}

CGnuPlotBar *
CQGnuPlotDevice::
createBar(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBar *bar = new CQGnuPlotBar(qplot);

  objects_.push_back(bar);

  qplot->qwindow()->updateProperties();

  return bar;
}

CGnuPlotPie *
CQGnuPlotDevice::
createPie(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPie *pie = new CQGnuPlotPie(qplot);

  objects_.push_back(pie);

  qplot->qwindow()->updateProperties();

  return pie;
}

CGnuPlotBubble *
CQGnuPlotDevice::
createBubble(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBubble *bubble = new CQGnuPlotBubble(qplot);

  objects_.push_back(bubble);

  qplot->qwindow()->updateProperties();

  return bubble;
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

  qwindow->redraw();
}

CGnuPlotRenderer *
CQGnuPlotDevice::
renderer()
{
  return renderer_;
}
