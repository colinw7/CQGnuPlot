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

CQGnuPlot::FillType
CQGnuPlotBar::
getFillType() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotBar::fillType());
}

void
CQGnuPlotBar::
setFillType(const CQGnuPlot::FillType &t)
{
  CGnuPlotTypes::FillType fillType = CQGnuPlotUtil::fillTypeConv(t);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBar *qbar = static_cast<CQGnuPlotBar *>(bar);

      qbar->CGnuPlotBar::setFillType(fillType);
    }
  }
  else
    CGnuPlotBar::setFillType(fillType);
}

CQGnuPlot::FillPattern
CQGnuPlotBar::
getFillPattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotBar::fillPattern());
}

void
CQGnuPlotBar::
setFillPattern(const CQGnuPlot::FillPattern &p)
{
  CGnuPlotTypes::FillPattern pattern = CQGnuPlotUtil::fillPatternConv(p);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBar *qbar = static_cast<CQGnuPlotBar *>(bar);

      qbar->CGnuPlotBar::setFillPattern(pattern);
    }
  }
  else
    CGnuPlotBar::setFillPattern(pattern);
}

QColor
CQGnuPlotBar::
getFillColor() const
{
  return toQColor(CGnuPlotBar::fillColor().getValue(CRGBA(1,1,1)));
}

void
CQGnuPlotBar::
setFillColor(const QColor &color)
{
  CRGBA c = fromQColor(color);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBar *qbar = static_cast<CQGnuPlotBar *>(bar);

      qbar->CGnuPlotBar::setFillColor(c);
    }
  }
  else
    CGnuPlotBar::setFillColor(c);
}

QColor
CQGnuPlotBar::
getLineColor() const
{
  return toQColor(CGnuPlotBar::lineColor().getValue(CRGBA(0,0,0)));
}

void
CQGnuPlotBar::
setLineColor(const QColor &color)
{
  CRGBA c = fromQColor(color);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBar *qbar = static_cast<CQGnuPlotBar *>(bar);

      qbar->CGnuPlotBar::setLineColor(c);
    }
  }
  else
    CGnuPlotBar::setLineColor(c);
}

void
CQGnuPlotBar::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBar::draw(renderer);

  if (isSelected()) {
    CGnuPlotBar bar1(*this);

    bar1.setFillType   (CGnuPlotTypes::FillType   ::PATTERN);
    bar1.setFillPattern(CGnuPlotTypes::FillPattern::HATCH);

    CRGBA fc(0,0,0);

    if (fillType() == CGnuPlotTypes::FillType::SOLID ||
        (fillType() == CGnuPlotTypes::FillType::PATTERN &&
         fillPattern() != CGnuPlotTypes::FillPattern::NONE)) {
      double g = fillColor().getValue(CRGBA(1,1,1)).getGray();

      if (g < 0.5)
        fc = CRGBA(1, 1, 1);
    }

    bar1.setFillColor(fc);

    bar1.setBorder   (true);
    bar1.setLineColor(CRGBA(1,0,0));

    bar1.draw(renderer);
  }
}
