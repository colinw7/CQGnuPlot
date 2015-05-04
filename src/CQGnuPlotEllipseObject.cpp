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
  CQGnuPlotEllipseObject *th = const_cast<CQGnuPlotEllipseObject *>(this);

  COptRGBA c;

  if (hasLineColor())
    c = lineColor();

  if (isSelected())
    th->CGnuPlotEllipseObject::setLineColor(CRGBA(1,0,0));

  CGnuPlotEllipseObject::draw(renderer);

  if (isSelected()) {
    if (c.isValid())
      th->CGnuPlotEllipseObject::setLineColor(c.getValue());
    else
      th->CGnuPlotEllipseObject::resetLineColor();
  }
}
