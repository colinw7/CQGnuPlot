#include <CQGnuPlotCircle.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>

CQGnuPlotCircle::
CQGnuPlotCircle(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(group, this), CGnuPlotCircle(group)
{
  setObjectName("circle");
}

QPointF
CQGnuPlotCircle::
getCenter() const
{
  CPoint3D center = CGnuPlotCircle::getCenter().point();

  return CQUtil::toQPoint(CPoint2D(center.x, center.y));
}

void
CQGnuPlotCircle::
setCenter(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotCircle::setCenter(CPoint3D(p1.x, p1.y, 0));
}

double
CQGnuPlotCircle::
getRadius() const
{
  return r_.value();
}

void
CQGnuPlotCircle::
setRadius(double r)
{
  r_.setValue(r);
}

double
CQGnuPlotCircle::
getLineWidth() const
{
  return lw_.getValue(1);
}

void
CQGnuPlotCircle::
setLineWidth(double r)
{
  lw_.setValue(r);
}

void
CQGnuPlotCircle::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotCircle::draw(renderer);
}
