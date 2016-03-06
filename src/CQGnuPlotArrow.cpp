#include <CQGnuPlotArrow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotEnum.h>
#include <CQUtil.h>

CQGnuPlotArrow::
CQGnuPlotArrow(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(group, this), CGnuPlotArrow(group)
{
  setObjectName("arrow");
}

CQGnuPlotEnum::ArrowCoordType
CQGnuPlotArrow::
getCoordType() const
{
  return CQGnuPlotEnum::arrowCoordTypeConv(CGnuPlotArrow::coordType());
}

void
CQGnuPlotArrow::
setCoordType(CQGnuPlotEnum::ArrowCoordType type)
{
  CGnuPlotArrow::setCoordType(CQGnuPlotEnum::arrowCoordTypeConv(type));
}

QPointF
CQGnuPlotArrow::
getFrom() const
{
  CPoint3D p = CGnuPlotArrow::getFrom().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotArrow::
setFrom(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotArrow::setFrom(CPoint3D(p1.x, p1.y, 0));
}

QPointF
CQGnuPlotArrow::
getTo() const
{
  CPoint3D p = CGnuPlotArrow::getTo().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotArrow::
setTo(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotArrow::setTo(CPoint3D(p1.x, p1.y, 0));
}

double
CQGnuPlotArrow::
getLength() const
{
  return CGnuPlotArrow::getLength().value();
}

void
CQGnuPlotArrow::
setLength(double l)
{
  CGnuPlotArrow::setLength(l);
}

double
CQGnuPlotArrow::
getHeadLength() const
{
  return CGnuPlotArrow::getHeadLength().value();
}

void
CQGnuPlotArrow::
setHeadLength(double l)
{
  CGnuPlotArrow::setHeadLength(l);
}

CLineDash
CQGnuPlotArrow::
getLineDash() const
{
  return CGnuPlotArrow::getLineDash().calcDash(group_->app(), CLineDash());
}

void
CQGnuPlotArrow::
setLineDash(const CLineDash &dash)
{
  CGnuPlotArrow::setLineDash(CGnuPlotDash(dash));
}

void
CQGnuPlotArrow::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotArrow::draw(renderer);
}
