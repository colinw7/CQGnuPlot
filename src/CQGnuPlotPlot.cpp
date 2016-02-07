#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>

#include <CQGnuPlotArrowObject.h>
#include <CQGnuPlotBoxBarObject.h>
#include <CQGnuPlotBoxObject.h>
#include <CQGnuPlotBubbleObject.h>
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

#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotPlot::
CQGnuPlotPlot(CQGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style) :
 CGnuPlotPlot(group, style), group_(group)
{
  setObjectName("plot");

  // QObject to set properties for collections of bar and pieobjects
  arrowObjects_      = new CQGnuPlotPlotArrowObjects(this);
  boxBarObjects_     = new CQGnuPlotPlotBoxBarObjects(this);
  boxObjects_        = new CQGnuPlotPlotBoxObjects(this);
  bubbleObjects_     = new CQGnuPlotPlotBubbleObjects(this);
  ellipseObjects_    = new CQGnuPlotPlotEllipseObjects(this);
  errorBarObjects_   = new CQGnuPlotPlotErrorBarObjects(this);
  financeBarObjects_ = new CQGnuPlotPlotFinanceBarObjects(this);
  imageObjects_      = new CQGnuPlotPlotImageObjects(this);
  labelObjects_      = new CQGnuPlotPlotLabelObjects(this);
  pathObjects_       = new CQGnuPlotPlotPathObjects(this);
  pieObjects_        = new CQGnuPlotPlotPieObjects(this);
  pointObjects_      = new CQGnuPlotPlotPointObjects(this);
  polygonObjects_    = new CQGnuPlotPlotPolygonObjects(this);
  rectObjects_       = new CQGnuPlotPlotRectObjects(this);
}

CQGnuPlotPlot::
~CQGnuPlotPlot()
{
}

CQGnuPlotWindow *
CQGnuPlotPlot::
qwindow() const
{
  return qgroup()->qwindow();
}

QString
CQGnuPlotPlot::
usingColsStr() const
{
  return usingCols().str().c_str();
}

void
CQGnuPlotPlot::
setUsingColsStr(const QString &str)
{
  if (str != usingColsStr()) {
    usingCols_.parse(str.toStdString());

    if (is2D())
      app()->setPlotValues2D(this);
    else
      app()->setPlotValues3D(this);
  }
}

QColor
CQGnuPlotPlot::
lineColor() const
{
  return toQColor(CGnuPlotPlot::lineColor(CRGBA(0,0,0)));
}

void
CQGnuPlotPlot::
setLineColor(const QColor &c)
{
  if (c != lineColor()) {
    CGnuPlotColorSpec cs;

    cs.setRGB(fromQColor(c));

    CGnuPlotPlot::setLineColor(cs);
  }
}

bool
CQGnuPlotPlot::
isSurfaceEnabled() const
{
  return CGnuPlotPlot::surfaceData().isEnabled();
}

void
CQGnuPlotPlot::
setSurfaceEnabled(bool b)
{
  if (b != isSurfaceEnabled()) {
    CGnuPlotSurfaceData surfaceData = CGnuPlotPlot::surfaceData();

    surfaceData.setEnabled(b);

    CGnuPlotPlot::setSurfaceData(surfaceData);
  }
}

QColor
CQGnuPlotPlot::
surfaceColor() const
{
  const CGnuPlotSurfaceData &surfaceData = CGnuPlotPlot::surfaceData();

  return toQColor(surfaceData.color());
}

void
CQGnuPlotPlot::
setSurfaceColor(const QColor &c)
{
  if (c != surfaceColor()) {
    CGnuPlotSurfaceData surfaceData = CGnuPlotPlot::surfaceData();

    surfaceData.setColor(fromQColor(c));

    CGnuPlotPlot::setSurfaceData(surfaceData);
  }
}

CQGnuPlotEnum::PlotStyle
CQGnuPlotPlot::
plotStyle() const
{
  return CQGnuPlotEnum::plotStyleConv(CGnuPlotPlot::style());
}

void
CQGnuPlotPlot::
setPlotStyle(const CQGnuPlotEnum::PlotStyle &s)
{
  if (s != plotStyle()) {
    CGnuPlotPlot::setStyle(CQGnuPlotEnum::plotStyleConv(s));
  }
}

double
CQGnuPlotPlot::
xrangeMin() const
{
  double xmin, xmax;

  CGnuPlotPlot::getXRange(&xmin, &xmax);

  return xmin;
}

void
CQGnuPlotPlot::
setXRangeMin(double x)
{
  CGnuPlotPlot::xaxis(1).setMin(x);
}

