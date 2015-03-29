#include <CQGnuPlotEllipse.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotEllipse::
CQGnuPlotEllipse(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotEllipse(group)
{
  setObjectName("ellipse");
}

QPointF
CQGnuPlotEllipse::
getCenter() const
{
  CPoint2D center = CGnuPlotEllipse::getCenter().point();

  return CQUtil::toQPoint(center);
}

void
CQGnuPlotEllipse::
setCenter(const QPointF &p)
{
  CGnuPlotEllipse::setCenter(CQUtil::fromQPoint(p));
}

void
CQGnuPlotEllipse::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotEllipse::draw(renderer);

  if (isSelected()) {
    CPoint2D center = CGnuPlotEllipse::getCenter().getPoint(renderer);

    renderer->drawEllipse(center, getRX(), getRY(), 0, CRGBA(1,0,0), 2);
  }
}
