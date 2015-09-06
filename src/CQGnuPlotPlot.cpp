#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotRectObject.h>
#include <CQGnuPlotPointObject.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotPlot::
CQGnuPlotPlot(CQGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style) :
 CGnuPlotPlot(group, style), group_(group)
{
  setObjectName("plot");

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
mousePress(const CPoint2D &p, std::vector<CQGnuPlotObject *> &objects)
{
  for (auto &bar : barObjects()) {
    if (! bar->inside(p))
      continue;

    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    objects.push_back(qbar);
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(p))
      continue;

    CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

    objects.push_back(qbubble);
  }

  for (auto &ellipse : ellipseObjects()) {
    if (! ellipse->inside(p))
      continue;

    CQGnuPlotEllipseObject *qellipse = static_cast<CQGnuPlotEllipseObject *>(ellipse);

    objects.push_back(qellipse);
  }

  for (auto &pie : pieObjects()) {
    if (pie->inside(p) || pie->keyInside(p)) {
      CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

      objects.push_back(qpie);
    }
  }

  for (auto &polygon : polygonObjects()) {
    if (! polygon->inside(p))
      continue;

    CQGnuPlotPolygonObject *qpolygon = static_cast<CQGnuPlotPolygonObject *>(polygon);

    objects.push_back(qpolygon);
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(p))
      continue;

    CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

    objects.push_back(qrect);
  }

  for (auto &point : pointObjects()) {
    if (! point->inside(p))
      continue;

    CQGnuPlotPointObject *qpoint = static_cast<CQGnuPlotPointObject *>(point);

    objects.push_back(qpoint);
  }
}

void
CQGnuPlotPlot::
mouseMove(const CPoint2D &)
{
}

bool
CQGnuPlotPlot::
mouseTip(const CPoint2D &p, CGnuPlotTipData &tip)
{
  selectedPos_.setInvalid();

  for (auto &bar : barObjects()) {
    if (! bar->inside(p))
      continue;

    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qwindow()->highlightObject(qbar);

    tip = bar->tip();

    return true;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(p))
      continue;

    CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

    qwindow()->highlightObject(qbubble);

    tip = bubble->tip();

    return true;
  }

  for (auto &pie : pieObjects()) {
    if (! pie->inside(p))
      continue;

    CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

    qwindow()->highlightObject(qpie);

    tip = pie->tip();

    return true;
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(p))
      continue;

    CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

    qwindow()->highlightObject(qrect);

    tip = rect->tip();

    return true;
  }

  for (auto &point : pointObjects()) {
    if (! point->inside(p))
      continue;

    CQGnuPlotPointObject *qpoint = static_cast<CQGnuPlotPointObject *>(point);

    qwindow()->highlightObject(qpoint);

    tip = point->tip();

    return true;
  }

  return CGnuPlotPlot::mouseTip(p, tip);
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