double
CQGnuPlotPlot::
xrangeMax() const
{
  double xmin, xmax;

  CGnuPlotPlot::getXRange(&xmin, &xmax);

  return xmax;
}

void
CQGnuPlotPlot::
setXRangeMax(double x)
{
  CGnuPlotPlot::xaxis(1).setMax(x);
}

double
CQGnuPlotPlot::
yrangeMin() const
{
  double ymin, ymax;

  CGnuPlotPlot::getYRange(&ymin, &ymax);

  return ymin;
}

void
CQGnuPlotPlot::
setYRangeMin(double y)
{
  CGnuPlotPlot::yaxis(1).setMin(y);
}

double
CQGnuPlotPlot::
yrangeMax() const
{
  double ymin, ymax;

  CGnuPlotPlot::getYRange(&ymin, &ymax);

  return ymax;
}

void
CQGnuPlotPlot::
setYRangeMax(double y)
{
  CGnuPlotPlot::yaxis(1).setMax(y);
}

QString
CQGnuPlotPlot::
functions() const
{
  QString fns;

  for (const auto &s : CGnuPlotPlot::functions()) {
    if (fns.length()) fns += " ";

    fns += s.c_str();
  }

  return fns;
}

void
CQGnuPlotPlot::
setFunctions(const QString &functions)
{
  if (functions != this->functions()) {
    QStringList fns = functions.split(" ", QString::SkipEmptyParts);

    CGnuPlotPlot::StringArray fns1;

    for (int i = 0; i < fns.size(); ++i)
      fns1.push_back(fns[i].toStdString());

    CGnuPlotPlot::setFunctions(fns1);

    CGnuPlot::updateFunction2D(this);
  }
}

QString
CQGnuPlotPlot::
keyTitle() const
{
  return CGnuPlotPlot::keyTitleString().c_str();
}

void
CQGnuPlotPlot::
setKeyTitle(const QString &s)
{
  CGnuPlotPlot::setKeyTitleString(s.toStdString());
}

CQGnuPlotEnum::FillType
CQGnuPlotPlot::
fillType() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotPlot::fillStyle_.style());
}

void
CQGnuPlotPlot::
setFillType(const CQGnuPlotEnum::FillType &type)
{
  if (type != fillType()) {
    CGnuPlotPlot::fillStyle_.setStyle(CQGnuPlotUtil::fillTypeConv(type));
  }
}

CQGnuPlotEnum::FillPattern
CQGnuPlotPlot::
fillPattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotPlot::fillStyle_.pattern());
}

void
CQGnuPlotPlot::
setFillPattern(const CQGnuPlotEnum::FillPattern &pattern)
{
  if (pattern != fillPattern()) {
    CGnuPlotPlot::fillStyle_.setPattern(CQGnuPlotUtil::fillPatternConv(pattern));
  }
}

CQGnuPlotEnum::SymbolType
CQGnuPlotPlot::
pointType() const
{
  return CQGnuPlotEnum::symbolConv(CGnuPlotPlot::pointType());
}

void
CQGnuPlotPlot::
setPointType(const CQGnuPlotEnum::SymbolType &type)
{
  if (type != pointType()) {
    CGnuPlotPlot::setPointType(int(CQGnuPlotEnum::symbolConv(type)));
  }
}

CQGnuPlotEnum::BoxWidthType
CQGnuPlotPlot::
getBoxWidthType() const
{
  return CQGnuPlotUtil::boxWidthTypeConv(CGnuPlotPlot::getBoxWidthType());
}

void
CQGnuPlotPlot::
setBoxWidthType(const CQGnuPlotEnum::BoxWidthType &type)
{
  if (type != getBoxWidthType()) {
    CGnuPlotPlot::setBoxWidthType(CQGnuPlotUtil::boxWidthTypeConv(type));
  }
}

void
CQGnuPlotPlot::
draw()
{
  CGnuPlotPlot::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    if (! is3D())
      renderer->drawRect(bbox2D(), CRGBA(1,0,0), 2);
    else
      renderer->drawRect(bbox3D(), CRGBA(1,0,0), 2);
  }
}

