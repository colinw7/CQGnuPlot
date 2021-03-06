#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotMultiplot.h>
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
#include <CQGnuPlotPm3D.h>
#include <CQGnuPlotTimeStamp.h>
#include <CQGnuPlotBubbleObject.h>

#include <CQGnuPlotArrowObject.h>
#include <CQGnuPlotBoxBarObject.h>
#include <CQGnuPlotBoxObject.h>
#include <CQGnuPlotEndBar.h>
#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotErrorBarObject.h>
#include <CQGnuPlotFinanceBarObject.h>
#include <CQGnuPlotImageObject.h>
#include <CQGnuPlotLabelObject.h>
#include <CQGnuPlotPathObject.h>
#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPointObject.h>
#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotRectObject.h>

#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>
#include <CQGnuPlotMark.h>

#include <CQGnuPlotAdjacencyStyleValue.h>
#include <CQGnuPlotBoxErrorBarsStyleValue.h>
#include <CQGnuPlotBoxesStyleValue.h>
#include <CQGnuPlotBoxPlotStyleValue.h>
#include <CQGnuPlotBubbleStyleValue.h>
#include <CQGnuPlotCandlesticksStyleValue.h>
#include <CQGnuPlotChordDiagramStyleValue.h>
#include <CQGnuPlotCirclesStyleValue.h>
#include <CQGnuPlotDelaunayStyleValue.h>
#include <CQGnuPlotDendrogramStyleValue.h>
#include <CQGnuPlotEllipsesStyleValue.h>
#include <CQGnuPlotForceDirectedStyleValue.h>
#include <CQGnuPlotHierBubbleStyleValue.h>
#include <CQGnuPlotLabelsStyleValue.h>
#include <CQGnuPlotPieChartStyleValue.h>
#include <CQGnuPlotRadarStyleValue.h>
#include <CQGnuPlotSunburstStyleValue.h>
#include <CQGnuPlotTreeMapStyleValue.h>
#include <CQGnuPlotVectorsStyleValue.h>

#include <QApplication>

CQGnuPlotDevice::
CQGnuPlotDevice() :
 CGnuPlotDevice("Qt")
{
  renderer_ = new CQGnuPlotRenderer;

  setSize(CISize2D(1800, 1400));
}

CQGnuPlotDevice::
~CQGnuPlotDevice()
{
}

CGnuPlotWindow *
CQGnuPlotDevice::
createWindow()
{
  CQGnuPlot *qapp = dynamic_cast<CQGnuPlot *>(plot_);

  CQGnuPlotMainWindow *window = new CQGnuPlotMainWindow(qapp);

  const CISize2D &s = size();

  window->setSize(s);

  window->show();

  return window;
}

CGnuPlotMultiplot *
CQGnuPlotDevice::
createMultiplot()
{
  CQGnuPlot *qapp = dynamic_cast<CQGnuPlot *>(plot_);

  CQGnuPlotMultiplot *multiplot = new CQGnuPlotMultiplot(qapp);

  return multiplot;
}

CGnuPlotGroup *
CQGnuPlotDevice::
createGroup(CGnuPlotWindow *window)
{
  CQGnuPlotWindow *qwindow = dynamic_cast<CQGnuPlotWindow *>(window);

  CQGnuPlotGroup *group = new CQGnuPlotGroup(qwindow);

  objects_.push_back(group);

  return group;
}

CGnuPlotPlot *
CQGnuPlotDevice::
createPlot(CGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPlot *plot = new CQGnuPlotPlot(qgroup, style);

  objects_.push_back(plot);

  qgroup->qwindow()->updateProperties();

  return plot;
}

CGnuPlotLineStyle *
CQGnuPlotDevice::
createLineStyle(CGnuPlot *plot)
{
  return new CQGnuPlotLineStyle(plot);
}

//---

CGnuPlotArrow *
CQGnuPlotDevice::
createArrow(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotArrow *arrow = new CQGnuPlotArrow(qgroup);

  objects_.push_back(arrow);

  return arrow;
}

CGnuPlotCircle *
CQGnuPlotDevice::
createCircle(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotCircle *circle = new CQGnuPlotCircle(qgroup);

  objects_.push_back(circle);

  return circle;
}

CGnuPlotEllipse *
CQGnuPlotDevice::
createEllipse(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotEllipse *ellipse = new CQGnuPlotEllipse(qgroup);

  objects_.push_back(ellipse);

  return ellipse;
}

CGnuPlotLabel *
CQGnuPlotDevice::
createLabel(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotLabel *label = new CQGnuPlotLabel(qgroup);

  objects_.push_back(label);

  return label;
}

