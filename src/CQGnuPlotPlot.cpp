#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>

#include <CQGnuPlotArrowObject.h>
#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotLabelObject.h>
#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPointObject.h>
#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotRectObject.h>

#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotPlot::
CQGnuPlotPlot(CQGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style) :
 CGnuPlotPlot(group, style), group_(group)
{
  setObjectName("plot");

  // QObject to set properties for collections of bar and pieobjects
  barObjects_ = new CQGnuPlotPlotBarObjects(this);
  pieObjects_ = new CQGnuPlotPlotPieObjects(this);
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
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotPlot::fillType());
}

void
CQGnuPlotPlot::
setFillType(const CQGnuPlotEnum::FillType &type)
{
  CGnuPlotPlot::setFillType(CQGnuPlotUtil::fillTypeConv(type));
}

CQGnuPlotEnum::FillPattern
CQGnuPlotPlot::
fillPattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotPlot::fillPattern());
}

void
CQGnuPlotPlot::
setFillPattern(const CQGnuPlotEnum::FillPattern &pattern)
{
  CGnuPlotPlot::setFillPattern(CQGnuPlotUtil::fillPatternConv(pattern));
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
mousePress(const CPoint2D &pixel, const CPoint2D &window, Objects &objects)
{
  CGnuPlotTypes::InsideData insideData(window, pixel);

  //---

  for (auto &arrow : arrowObjects()) {
    if (! arrow->inside(insideData))
      continue;

    CQGnuPlotArrowObject *qarrow = static_cast<CQGnuPlotArrowObject *>(arrow);

    objects.push_back(qarrow);
  }

  for (auto &bar : barObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    objects.push_back(qbar);
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

  for (auto &label : labelObjects()) {
    if (! label->inside(insideData))
      continue;

    CQGnuPlotLabelObject *qlabel = static_cast<CQGnuPlotLabelObject *>(label);

    objects.push_back(qlabel);
  }

  for (auto &pie : pieObjects()) {
    if (pie->inside(insideData) || pie->keyInside(window)) {
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
}

void
CQGnuPlotPlot::
mouseMove(const CPoint2D &, const CPoint2D &)
{
}

bool
CQGnuPlotPlot::
mouseTip(const CPoint2D &pixel, const CPoint2D &window, CGnuPlotTipData &tip)
{
  CGnuPlotTypes::InsideData insideData(window, pixel);

  //---

  selectedPos_.setInvalid();

  for (auto &arrow : arrowObjects()) {
    if (! arrow->inside(insideData))
      continue;

    CQGnuPlotArrowObject *qarrow = static_cast<CQGnuPlotArrowObject *>(arrow);

    qwindow()->highlightObject(qarrow);

    tip = arrow->tip();

    return true;
  }

  for (auto &bar : barObjects()) {
    if (! bar->inside(insideData))
      continue;

    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

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

  for (auto &label : labelObjects()) {
    if (! label->inside(insideData))
      continue;

    CQGnuPlotLabelObject *qlabel = static_cast<CQGnuPlotLabelObject *>(label);

    qwindow()->highlightObject(qlabel);

    tip = label->tip();

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

  return CGnuPlotPlot::mouseTip(window, tip);
}

//------

QColor
CQGnuPlotPlotBarObjects::
fillColor() const
{
  if (! plot_->barObjects().empty()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(plot_->barObjects()[0]);

    return qbar->getFillColor();
  }

  return QColor();
}

void
CQGnuPlotPlotBarObjects::
setFillColor(const QColor &c)
{
  for (auto &bar : plot_->barObjects()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qbar->setFillColor(c);
  }
}

bool
CQGnuPlotPlotBarObjects::
hasBorder() const
{
  if (! plot_->barObjects().empty()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(plot_->barObjects()[0]);

    return qbar->hasBorder();
  }

  return false;
}

void
CQGnuPlotPlotBarObjects::
setBorder(bool b)
{
  for (auto &bar : plot_->barObjects()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qbar->setBorder(b);
  }
}

QColor
CQGnuPlotPlotBarObjects::
lineColor() const
{
  if (! plot_->barObjects().empty()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(plot_->barObjects()[0]);

    return qbar->getLineColor();
  }

  return QColor();
}

void
CQGnuPlotPlotBarObjects::
setLineColor(const QColor &c)
{
  for (auto &bar : plot_->barObjects()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qbar->setLineColor(c);
  }
}

CQGnuPlotEnum::FillType
CQGnuPlotPlotBarObjects::
getFillType() const
{
  if (! plot_->barObjects().empty()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(plot_->barObjects()[0]);

    return qbar->getFillType();
  }

  return CQGnuPlotEnum::FillType::FillNone;
}

void
CQGnuPlotPlotBarObjects::
setFillType(const CQGnuPlotEnum::FillType &t)
{
  for (auto &bar : plot_->barObjects()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qbar->setFillType(t);
  }
}

CQGnuPlotEnum::FillPattern
CQGnuPlotPlotBarObjects::
getFillPattern() const
{
  if (! plot_->barObjects().empty()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(plot_->barObjects()[0]);

    return qbar->getFillPattern();
  }

  return CQGnuPlotEnum::FillPattern::PatternNone;
}

void
CQGnuPlotPlotBarObjects::
setFillPattern(const CQGnuPlotEnum::FillPattern &p)
{
  for (auto &bar : plot_->barObjects()) {
    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qbar->setFillPattern(p);
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