void
CQGnuPlotPlot::
mousePress(const CGnuPlotMouseEvent &mouseEvent, Objects &objects)
{
  for (auto &arrow : arrowObjects()) {
    if (! arrow->inside(mouseEvent))
      continue;

    CQGnuPlotArrowObject *qarrow = dynamic_cast<CQGnuPlotArrowObject *>(arrow);

    objects.push_back(qarrow);
  }

  for (auto &bar : boxBarObjects()) {
    if (! bar->inside(mouseEvent))
      continue;

    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &box : boxObjects()) {
    if (! box->inside(mouseEvent))
      continue;

    CQGnuPlotBoxObject *qbox = dynamic_cast<CQGnuPlotBoxObject *>(box);

    objects.push_back(qbox);
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(mouseEvent))
      continue;

    CQGnuPlotBubbleObject *qbubble = dynamic_cast<CQGnuPlotBubbleObject *>(bubble);

    objects.push_back(qbubble);
  }

  for (auto &ellipse : ellipseObjects()) {
    if (! ellipse->inside(mouseEvent))
      continue;

    CQGnuPlotEllipseObject *qellipse = dynamic_cast<CQGnuPlotEllipseObject *>(ellipse);

    objects.push_back(qellipse);
  }

  for (auto &bar : errorBarObjects()) {
    if (! bar->inside(mouseEvent))
      continue;

    CQGnuPlotErrorBarObject *qbar = dynamic_cast<CQGnuPlotErrorBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &bar : financeBarObjects()) {
    if (! bar->inside(mouseEvent))
      continue;

    CQGnuPlotFinanceBarObject *qbar = dynamic_cast<CQGnuPlotFinanceBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &image : imageObjects()) {
    if (! image->inside(mouseEvent))
      continue;

    CQGnuPlotImageObject *qimage = dynamic_cast<CQGnuPlotImageObject *>(image);

    objects.push_back(qimage);
  }

  for (auto &label : labelObjects()) {
    if (! label->inside(mouseEvent))
      continue;

    CQGnuPlotLabelObject *qlabel = dynamic_cast<CQGnuPlotLabelObject *>(label);

    objects.push_back(qlabel);
  }

  for (auto &path : pathObjects()) {
    if (path->inside(mouseEvent)) {
      CQGnuPlotPathObject *qpath = dynamic_cast<CQGnuPlotPathObject *>(path);

      objects.push_back(qpath);
    }
  }

  for (auto &pie : pieObjects()) {
    if (pie->inside(mouseEvent) || pie->keyInside(mouseEvent.window())) {
      CQGnuPlotPieObject *qpie = dynamic_cast<CQGnuPlotPieObject *>(pie);

      objects.push_back(qpie);
    }
  }

  for (auto &point : pointObjects()) {
    if (! point->inside(mouseEvent))
      continue;

    CQGnuPlotPointObject *qpoint = dynamic_cast<CQGnuPlotPointObject *>(point);

    objects.push_back(qpoint);
  }

  for (auto &polygon : polygonObjects()) {
    if (! polygon->inside(mouseEvent))
      continue;

    CQGnuPlotPolygonObject *qpolygon = dynamic_cast<CQGnuPlotPolygonObject *>(polygon);

    objects.push_back(qpolygon);
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(mouseEvent))
      continue;

    CQGnuPlotRectObject *qrect = dynamic_cast<CQGnuPlotRectObject *>(rect);

    objects.push_back(qrect);
  }

  CGnuPlotPlot::mousePress(mouseEvent);
}

void
CQGnuPlotPlot::
mouseMove(const CGnuPlotMouseEvent &)
{
}

