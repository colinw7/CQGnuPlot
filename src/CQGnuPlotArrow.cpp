#include <CQGnuPlotArrow.h>
#include <CQGnuPlotGroup.h>
#include <CQUtil.h>

CQGnuPlotArrow::
CQGnuPlotArrow(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotArrow(group)
{
  setObjectName("arrow");
}

QPointF
CQGnuPlotArrow::
getFrom() const
{
  return CQUtil::toQPoint(CGnuPlotArrow::getFrom());
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
  return CQUtil::toQPoint(CGnuPlotArrow::getTo());
}

void
CQGnuPlotArrow::
setTo(const QPointF &p)
{
  CGnuPlotArrow::setTo(CQUtil::fromQPoint(p));
}

void
CQGnuPlotArrow::
draw() const
{
  CQGnuPlotArrow *th = const_cast<CQGnuPlotArrow *>(this);

  CRGBA c = getLineColor();

  if (isSelected())
    th->setLineColor(CRGBA(1,0,0), true);

  CGnuPlotArrow::draw();

  if (isSelected())
    th->setLineColor(CRGBA(1,0,0), false);
}
