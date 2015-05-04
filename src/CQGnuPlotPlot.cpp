#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotPieObject.h>
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

CQGnuPlotCanvas *
CQGnuPlotPlot::
canvas() const
{
  return qwindow()->canvas();
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

CQGnuPlot::PlotStyle
CQGnuPlotPlot::
plotStyle() const
{
  return CQGnuPlotUtil::plotStyleConv(CGnuPlotPlot::getStyle());
}

void
CQGnuPlotPlot::
setPlotStyle(const CQGnuPlot::PlotStyle &s)
{
  CGnuPlotPlot::setStyle(CQGnuPlotUtil::plotStyleConv(s));
}

CQGnuPlot::FillType
CQGnuPlotPlot::
fillType() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotPlot::fillType());
}

void
CQGnuPlotPlot::
setFillType(const CQGnuPlot::FillType &type)
{
  CGnuPlotPlot::setFillType(CQGnuPlotUtil::fillTypeConv(type));
}

CQGnuPlot::FillPattern
CQGnuPlotPlot::
fillPattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotPlot::fillPattern());
}

void
CQGnuPlotPlot::
setFillPattern(const CQGnuPlot::FillPattern &pattern)
{
  CGnuPlotPlot::setFillPattern(CQGnuPlotUtil::fillPatternConv(pattern));
}

CQGnuPlot::SymbolType
CQGnuPlotPlot::
pointType() const
{
  return CQGnuPlotUtil::symbolConv(CGnuPlotPlot::pointType());
}

void
CQGnuPlotPlot::
setPointType(const CQGnuPlot::SymbolType &type)
{
  CGnuPlotPlot::setPointType(CQGnuPlotUtil::symbolConv(type));
}

CQGnuPlot::BoxWidthType
CQGnuPlotPlot::
getBoxWidthType() const
{
  return CQGnuPlotUtil::boxWidthTypeConv(CGnuPlotPlot::getBoxWidthType());
}

void
CQGnuPlotPlot::
setBoxWidthType(const CQGnuPlot::BoxWidthType &type)
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
    if (! pie->inside(p))
      continue;

    CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

    objects.push_back(qpie);
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
}

void
CQGnuPlotPlot::
mouseMove(const CPoint2D &)
{
}

bool
CQGnuPlotPlot::
mouseTip(const CPoint2D &p, CQGnuPlot::TipRect &tip)
{
  selectedPos_.setInvalid();

  for (auto &bar : barObjects()) {
    if (! bar->inside(p))
      continue;

    tip.str  = bar->tip().c_str();
    tip.rect = CQUtil::toQRect(bar->tipRect());

    return true;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(p))
      continue;

    tip.str  = bubble->tip().c_str();
    tip.rect = CQUtil::toQRect(bubble->tipRect());

    return true;
  }

  for (auto &pie : pieObjects()) {
    if (! pie->inside(p))
      continue;

    tip.str  = pie->tip().c_str();
    tip.rect = CQUtil::toQRect(pie->tipRect());

    return true;
  }

  for (auto &rect : rectObjects()) {
    if (! rect->inside(p))
      continue;

    tip.str  = rect->tip().c_str();
    tip.rect = CQUtil::toQRect(rect->tipRect());

    return true;
  }

  return false;
}