bool
CQGnuPlotPlot::
mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip)
{
  selectedPos_.setInvalid();

  for (auto &arrow : arrowObjects()) {
    if (! arrow->inside(mouseEvent))
      continue;

    CQGnuPlotArrowObject *qarrow = dynamic_cast<CQGnuPlotArrowObject *>(arrow);

    qwindow()->highlightObject(qarrow);

    tip = arrow->tip();

    return true;
  }

  for (auto &bar : boxBarObjects()) {
    if (! bar->inside(mouseEvent))
      continue;

    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &box : boxObjects()) {
    if (! box->inside(mouseEvent))
      continue;

    CQGnuPlotBoxObject *qbox = dynamic_cast<CQGnuPlotBoxObject *>(box);

    qwindow()->highlightObject(qbox);

    tip = box->tip();

    return true;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(mouseEvent))
      continue;

    CQGnuPlotBubbleObject *qbubble = dynamic_cast<CQGnuPlotBubbleObject *>(bubble);

    qwindow()->highlightObject(qbubble);

    tip = bubble->tip();

    return true;
  }

  for (auto &ellipse : ellipseObjects()) {
    if (! ellipse->inside(mouseEvent))
      continue;

    CQGnuPlotEllipseObject *qellipse = dynamic_cast<CQGnuPlotEllipseObject *>(ellipse);

    qwindow()->highlightObject(qellipse);

    tip = ellipse->tip();

    return true;
  }

  for (auto &bar : errorBarObjects()) {
    if (! bar->inside(mouseEvent))
      continue;

    CQGnuPlotErrorBarObject *qbar = dynamic_cast<CQGnuPlotErrorBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &bar : financeBarObjects()) {
    if (! bar->inside(mouseEvent))
      continue;

    CQGnuPlotFinanceBarObject *qbar = dynamic_cast<CQGnuPlotFinanceBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &image : imageObjects()) {
    if (! image->inside(mouseEvent))
      continue;

    CQGnuPlotImageObject *qimage = dynamic_cast<CQGnuPlotImageObject *>(image);

    qwindow()->highlightObject(qimage);

    tip = image->tip();

    return true;
  }

  for (auto &label : labelObjects()) {
    if (! label->inside(mouseEvent))
      continue;

    CQGnuPlotLabelObject *qlabel = dynamic_cast<CQGnuPlotLabelObject *>(label);

    qwindow()->highlightObject(qlabel);

    tip = label->tip();

    return true;
  }

  for (auto &path : pathObjects()) {
    if (! path->inside(mouseEvent))
      continue;

    CQGnuPlotPathObject *qpath = dynamic_cast<CQGnuPlotPathObject *>(path);

    qwindow()->highlightObject(qpath);

    tip = path->tip();

    return true;
  }

  for (auto &pie : pieObjects()) {
    if (! pie->inside(mouseEvent))
      continue;

    CQGnuPlotPieObject *qpie = dynamic_cast<CQGnuPlotPieObject *>(pie);

    qwindow()->highlightObject(qpie);

    tip = pie->tip();

    return true;
  }

  for (auto &point : pointObjects()) {
    if (! point->inside(mouseEvent))
      continue;

    CQGnuPlotPointObject *qpoint = dynamic_cast<CQGnuPlotPointObject *>(point);

    qwindow()->highlightObject(qpoint);

    tip = point->tip();

    return true;
  }

  for (auto &polygon : polygonObjects()) {
    if (! polygon->inside(mouseEvent))
      continue;

    CQGnuPlotPolygonObject *qpolygon = dynamic_cast<CQGnuPlotPolygonObject *>(polygon);

    qwindow()->highlightObject(qpolygon);

    tip = polygon->tip();

    return true;
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(mouseEvent))
      continue;

    CQGnuPlotRectObject *qrect = dynamic_cast<CQGnuPlotRectObject *>(rect);

    qwindow()->highlightObject(qrect);

    tip = rect->tip();

    return true;
  }

  return CGnuPlotPlot::mouseTip(mouseEvent, tip);
}

void
CQGnuPlotPlot::
moveObjects(int key)
{
  typedef std::vector<CGnuPlotPlotObject *> Objects;

  Objects objects;

  for (auto &image : imageObjects()) {
    if (image->isSelected())
      objects.push_back(image);
  }

  for (auto &rect : rectObjects()) {
    if (rect->isSelected())
      objects.push_back(rect);
  }

  for (const auto &obj : objects) {
    CBBox2D bbox = obj->bbox();

    if      (key == Qt::Key_Left)
      bbox.moveBy(-CPoint2D(bbox.getWidth()/10, 0));
    else if (key == Qt::Key_Right)
      bbox.moveBy( CPoint2D(bbox.getWidth()/10, 0));
    else if (key == Qt::Key_Down)
      bbox.moveBy(-CPoint2D(0, bbox.getHeight()/10));
    else if (key == Qt::Key_Up)
      bbox.moveBy( CPoint2D(0, bbox.getHeight()/10));

    obj->setBBox(bbox);
  }
}

//------

QColor
CQGnuPlotPlotBoxBarObjects::
fillColor() const
{
  CQGnuPlotBoxBarObject *qbar = firstBar();

  return (qbar ? qbar->fill()->color() : QColor());
}

void
CQGnuPlotPlotBoxBarObjects::
setFillColor(const QColor &c)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->fill()->setColor(c);
  }
}

bool
CQGnuPlotPlotBoxBarObjects::
hasBorder() const
{
  CQGnuPlotBoxBarObject *qbar = firstBar();

  return (qbar ? qbar->stroke()->isEnabled() : false);
}

void
CQGnuPlotPlotBoxBarObjects::
setBorder(bool b)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->stroke()->setEnabled(b);
  }
}

QColor
CQGnuPlotPlotBoxBarObjects::
lineColor() const
{
  CQGnuPlotBoxBarObject *qbar = firstBar();

  return (qbar ? qbar->stroke()->color() : QColor());
}

