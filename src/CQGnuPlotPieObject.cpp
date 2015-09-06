#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotPieObject::
CQGnuPlotPieObject(CQGnuPlotPlot *plot) :
 CGnuPlotPieObject(plot)
{
}

CQGnuPlotPieObject::
~CQGnuPlotPieObject()
{
}

QString
CQGnuPlotPieObject::
getName() const
{
  return QString(CGnuPlotPieObject::name().c_str());
}

void
CQGnuPlotPieObject::
setName(const QString &name)
{
  CGnuPlotPieObject::setName(name.toStdString());
}

QColor
CQGnuPlotPieObject::
getLineColor() const
{
  return toQColor(CGnuPlotPieObject::lineColor());
}

void
CQGnuPlotPieObject::
setLineColor(const QColor &color)
{
  CGnuPlotPieObject::setLineColor(fromQColor(color));
}

QColor
CQGnuPlotPieObject::
getFillColor() const
{
  return toQColor(CGnuPlotPieObject::fillColor());
}

void
CQGnuPlotPieObject::
setFillColor(const QColor &color)
{
  CGnuPlotPieObject::setFillColor(fromQColor(color));
}

void
CQGnuPlotPieObject::
draw(CGnuPlotRenderer *renderer) const
{
  CQGnuPlotPieObject *th = const_cast<CQGnuPlotPieObject *>(this);

  CRGBA fc = fillColor();
  bool  e  = isExploded();

  if (isHighlighted() || isSelected()) {
    CRGBA fc1 = fc.getLightRGBA();

    th->CGnuPlotPieObject::setFillColor(fc1);

    if (isSelected())
      th->CGnuPlotPieObject::setExploded(true);
  }

  CGnuPlotPieObject::draw(renderer);

  if (isHighlighted() || isSelected()) {
    th->CGnuPlotPieObject::setFillColor(fc);

    if (isSelected())
      th->CGnuPlotPieObject::setExploded(e);
  }
}
