#include <CQGnuPlotRectObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotRectObject::
CQGnuPlotRectObject(CQGnuPlotPlot *plot) :
 CGnuPlotRectObject(plot)
{
}

CQGnuPlotRectObject::
~CQGnuPlotRectObject()
{
}

QString
CQGnuPlotRectObject::
getText() const
{
  return QString(CGnuPlotRectObject::text().c_str());
}

void
CQGnuPlotRectObject::
setText(const QString &text)
{
  CGnuPlotRectObject::setText(text.toStdString());
}

QColor
CQGnuPlotRectObject::
getFillColor() const
{
  return toQColor(CGnuPlotRectObject::fillColor());
}

void
CQGnuPlotRectObject::
setFillColor(const QColor &color)
{
  CGnuPlotRectObject::setFillColor(fromQColor(color));
}

QColor
CQGnuPlotRectObject::
getLineColor() const
{
  return toQColor(CGnuPlotRectObject::lineColor());
}

void
CQGnuPlotRectObject::
setLineColor(const QColor &color)
{
  CGnuPlotRectObject::setLineColor(fromQColor(color));
}

void
CQGnuPlotRectObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotRectObject::draw(renderer);
}
