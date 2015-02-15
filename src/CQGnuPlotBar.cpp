#include <CQGnuPlotBar.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotBar::
CQGnuPlotBar(CQGnuPlotPlot *plot) :
 CGnuPlotBar(plot)
{
}

CQGnuPlotBar::
~CQGnuPlotBar()
{
}

QColor
CQGnuPlotBar::
getLineColor() const
{
  return toQColor(CGnuPlotBar::lineColor());
}

void
CQGnuPlotBar::
setLineColor(const QColor &color)
{
  CGnuPlotBar::setLineColor(fromQColor(color));
}

QColor
CQGnuPlotBar::
getFillColor() const
{
  return toQColor(CGnuPlotBar::fillColor());
}

void
CQGnuPlotBar::
setFillColor(const QColor &color)
{
  CGnuPlotBar::setFillColor(fromQColor(color));
}

void
CQGnuPlotBar::
draw() const
{
  CQGnuPlotBar *th = const_cast<CQGnuPlotBar *>(this);

  bool lcSet = hasLineColor();
  bool fcSet = hasFillColor();

  CRGBA lc = (lcSet ? lineColor() : CRGBA(0,0,0));
  CRGBA fc = (fcSet ? fillColor() : CRGBA(1,0,0));

  if (isSelected()) {
    th->CGnuPlotBar::setLineColor(CRGBA(1,0,0));
    th->CGnuPlotBar::setFillColor(CRGBA(0.1,0));
  }

  CGnuPlotBar::draw();

  if (isSelected()) {
    if (lcSet) th->CGnuPlotBar::setLineColor(lc); else th->CGnuPlotBar::resetLineColor();
    if (fcSet) th->CGnuPlotBar::setFillColor(fc); else th->CGnuPlotBar::resetFillColor();
  }
}
