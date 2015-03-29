#include <CQGnuPlotCircle.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotCircle::
CQGnuPlotCircle(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotCircle(group)
{
  setObjectName("circle");
}

QPointF
CQGnuPlotCircle::
getCenter() const
{
  CPoint2D center = CGnuPlotCircle::getCenter().point();

  return CQUtil::toQPoint(center);
}

void
CQGnuPlotCircle::
setCenter(const QPointF &p)
{
  CGnuPlotCircle::setCenter(CQUtil::fromQPoint(p));
}

void
CQGnuPlotCircle::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotCircle::draw(renderer);

  if (isSelected()) {
    CPoint2D center = CGnuPlotCircle::getCenter().getPoint(renderer);

    renderer->drawEllipse(center, getRadius(), getRadius(), 0, CRGBA(1,0,0), 2);
  }
}
