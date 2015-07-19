#include <CQGnuPlotAxis.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotAxis::
CQGnuPlotAxis(CQGnuPlotGroup *group, const CGnuPlotAxisData &data, double start, double end) :
 CGnuPlotAxis(group, data, start, end)
{
}

CQGnuPlotAxis::
~CQGnuPlotAxis()
{
}

QString
CQGnuPlotAxis::
getLabel() const
{
  return QString(CGnuPlotAxis::getLabel().c_str());
}

void
CQGnuPlotAxis::
setLabel(const QString &s)
{
  CGnuPlotAxis::setLabel(s.toStdString());
}

QString
CQGnuPlotAxis::
getTimeFormat() const
{
  return QString(CGnuPlotAxis::getTimeFormat().c_str());
}

void
CQGnuPlotAxis::
setTimeFormat(const QString &s)
{
  CGnuPlotAxis::setTimeFormat(s.toStdString());
}

void
CQGnuPlotAxis::
setStart1(double r)
{
  group_->setAxisStart1(type(), ind(), r);
}

void
CQGnuPlotAxis::
setEnd1(double r)
{
  group_->setAxisEnd1(type(), ind(), r);
}

CQGnuPlotEnum::DrawLayerType
CQGnuPlotAxis::
getGridLayer() const
{
  return CQGnuPlotUtil::drawLayerTypeConv(CGnuPlotAxis::getGridLayer());
}

void
CQGnuPlotAxis::
setGridLayer(const DrawLayerType &layer)
{
  CGnuPlotAxis::setGridLayer(CQGnuPlotUtil::drawLayerTypeConv(layer));
}

QFont
CQGnuPlotAxis::
getTicFont() const
{
  return CQUtil::toQFont(CGnuPlotAxis::ticFont());
}

void
CQGnuPlotAxis::
setTicFont(const QFont &f)
{
  CGnuPlotAxis::setTicFont(CQUtil::fromQFont(f));
}

QFont
CQGnuPlotAxis::
getLabelFont() const
{
  return CQUtil::toQFont(CGnuPlotAxis::labelFont());
}

void
CQGnuPlotAxis::
setLabelFont(const QFont &f)
{
  CGnuPlotAxis::setLabelFont(CQUtil::fromQFont(f));
}

QColor
CQGnuPlotAxis::
getTicColor() const
{
  const CGnuPlotColorSpec &cs = CGnuPlotAxis::ticColor();

  return toQColor(cs.color());
}

void
CQGnuPlotAxis::
setTicColor(const QColor &c)
{
  CGnuPlotColorSpec cs = CGnuPlotAxis::ticColor();

  cs.setRGB(fromQColor(c));

  CGnuPlotAxis::setTicColor(cs);
}

QColor
CQGnuPlotAxis::
getLabelColor() const
{
  const CGnuPlotColorSpec &cs = CGnuPlotAxis::labelColor();

  return toQColor(cs.color());
}

void
CQGnuPlotAxis::
setLabelColor(const QColor &c)
{
  CGnuPlotColorSpec cs = CGnuPlotAxis::labelColor();

  cs.setRGB(fromQColor(c));

  CGnuPlotAxis::setLabelColor(cs);
}

void
CQGnuPlotAxis::
drawAxes(CGnuPlotRenderer *renderer, bool drawOther)
{
  CGnuPlotAxis::drawAxes(renderer, drawOther);

  if (isSelected()) {
    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}
