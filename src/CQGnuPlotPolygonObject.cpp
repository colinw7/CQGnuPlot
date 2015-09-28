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

  CGnuPlotPolygonObject::setModified(true);
}

QColor
CQGnuPlotPolygonObject::
getFillColor() const
{
  if (CGnuPlotPolygonObject::hasFillColor())
    return toQColor(CGnuPlotPolygonObject::fillColor());
  else
    return QColor();
}

void
CQGnuPlotPolygonObject::
setFillColor(const QColor &color)
{
  CGnuPlotPolygonObject::setFillColor(fromQColor(color));

  CGnuPlotPolygonObject::setModified(true);
}

QColor
CQGnuPlotPolygonObject::
getLineColor() const
{
  if (CGnuPlotPolygonObject::hasLineColor())
    return toQColor(CGnuPlotPolygonObject::lineColor());
  else
    return QColor();
}

void
CQGnuPlotPolygonObject::
setLineColor(const QColor &color)
{
  CGnuPlotPolygonObject::setLineColor(fromQColor(color));

  CGnuPlotPolygonObject::setModified(true);
}

void
CQGnuPlotPolygonObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotPolygonObject::draw(renderer);
}