void
CQGnuPlotPlotBoxBarObjects::
setLineColor(const QColor &c)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->stroke()->setColor(c);
  }
}

double
CQGnuPlotPlotBoxBarObjects::
lineWidth() const
{
  CQGnuPlotBoxBarObject *qbar = firstBar();

  return (qbar ? qbar->stroke()->width() : 0);
}

void
CQGnuPlotPlotBoxBarObjects::
setLineWidth(double r)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->stroke()->setWidth(r);
  }
}

CQGnuPlotEnum::FillType
CQGnuPlotPlotBoxBarObjects::
getFillType() const
{
  CQGnuPlotBoxBarObject *qbar = firstBar();

  return (qbar ? qbar->fill()->type() : CQGnuPlotEnum::FillType::FillNone);
}

void
CQGnuPlotPlotBoxBarObjects::
setFillType(const CQGnuPlotEnum::FillType &t)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->fill()->setType(t);
  }
}

CQGnuPlotEnum::FillPattern
CQGnuPlotPlotBoxBarObjects::
getFillPattern() const
{
  CQGnuPlotBoxBarObject *qbar = firstBar();

  return (qbar ? qbar->fill()->pattern() : CQGnuPlotEnum::FillPattern::PatternNone);
}

void
CQGnuPlotPlotBoxBarObjects::
setFillPattern(const CQGnuPlotEnum::FillPattern &p)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = dynamic_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->fill()->setPattern(p);
  }
}

CQGnuPlotBoxBarObject *
CQGnuPlotPlotBoxBarObjects::
firstBar() const
{
  if (plot_->boxBarObjects().empty())
    return 0;

  return dynamic_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);
}

//------

double
CQGnuPlotPlotPieObjects::
innerRadius() const
{
  CQGnuPlotPieObject *qpie = firstPie();

  return (qpie ? qpie->innerRadius() : 0);
}

void
CQGnuPlotPlotPieObjects::
setInnerRadius(double r)
{
  for (auto &pie : plot_->pieObjects())
    pie->setInnerRadius(r);
}

double
CQGnuPlotPlotPieObjects::
labelRadius() const
{
  CQGnuPlotPieObject *qpie = firstPie();

  return (qpie ? qpie->labelRadius() : 0);
}

void
CQGnuPlotPlotPieObjects::
setLabelRadius(double r)
{
  for (auto &pie : plot_->pieObjects())
    pie->setLabelRadius(r);
}

CQGnuPlotPieObject *
CQGnuPlotPlotPieObjects::
firstPie() const
{
  if (plot_->pieObjects().empty())
    return 0;

  return dynamic_cast<CQGnuPlotPieObject *>(plot_->pieObjects()[0]);
}

//------

CQGnuPlotEnum::SymbolType
CQGnuPlotPlotPointObjects::
pointType() const
{
  const CQGnuPlotPointObject *qpoint = firstPoint();

  return (qpoint ? qpoint->pointType() : CQGnuPlotEnum::SymbolNone);
}

void
CQGnuPlotPlotPointObjects::
setPointType(const CQGnuPlotEnum::SymbolType &type)
{
  for (auto &point : plot_->pointObjects()) {
    CQGnuPlotPointObject *qpoint = dynamic_cast<CQGnuPlotPointObject *>(point);

    qpoint->setPointType(type);
  }
}

double
CQGnuPlotPlotPointObjects::
size() const
{
  const CQGnuPlotPointObject *qpoint = firstPoint();

  return (qpoint ? qpoint->size() : 0);
}

void
CQGnuPlotPlotPointObjects::
setSize(double s)
{
  for (auto &point : plot_->pointObjects()) {
    CQGnuPlotPointObject *qpoint = dynamic_cast<CQGnuPlotPointObject *>(point);

    qpoint->setSize(s);
  }
}

QColor
CQGnuPlotPlotPointObjects::
color() const
{
  const CQGnuPlotPointObject *qpoint = firstPoint();

  return (qpoint ? qpoint->color() : QColor());
}

void
CQGnuPlotPlotPointObjects::
setColor(const QColor &color)
{
  for (auto &point : plot_->pointObjects()) {
    CQGnuPlotPointObject *qpoint = dynamic_cast<CQGnuPlotPointObject *>(point);

    qpoint->setColor(color);
  }
}

CQGnuPlotPointObject *
CQGnuPlotPlotPointObjects::
firstPoint() const
{
  if (plot_->pointObjects().empty())
    return 0;

  return dynamic_cast<CQGnuPlotPointObject *>(plot_->pointObjects()[0]);
}
