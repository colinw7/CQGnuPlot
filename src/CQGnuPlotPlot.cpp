#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotPieObject.h>
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
mousePress(const CPoint2D &p)
{
  for (auto &bar : barObjects()) {
    if (! bar->inside(p))
      continue;

    CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

    qwindow()->selectObject(qbar);

    return;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(p))
      continue;

    CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

    qwindow()->selectObject(qbubble);

    return;
  }
  for (auto &pie : pieObjects()) {
    if (! pie->inside(p))
      continue;

    CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

    qwindow()->selectObject(qpie);

    return;
  }


  for (auto &rect : rectObjects()) {
    if (! rect->inside(p))
      continue;

    CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

    qwindow()->selectObject(qrect);

    return;
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
