#include <CQGnuPlotPointObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotPointObject::
CQGnuPlotPointObject(CQGnuPlotPlot *plot) :
 CGnuPlotPointObject(plot)
{
}

CQGnuPlotPointObject::
~CQGnuPlotPointObject()
{
}

QPointF
CQGnuPlotPointObject::
getPoint() const
{
  return CQUtil::toQPoint(CGnuPlotPointObject::point());
}

void
CQGnuPlotPointObject::
setPoint(const QPointF &p)
{
  CGnuPlotPointObject::setPoint(CQUtil::fromQPoint(p));
}

CQGnuPlotEnum::SymbolType
CQGnuPlotPointObject::
pointType() const
{
  return CQGnuPlotEnum::symbolConv(CGnuPlotPointObject::pointType());
}

void
CQGnuPlotPointObject::
setPointType(const CQGnuPlotEnum::SymbolType &type)
{
  CGnuPlotPointObject::setPointType(CQGnuPlotEnum::symbolConv(type));
}

double
CQGnuPlotPointObject::
getSize() const
{
  return CGnuPlotPointObject::size().getValue(1);
}

void
CQGnuPlotPointObject::
setSize(double s)
{
  CGnuPlotPointObject::setSize(s);
}

QColor
CQGnuPlotPointObject::
getColor() const
{
  return toQColor(CGnuPlotPointObject::color());
}

void
CQGnuPlotPointObject::
setColor(const QColor &color)
{
  CGnuPlotPointObject::setColor(fromQColor(color));
}

QString
CQGnuPlotPointObject::
getPointString() const
{
  return QString(CGnuPlotPointObject::pointString().c_str());
}

void
CQGnuPlotPointObject::
setPointString(const QString &name)
{
  CGnuPlotPointObject::setPointString(name.toStdString());
}

void
CQGnuPlotPointObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotPointObject::draw(renderer);
}
