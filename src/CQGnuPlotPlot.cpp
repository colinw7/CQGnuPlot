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
  boxBarObjects_ = new CQGnuPlotPlotBoxBarObjects(this);
  pieObjects_    = new CQGnuPlotPlotPieObjects(this);
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
  CGnuPlotColorSpec cs;

  cs.setRGB(fromQColor(c));

  CGnuPlotPlot::setLineColor(cs);
}

CQGnuPlotEnum::PlotStyle
CQGnuPlotPlot::
plotStyle() const
{
  return CQGnuPlotUtil::plotStyleConv(CGnuPlotPlot::getStyle());
}

void
CQGnuPlotPlot::
setPlotStyle(const CQGnuPlotEnum::PlotStyle &s)
{
  CGnuPlotPlot::setStyle(CQGnuPlotUtil::plotStyleConv(s));
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
  CGnuPlotPlot::fillStyle_.setStyle(CQGnuPlotUtil::fillTypeConv(type));
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
  CGnuPlotPlot::fillStyle_.setPattern(CQGnuPlotUtil::fillPatternConv(pattern));
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
  CGnuPlotPlot::setPointType(int(CQGnuPlotEnum::symbolConv(type)));
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
  CGnuPlotPlot::setBoxWidthType(CQGnuPlotUtil::boxWidthTypeConv(type));
}

void
CQGnuPlotPlot::
draw()
{
  CGnuPlotPlot::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}

void
CQGnuPlotPlot::
mousePress(const CGnuPlotTypes::InsideData &insideData, Objects &objects)
{
  for (auto &arrow : arrowObjects()) {
    if (! arrow->inside(insideData))
      continue;

    CQGnuPlotArrowObject *qarrow = static_cast<CQGnuPlotArrowObject *>(arrow);

    objects.push_back(qarrow);
  }

  for (auto &bar : boxBarObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &box : boxObjects()) {
    if (! box->inside(insideData))
      continue;

    CQGnuPlotBoxObject *qbox = static_cast<CQGnuPlotBoxObject *>(box);

    objects.push_back(qbox);
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(insideData))
      continue;

    CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

    objects.push_back(qbubble);
  }

  for (auto &ellipse : ellipseObjects()) {
    if (! ellipse->inside(insideData))
      continue;

    CQGnuPlotEllipseObject *qellipse = static_cast<CQGnuPlotEllipseObject *>(ellipse);

    objects.push_back(qellipse);
  }

  for (auto &bar : errorBarObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotErrorBarObject *qbar = static_cast<CQGnuPlotErrorBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &bar : financeBarObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotFinanceBarObject *qbar = static_cast<CQGnuPlotFinanceBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &label : labelObjects()) {
    if (! label->inside(insideData))
      continue;

    CQGnuPlotLabelObject *qlabel = static_cast<CQGnuPlotLabelObject *>(label);

    objects.push_back(qlabel);
  }

  for (auto &path : pathObjects()) {
    if (path->inside(insideData)) {
      CQGnuPlotPathObject *qpath = static_cast<CQGnuPlotPathObject *>(path);

      objects.push_back(qpath);
    }
  }

  for (auto &pie : pieObjects()) {
    if (pie->inside(insideData) || pie->keyInside(insideData.window)) {
      CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

      objects.push_back(qpie);
    }
  }

  for (auto &point : pointObjects()) {
    if (! point->inside(insideData))
      continue;

    CQGnuPlotPointObject *qpoint = static_cast<CQGnuPlotPointObject *>(point);

    objects.push_back(qpoint);
  }

  for (auto &polygon : polygonObjects()) {
    if (! polygon->inside(insideData))
      continue;

    CQGnuPlotPolygonObject *qpolygon = static_cast<CQGnuPlotPolygonObject *>(polygon);

    objects.push_back(qpolygon);
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(insideData))
      continue;

    CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

    objects.push_back(qrect);
  }

  CGnuPlotPlot::mousePress(insideData);
}

void
CQGnuPlotPlot::
mouseMove(const CGnuPlotTypes::InsideData &)
{
}

