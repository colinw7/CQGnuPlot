#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotPolygonObject::
CQGnuPlotPolygonObject(CQGnuPlotPlot *plot) :
 CGnuPlotPolygonObject(plot)
{
}

CQGnuPlotPolygonObject::
~CQGnuPlotPolygonObject()
{
}

QString
CQGnuPlotPolygonObject::
getText() const
{
  return QString(CGnuPlotPolygonObject::text().c_str());
}

void
CQGnuPlotPolygonObject::
setText(const QString &text)
{
  CGnuPlotPolygonObject::setText(text.toStdString());
}

QColor
CQGnuPlotPolygonObject::
getFillColor() const
{
  return toQColor(CGnuPlotPolygonObject::fillColor());
}

void
CQGnuPlotPolygonObject::
setFillColor(const QColor &color)
{
  CGnuPlotPolygonObject::setFillColor(fromQColor(color));
}

QColor
CQGnuPlotPolygonObject::
getLineColor() const
{
  return toQColor(CGnuPlotPolygonObject::lineColor());
}

void
CQGnuPlotPolygonObject::
setLineColor(const QColor &color)
{
  CGnuPlotPolygonObject::setLineColor(fromQColor(color));
}

void
CQGnuPlotPolygonObject::
draw(CGnuPlotRenderer *renderer) const
{
  CQGnuPlotPolygonObject *th = const_cast<CQGnuPlotPolygonObject *>(this);

  CRGBA c = lineColor();

  if (isSelected())
    th->CGnuPlotPolygonObject::setLineColor(CRGBA(1,0,0));

  CGnuPlotPolygonObject::draw(renderer);

  if (isSelected())
    th->CGnuPlotPolygonObject::setLineColor(c);
}
