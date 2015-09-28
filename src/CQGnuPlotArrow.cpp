#include <CQGnuPlotArrow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
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
  return CQGnuPlotUtil::arrowCoordTypeConv(CGnuPlotArrow::coordType());
}

void
CQGnuPlotArrow::
setCoordType(CQGnuPlotEnum::ArrowCoordType type)
{
  CGnuPlotArrow::setCoordType(CQGnuPlotUtil::arrowCoordTypeConv(type));
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
getDash() const
{
  return CGnuPlotArrow::getDash().calcDash(group_->app(), CLineDash());
}

void
CQGnuPlotArrow::
setDash(const CLineDash &dash)
{
  CGnuPlotArrow::setDash(CGnuPlotDash(dash));
}

void
CQGnuPlotArrow::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotArrow::draw(renderer);
}
