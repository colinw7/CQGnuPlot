#include <CQGnuPlotRectangle.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotRectangle::
CQGnuPlotRectangle(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(group, this), CGnuPlotRectangle(group)
{
  setObjectName("rectangle");
}

QPointF
CQGnuPlotRectangle::
getFrom() const
{
  CBBox2D bbox = CGnuPlotRectangle::calcBBox();

  return CQUtil::toQPoint(bbox.getLL());
}

void
CQGnuPlotRectangle::
setFrom(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotRectangle::setFrom(CPoint3D(p1.x, p1.y, 0));
}

QPointF
CQGnuPlotRectangle::
getTo() const
{
  CBBox2D bbox = CGnuPlotRectangle::calcBBox();

  return CQUtil::toQPoint(bbox.getUR());
}

void
CQGnuPlotRectangle::
setTo(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotRectangle::setTo(CPoint3D(p1.x, p1.y, 0));
}

double
CQGnuPlotRectangle::
getLineWidth() const
{
  return CGnuPlotRectangle::getLineWidth().getValue(0);
}

CLineDash
CQGnuPlotRectangle::
getLineDash() const
{
  return CGnuPlotRectangle::getLineDash();
}

void
CQGnuPlotRectangle::
setLineDash(const CLineDash &dash)
{
  CGnuPlotRectangle::setLineDash(dash);
}

CQGnuPlotEnum::FillType
CQGnuPlotRectangle::
fillType() const
{
  return CQGnuPlotEnum::fillTypeConv(CGnuPlotRectangle::getFillStyle().style());
}

void
CQGnuPlotRectangle::
setFillType(const CQGnuPlotEnum::FillType &type)
{
  CGnuPlotFillStyle fs = CGnuPlotRectangle::getFillStyle();

  fs.setStyle(CQGnuPlotEnum::fillTypeConv(type));

  CGnuPlotRectangle::setFillStyle(fs);
}

void
CQGnuPlotRectangle::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotRectangle::draw(renderer);
}
