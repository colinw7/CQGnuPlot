#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotAnnotation.h>
#include <CQGnuPlotArrow.h>
#include <CQGnuPlotCircle.h>
#include <CQGnuPlotEllipse.h>
#include <CQGnuPlotLabel.h>
#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotRectangle.h>
#include <CQGnuPlotAxis.h>
#include <CQGnuPlotKey.h>
#include <CQGnuPlotColorBox.h>
#include <CQGnuPlotPalette.h>
#include <CQGnuPlotTitle.h>
#include <CQGnuPlotCamera.h>
#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotRectObject.h>

#include <QApplication>

CQGnuPlotDevice::
CQGnuPlotDevice() :
 CGnuPlotDevice("Qt")
{
  renderer_ = new CQGnuPlotRenderer;

  setSize(CISize2D(800, 800));
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

  const CISize2D &s = size();

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
createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPlot *plot = new CQGnuPlotPlot(qgroup, style);

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

CGnuPlotCircle *
CQGnuPlotDevice::
createCircle(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotCircle *circle = new CQGnuPlotCircle(qgroup);

  objects_.push_back(circle);

  return circle;
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

CGnuPlotLabel *
CQGnuPlotDevice::
createLabel(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotLabel *label = new CQGnuPlotLabel(qgroup);

  objects_.push_back(label);

  return label;
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

CGnuPlotColorBox *
CQGnuPlotDevice::
createColorBox(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotColorBox *colorBox = new CQGnuPlotColorBox(qgroup);

  objects_.push_back(colorBox);

  return colorBox;
}

CGnuPlotPalette *
CQGnuPlotDevice::
createPalette(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPalette *palette = new CQGnuPlotPalette(qgroup);

  objects_.push_back(palette);

  return palette;
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

CGnuPlotCamera *
CQGnuPlotDevice::
createCamera(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotCamera *camera = new CQGnuPlotCamera(qgroup);

  return camera;
}

CGnuPlotBarObject *
CQGnuPlotDevice::
createBarObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBarObject *bar = new CQGnuPlotBarObject(qplot);

  objects_.push_back(bar);

  qplot->qwindow()->updateProperties();

  return bar;
}

CGnuPlotBubbleObject *
CQGnuPlotDevice::
createBubbleObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBubbleObject *bubble = new CQGnuPlotBubbleObject(qplot);

  objects_.push_back(bubble);

  qplot->qwindow()->updateProperties();

  return bubble;
}

CGnuPlotEllipseObject *
CQGnuPlotDevice::
createEllipseObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotEllipseObject *ellipse = new CQGnuPlotEllipseObject(qplot);

  objects_.push_back(ellipse);

  qplot->qwindow()->updateProperties();

  return ellipse;
}

CGnuPlotPieObject *
CQGnuPlotDevice::
createPieObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPieObject *pie = new CQGnuPlotPieObject(qplot);

  objects_.push_back(pie);

  qplot->qwindow()->updateProperties();

  return pie;
}

CGnuPlotPolygonObject *
CQGnuPlotDevice::
createPolygonObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPolygonObject *polygon = new CQGnuPlotPolygonObject(qplot);

  objects_.push_back(polygon);

  qplot->qwindow()->updateProperties();

  return polygon;
}

CGnuPlotRectObject *
CQGnuPlotDevice::
createRectObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotRectObject *rect = new CQGnuPlotRectObject(qplot);

  objects_.push_back(rect);

  qplot->qwindow()->updateProperties();

  return rect;
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

bool
CQGnuPlotDevice::
parseArgs(CParseLine &)
{
  return false;
}

void
CQGnuPlotDevice::
show(std::ostream &os) const
{
  os << "size " << size_.width << "," << size_.height;
  os << " fixed";
  os << " " << (enhanced_ ? "enhanced" : "noenhanced");
  os << " " << font_;
}
