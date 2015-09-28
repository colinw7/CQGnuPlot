#include <CQGnuPlotArrowObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotArrowObject::
CQGnuPlotArrowObject(CQGnuPlotPlot *plot) :
 CGnuPlotArrowObject(plot)
{
}

CQGnuPlotArrowObject::
~CQGnuPlotArrowObject()
{
}

CQGnuPlotEnum::ArrowCoordType
CQGnuPlotArrowObject::
getCoordType() const
{
  return CQGnuPlotUtil::arrowCoordTypeConv(CGnuPlotArrowObject::coordType());
}

void
CQGnuPlotArrowObject::
setCoordType(CQGnuPlotEnum::ArrowCoordType type)
{
  CGnuPlotArrowObject::setCoordType(CQGnuPlotUtil::arrowCoordTypeConv(type));
}

QPointF
CQGnuPlotArrowObject::
getFrom() const
{
  CPoint3D p = CGnuPlotArrowObject::getFrom().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotArrowObject::
setFrom(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotArrowObject::setFrom(CPoint3D(p1.x, p1.y, 0));
}

QPointF
CQGnuPlotArrowObject::
getTo() const
{
  CPoint3D p = CGnuPlotArrowObject::getTo().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotArrowObject::
setTo(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotArrowObject::setTo(CPoint3D(p1.x, p1.y, 0));
}

double
CQGnuPlotArrowObject::
getLength() const
{
  return CGnuPlotArrowObject::getLength().value();
}

void
CQGnuPlotArrowObject::
setLength(double l)
{
  CGnuPlotArrowObject::setLength(l);
}

double
CQGnuPlotArrowObject::
getHeadLength() const
{
  return CGnuPlotArrowObject::getHeadLength().value();
}

void
CQGnuPlotArrowObject::
setHeadLength(double l)
{
  CGnuPlotArrowObject::setHeadLength(l);
}

CLineDash
CQGnuPlotArrowObject::
getDash() const
{
  return CGnuPlotArrowObject::getDash().calcDash(plot_->group()->app(), CLineDash());
}

void
CQGnuPlotArrowObject::
setDash(const CLineDash &dash)
{
  CGnuPlotArrowObject::setDash(CGnuPlotDash(dash));
}

void
CQGnuPlotArrowObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotArrowObject::draw(renderer);
}
