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
  CPoint3D center = CGnuPlotEllipse::getCenter().point();

  return CQUtil::toQPoint(CPoint2D(center.x, center.y));
}

void
CQGnuPlotEllipse::
setCenter(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotEllipse::setCenter(CPoint3D(p1.x, p1.y, 0));
}

void
CQGnuPlotEllipse::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotEllipse::draw(renderer);

  if (isSelected()) {
    CPoint2D center = CGnuPlotEllipse::getCenter().getPoint2D(renderer);

    renderer->drawEllipse(center, getRX(), getRY(), 0, CRGBA(1,0,0), 2);
  }
}
