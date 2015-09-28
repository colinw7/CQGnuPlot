#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotEllipseObject::
CQGnuPlotEllipseObject(CQGnuPlotPlot *plot) :
 CGnuPlotEllipseObject(plot)
{
}

CQGnuPlotEllipseObject::
~CQGnuPlotEllipseObject()
{
}

QString
CQGnuPlotEllipseObject::
getText() const
{
  return QString(CGnuPlotEllipseObject::text().c_str());
}

void
CQGnuPlotEllipseObject::
setText(const QString &text)
{
  CGnuPlotEllipseObject::setText(text.toStdString());
}

QColor
CQGnuPlotEllipseObject::
getFillColor() const
{
  return toQColor(CGnuPlotEllipseObject::fillColor());
}

void
CQGnuPlotEllipseObject::
setFillColor(const QColor &color)
{
  CGnuPlotEllipseObject::setFillColor(fromQColor(color));
}

QColor
CQGnuPlotEllipseObject::
getLineColor() const
{
  return toQColor(CGnuPlotEllipseObject::lineColor());
}

void
CQGnuPlotEllipseObject::
setLineColor(const QColor &color)
{
  CGnuPlotEllipseObject::setLineColor(fromQColor(color));
}

void
CQGnuPlotEllipseObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotEllipseObject::draw(renderer);
}
