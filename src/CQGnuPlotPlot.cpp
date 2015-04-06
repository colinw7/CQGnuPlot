#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotBar.h>
#include <CQGnuPlotPie.h>
#include <CQGnuPlotBubble.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotPlot::
CQGnuPlotPlot(CQGnuPlotGroup *group) :
 CGnuPlotPlot(group), group_(group)
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

    CQGnuPlotBar *qbar = static_cast<CQGnuPlotBar *>(bar);

    qwindow()->selectObject(qbar);

    return;
  }

  for (auto &pie : pieObjects()) {
    if (! pie->inside(p))
      continue;

    CQGnuPlotPie *qpie = static_cast<CQGnuPlotPie *>(pie);

    qwindow()->selectObject(qpie);

    return;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(p))
      continue;

    CQGnuPlotBubble *qbubble = static_cast<CQGnuPlotBubble *>(bubble);

    qwindow()->selectObject(qbubble);

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

    tip.str  = QString("%1").arg(bar->value());
    tip.rect = CQUtil::toQRect(bar->bbox());

    return true;
  }

  for (auto &pie : pieObjects()) {
    if (! pie->inside(p))
      continue;

    const CPoint2D &c = pie->center();
    double          r = pie->radius();

    CBBox2D bbox(c - CPoint2D(r, r), c + CPoint2D(r, r));

    tip.str  = QString("%1").arg(pie->name().c_str());
    tip.rect = CQUtil::toQRect(bbox);

    return true;
  }

  for (auto &bubble : bubbleObjects()) {
    if (! bubble->inside(p))
      continue;

    const CPoint2D &c  = bubble->center();
    double          xr = bubble->xRadius();
    double          yr = bubble->yRadius();

    CBBox2D bbox(c - CPoint2D(xr, yr), c + CPoint2D(xr, yr));

    tip.str  = QString("%1").arg(bubble->name().c_str());
    tip.rect = CQUtil::toQRect(bbox);

    return true;
  }

  return false;
}
