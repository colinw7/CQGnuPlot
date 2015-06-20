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
  CQGnuPlotPolygonObject *th = const_cast<CQGnuPlotPolygonObject *>(this);

  bool hasLineColor = th->CGnuPlotPolygonObject::hasLineColor();

  CRGBA c;

  if (hasLineColor)
    c = lineColor();

  if (isSelected())
    th->CGnuPlotPolygonObject::setLineColor(CRGBA(1,0,0));

  CGnuPlotPolygonObject::draw(renderer);

  if (isSelected()) {
    if (hasLineColor)
      th->CGnuPlotPolygonObject::setLineColor(c);
    else
      th->CGnuPlotPolygonObject::resetLineColor();
  }
}