CGnuPlotPolygon *
CQGnuPlotDevice::
createPolygon(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPolygon *poly = new CQGnuPlotPolygon(qgroup);

  objects_.push_back(poly);

  return poly;
}

CGnuPlotRectangle *
CQGnuPlotDevice::
createRectangle(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotRectangle *rect = new CQGnuPlotRectangle(qgroup);

  objects_.push_back(rect);

  return rect;
}

//---

CGnuPlotAxis *
CQGnuPlotDevice::
createAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotAxis *axis = new CQGnuPlotAxis(qgroup, data);

  objects_.push_back(axis);

  return axis;
}

CGnuPlotKey *
CQGnuPlotDevice::
createKey(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotKey *key = new CQGnuPlotKey(qgroup);

  objects_.push_back(key);

  return key;
}

CGnuPlotColorBox *
CQGnuPlotDevice::
createColorBox(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotColorBox *colorBox = new CQGnuPlotColorBox(qgroup);

  objects_.push_back(colorBox);

  return colorBox;
}

CGnuPlotPalette *
CQGnuPlotDevice::
createPalette(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPalette *palette = new CQGnuPlotPalette(qgroup);

  objects_.push_back(palette);

  return palette;
}

CGnuPlotTitle *
CQGnuPlotDevice::
createTitle(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotTitle *title = new CQGnuPlotTitle(qgroup);

  objects_.push_back(title);

  return title;
}

CGnuPlotCamera *
CQGnuPlotDevice::
createCamera(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotCamera *camera = new CQGnuPlotCamera(qgroup);

  return camera;
}

CGnuPlotPm3D *
CQGnuPlotDevice::
createPm3D(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotPm3D *pm3d = new CQGnuPlotPm3D(qgroup);

  return pm3d;
}

CGnuPlotTimeStamp *
CQGnuPlotDevice::
createTimeStamp(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = dynamic_cast<CQGnuPlotGroup *>(group);

  CQGnuPlotTimeStamp *timeStamp = new CQGnuPlotTimeStamp(qgroup);

  return timeStamp;
}

//---

CGnuPlotArrowObject *
CQGnuPlotDevice::
createArrowObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotArrowObject *arrow = new CQGnuPlotArrowObject(qplot);

  objects_.push_back(arrow);

  //qplot->qwindow()->updateProperties();

  return arrow;
}

CGnuPlotBoxBarObject *
CQGnuPlotDevice::
createBoxBarObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBoxBarObject *bar = new CQGnuPlotBoxBarObject(qplot);

  objects_.push_back(bar);

  //qplot->qwindow()->updateProperties();

  return bar;
}

CGnuPlotEndBar *
CQGnuPlotDevice::
createEndBar(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotEndBar *bar = new CQGnuPlotEndBar(qplot);

  return bar;
}

CGnuPlotBoxObject *
CQGnuPlotDevice::
createBoxObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBoxObject *box = new CQGnuPlotBoxObject(qplot);

  objects_.push_back(box);

  //qplot->qwindow()->updateProperties();

  return box;
}

CGnuPlotBubbleObject *
CQGnuPlotDevice::
createBubbleObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotBubbleObject *bubble = new CQGnuPlotBubbleObject(qplot);

  objects_.push_back(bubble);

  //qplot->qwindow()->updateProperties();

  return bubble;
}

CGnuPlotEllipseObject *
CQGnuPlotDevice::
createEllipseObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotEllipseObject *ellipse = new CQGnuPlotEllipseObject(qplot);

  objects_.push_back(ellipse);

  //qplot->qwindow()->updateProperties();

  return ellipse;
}

CGnuPlotErrorBarObject *
CQGnuPlotDevice::
createErrorBarObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotErrorBarObject *bar = new CQGnuPlotErrorBarObject(qplot);

  objects_.push_back(bar);

  //qplot->qwindow()->updateProperties();

  return bar;
}

CGnuPlotFinanceBarObject *
CQGnuPlotDevice::
createFinanceBarObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotFinanceBarObject *bar = new CQGnuPlotFinanceBarObject(qplot);

  objects_.push_back(bar);

  //qplot->qwindow()->updateProperties();

  return bar;
}

CGnuPlotImageObject *
CQGnuPlotDevice::
createImageObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotImageObject *image = new CQGnuPlotImageObject(qplot);

  objects_.push_back(image);

  //qplot->qwindow()->updateProperties();

  return image;
}

CGnuPlotLabelObject *
CQGnuPlotDevice::
createLabelObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotLabelObject *label = new CQGnuPlotLabelObject(qplot);

  objects_.push_back(label);

  //qplot->qwindow()->updateProperties();

  return label;
}