bool
CQGnuPlotPlot::
mouseTip(const CGnuPlotTypes::InsideData &insideData, CGnuPlotTipData &tip)
{
  selectedPos_.setInvalid();

  for (auto &arrow : arrowObjects()) {
    if (! arrow->inside(insideData))
      continue;

    CQGnuPlotArrowObject *qarrow = static_cast<CQGnuPlotArrowObject *>(arrow);

    qwindow()->highlightObject(qarrow);

    tip = arrow->tip();

    return true;
  }

  for (auto &bar : boxBarObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &box : boxObjects()) {
    if (! box->inside(insideData))
      continue;

    CQGnuPlotBoxObject *qbox = static_cast<CQGnuPlotBoxObject *>(box);

    qwindow()->highlightObject(qbox);

    tip = box->tip();

    return true;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(insideData))
      continue;

    CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

    qwindow()->highlightObject(qbubble);

    tip = bubble->tip();

    return true;
  }

  for (auto &ellipse : ellipseObjects()) {
    if (! ellipse->inside(insideData))
      continue;

    CQGnuPlotEllipseObject *qellipse = static_cast<CQGnuPlotEllipseObject *>(ellipse);

    qwindow()->highlightObject(qellipse);

    tip = ellipse->tip();

    return true;
  }

  for (auto &bar : errorBarObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotErrorBarObject *qbar = static_cast<CQGnuPlotErrorBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &bar : financeBarObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotFinanceBarObject *qbar = static_cast<CQGnuPlotFinanceBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &label : labelObjects()) {
    if (! label->inside(insideData))
      continue;

    CQGnuPlotLabelObject *qlabel = static_cast<CQGnuPlotLabelObject *>(label);

    qwindow()->highlightObject(qlabel);

    tip = label->tip();

    return true;
  }

  for (auto &path : pathObjects()) {
    if (! path->inside(insideData))
      continue;

    CQGnuPlotPathObject *qpath = static_cast<CQGnuPlotPathObject *>(path);

    qwindow()->highlightObject(qpath);

    tip = path->tip();

    return true;
  }

  for (auto &pie : pieObjects()) {
    if (! pie->inside(insideData))
      continue;

    CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

    qwindow()->highlightObject(qpie);

    tip = pie->tip();

    return true;
  }

  for (auto &point : pointObjects()) {
    if (! point->inside(insideData))
      continue;

    CQGnuPlotPointObject *qpoint = static_cast<CQGnuPlotPointObject *>(point);

    qwindow()->highlightObject(qpoint);

    tip = point->tip();

    return true;
  }

  for (auto &polygon : polygonObjects()) {
    if (! polygon->inside(insideData))
      continue;

    CQGnuPlotPolygonObject *qpolygon = static_cast<CQGnuPlotPolygonObject *>(polygon);

    qwindow()->highlightObject(qpolygon);

    tip = polygon->tip();

    return true;
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(insideData))
      continue;

    CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

    qwindow()->highlightObject(qrect);

    tip = rect->tip();

    return true;
  }

  return CGnuPlotPlot::mouseTip(insideData, tip);
}

//------

QColor
CQGnuPlotPlotBoxBarObjects::
fillColor() const
{
  if (! plot_->boxBarObjects().empty()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);

    return qbar->fill()->color();
  }

  return QColor();
}

void
CQGnuPlotPlotBoxBarObjects::
setFillColor(const QColor &c)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->fill()->setColor(c);
  }
}

bool
CQGnuPlotPlotBoxBarObjects::
hasBorder() const
{
  if (! plot_->boxBarObjects().empty()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);

    return qbar->stroke()->isEnabled();
  }

  return false;
}

void
CQGnuPlotPlotBoxBarObjects::
setBorder(bool b)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->stroke()->setEnabled(b);
  }
}

QColor
CQGnuPlotPlotBoxBarObjects::
lineColor() const
{
  if (! plot_->boxBarObjects().empty()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);

    return qbar->stroke()->color();
  }

  return QColor();
}

void
CQGnuPlotPlotBoxBarObjects::
setLineColor(const QColor &c)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->stroke()->setColor(c);
  }
}

double
CQGnuPlotPlotBoxBarObjects::
lineWidth() const
{
  if (! plot_->boxBarObjects().empty()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);

    return qbar->stroke()->width();
  }

  return 0;
}

void
CQGnuPlotPlotBoxBarObjects::
setLineWidth(double r)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->stroke()->setWidth(r);
  }
}

CQGnuPlotEnum::FillType
CQGnuPlotPlotBoxBarObjects::
getFillType() const
{
  if (! plot_->boxBarObjects().empty()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);

    return qbar->fill()->type();
  }

  return CQGnuPlotEnum::FillType::FillNone;
}

void
CQGnuPlotPlotBoxBarObjects::
setFillType(const CQGnuPlotEnum::FillType &t)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->fill()->setType(t);
  }
}

CQGnuPlotEnum::FillPattern
CQGnuPlotPlotBoxBarObjects::
getFillPattern() const
{
  if (! plot_->boxBarObjects().empty()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(plot_->boxBarObjects()[0]);

    return qbar->fill()->pattern();
  }

  return CQGnuPlotEnum::FillPattern::PatternNone;
}

void
CQGnuPlotPlotBoxBarObjects::
setFillPattern(const CQGnuPlotEnum::FillPattern &p)
{
  for (auto &bar : plot_->boxBarObjects()) {
    CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

    qbar->fill()->setPattern(p);
  }
}

//------

double
CQGnuPlotPlotPieObjects::
innerRadius() const
{
  if (! plot_->pieObjects().empty())
    return plot_->pieObjects()[0]->innerRadius();

  return 0.0;
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
  if (! plot_->pieObjects().empty())
    return plot_->pieObjects()[0]->labelRadius();

  return 0.0;
}

void
CQGnuPlotPlotPieObjects::
setLabelRadius(double r)
{
  for (auto &pie : plot_->pieObjects())
    pie->setLabelRadius(r);
}
