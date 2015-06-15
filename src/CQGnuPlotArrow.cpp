#include <CQGnuPlotArrow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotArrow::
CQGnuPlotArrow(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotArrow(group)
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
  return CQUtil::toQPoint(CGnuPlotArrow::getFrom().point());
}

void
CQGnuPlotArrow::
setFrom(const QPointF &p)
{
  CGnuPlotArrow::setFrom(CQUtil::fromQPoint(p));
}

QPointF
CQGnuPlotArrow::
getTo() const
{
  return CQUtil::toQPoint(CGnuPlotArrow::getTo().point());
}

void
CQGnuPlotArrow::
setTo(const QPointF &p)
{
  CGnuPlotArrow::setTo(CQUtil::fromQPoint(p));
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

void
CQGnuPlotArrow::
draw(CGnuPlotRenderer *renderer) const
{
  CQGnuPlotArrow *th = const_cast<CQGnuPlotArrow *>(this);

  th->qfrom_ = CQUtil::toQPoint(CGnuPlotArrow::getFrom().getPoint(renderer));
  th->qto_   = CQUtil::toQPoint(CGnuPlotArrow::getTo  ().getPoint(renderer));

  CRGBA c = getLineColor();

  if (isSelected())
    th->setLineColor(CRGBA(1,0,0), true);

  CGnuPlotArrow::draw(renderer);

  if (isSelected())
    th->setLineColor(CRGBA(1,0,0), false);
}
