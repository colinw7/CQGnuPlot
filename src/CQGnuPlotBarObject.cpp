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

  CGnuPlotBarObject::setLineColor(c);
}

void
CQGnuPlotBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBarObject::draw(renderer);
}