CGnuPlotPathObject *
CQGnuPlotDevice::
createPathObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPathObject *path = new CQGnuPlotPathObject(qplot);

  objects_.push_back(path);

  //qplot->qwindow()->updateProperties();

  return path;
}

CGnuPlotPieObject *
CQGnuPlotDevice::
createPieObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPieObject *pie = new CQGnuPlotPieObject(qplot);

  objects_.push_back(pie);

  //qplot->qwindow()->updateProperties();

  return pie;
}

CGnuPlotPointObject *
CQGnuPlotDevice::
createPointObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPointObject *point = new CQGnuPlotPointObject(qplot);

  objects_.push_back(point);

  //qplot->qwindow()->updateProperties();

  return point;
}

CGnuPlotPolygonObject *
CQGnuPlotDevice::
createPolygonObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotPolygonObject *polygon = new CQGnuPlotPolygonObject(qplot);

  objects_.push_back(polygon);

  //qplot->qwindow()->updateProperties();

  return polygon;
}

CGnuPlotRectObject *
CQGnuPlotDevice::
createRectObject(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotRectObject *rect = new CQGnuPlotRectObject(qplot);

  objects_.push_back(rect);

  //qplot->qwindow()->updateProperties();

  return rect;
}

CGnuPlotFill *
CQGnuPlotDevice::
createFill(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotFill *fill = new CQGnuPlotFill(qplot);

  return fill;
}

CGnuPlotStroke *
CQGnuPlotDevice::
createStroke(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotStroke *stroke = new CQGnuPlotStroke(qplot);

  return stroke;
}

CGnuPlotMark *
CQGnuPlotDevice::
createMark(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotMark *mark = new CQGnuPlotMark(qplot);

  return mark;
}

//---

CGnuPlotAdjacencyStyleValue *
CQGnuPlotDevice::
createAdjacencyStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotAdjacencyStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotBoxErrorBarsStyleValue *
CQGnuPlotDevice::
createBoxErrorBarsStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotBoxErrorBarsStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotBoxesStyleValue *
CQGnuPlotDevice::
createBoxesStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotBoxesStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotBoxPlotStyleValue *
CQGnuPlotDevice::
createBoxPlotStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotBoxPlotStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotBubbleStyleValue *
CQGnuPlotDevice::
createBubbleStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotBubbleStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotCandlesticksStyleValue *
CQGnuPlotDevice::
createCandlesticksStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotCandlesticksStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotChordDiagramStyleValue *
CQGnuPlotDevice::
createChordDiagramStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotChordDiagramStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotCirclesStyleValue *
CQGnuPlotDevice::
createCirclesStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotCirclesStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotDelaunayStyleValue *
CQGnuPlotDevice::
createDelaunayStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotDelaunayStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotDendrogramStyleValue *
CQGnuPlotDevice::
createDendrogramStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotDendrogramStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotEllipsesStyleValue *
CQGnuPlotDevice::
createEllipsesStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotEllipsesStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotForceDirectedStyleValue *
CQGnuPlotDevice::
createForceDirectedStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotForceDirectedStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotHierBubbleStyleValue *
CQGnuPlotDevice::
createHierBubbleStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotHierBubbleStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotLabelsStyleValue *
CQGnuPlotDevice::
createLabelsStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotLabelsStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotPieChartStyleValue *
CQGnuPlotDevice::
createPieChartStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotPieChartStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotRadarStyleValue *
CQGnuPlotDevice::
createRadarStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotRadarStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotSunburstStyleValue *
CQGnuPlotDevice::
createSunburstStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotSunburstStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotTreeMapStyleValue *
CQGnuPlotDevice::
createTreeMapStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotTreeMapStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

CGnuPlotVectorsStyleValue *
CQGnuPlotDevice::
createVectorsStyleValue(CGnuPlotPlot *plot)
{
  return new CQGnuPlotVectorsStyleValue(dynamic_cast<CQGnuPlotPlot *>(plot));
}

//---

void
CQGnuPlotDevice::
timeout()
{
  if (! qApp->activeModalWidget())
    qApp->processEvents();
}

void
CQGnuPlotDevice::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState)
{
  CQGnuPlotWindow *qwindow = dynamic_cast<CQGnuPlotWindow *>(window);

  qwindow->redraw();
}

void
CQGnuPlotDevice::
redraw()
{
  for (auto &w : windows_) {
    CQGnuPlotWindow *qwindow = dynamic_cast<CQGnuPlotWindow *>(w.get());

    qwindow->redraw();
  }

  qApp->processEvents();
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
