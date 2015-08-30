#include <CQGnuPlotBarObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotBarObject::
CQGnuPlotBarObject(CQGnuPlotPlot *plot) :
 CGnuPlotBarObject(plot)
{
}

CQGnuPlotBarObject::
~CQGnuPlotBarObject()
{
}

CQGnuPlotEnum::FillType
CQGnuPlotBarObject::
getFillType() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotBarObject::fillType());
}

void
CQGnuPlotBarObject::
setFillType(const CQGnuPlotEnum::FillType &t)
{
  CGnuPlotTypes::FillType fillType = CQGnuPlotUtil::fillTypeConv(t);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

      qbar->CGnuPlotBarObject::setFillType(fillType);
    }
  }
  else
    CGnuPlotBarObject::setFillType(fillType);
}

CQGnuPlotEnum::FillPattern
CQGnuPlotBarObject::
getFillPattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotBarObject::fillPattern());
}

void
CQGnuPlotBarObject::
setFillPattern(const CQGnuPlotEnum::FillPattern &p)
{
  CGnuPlotTypes::FillPattern pattern = CQGnuPlotUtil::fillPatternConv(p);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

      qbar->CGnuPlotBarObject::setFillPattern(pattern);
    }
  }
  else
    CGnuPlotBarObject::setFillPattern(pattern);
}

QColor
CQGnuPlotBarObject::
getFillColor() const
{
  return toQColor(CGnuPlotBarObject::fillColor().getValue(CRGBA(1,1,1)));
}

void
CQGnuPlotBarObject::
setFillColor(const QColor &color)
{
  CRGBA c = fromQColor(color);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

      qbar->CGnuPlotBarObject::setFillColor(c);
    }
  }
  else
    CGnuPlotBarObject::setFillColor(c);
}

QColor
CQGnuPlotBarObject::
getLineColor() const
{
  return toQColor(CGnuPlotBarObject::lineColor().getValue(CRGBA(0,0,0)));
}

void
CQGnuPlotBarObject::
setLineColor(const QColor &color)
{
  CRGBA c = fromQColor(color);

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  if (qplot->syncBars()) {
    for (const auto &bar : qplot->barObjects()) {
      CQGnuPlotBarObject *qbar = static_cast<CQGnuPlotBarObject *>(bar);

      qbar->CGnuPlotBarObject::setLineColor(c);
    }
  }
  else
    CGnuPlotBarObject::setLineColor(c);
}

void
CQGnuPlotBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBarObject::draw(renderer);

  if (isSelected() || isHighlighted()) {
    CGnuPlotBarObject bar1(*this);

    if      (fillType() == CGnuPlotTypes::FillType::SOLID) {
      CRGBA fc  = fillColor().getValue(CRGBA(1,1,1));
      CRGBA fc1 = fc.getLightRGBA();

      bar1.setFillColor(fc1);
    }
    else if (fillType() == CGnuPlotTypes::FillType::PATTERN &&
             fillPattern() != CGnuPlotTypes::FillPattern::NONE) {
      bar1.setFillType   (CGnuPlotTypes::FillType   ::PATTERN);
      bar1.setFillPattern(CGnuPlotTypes::FillPattern::HATCH);

      CRGBA fc(0,0,0);

      double g = fillColor().getValue(CRGBA(1,1,1)).getGray();

      if (g < 0.5)
        fc = CRGBA(1, 1, 1);

      bar1.setFillColor(fc);
    }
    else {
      bar1.setBorder   (true);
      bar1.setLineColor(CRGBA(1,0,0));
    }

    bar1.draw(renderer);
  }
}
