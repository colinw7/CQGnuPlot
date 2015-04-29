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

void
CQGnuPlotCircle::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotCircle::draw(renderer);

  if (isSelected()) {
    CPoint2D center = CGnuPlotCircle::getCenter().getPoint(renderer);

    double xr = CGnuPlotCircle::getRadius().getXValue(renderer);
    double yr = CGnuPlotCircle::getRadius().getYValue(renderer);

    renderer->drawEllipse(center, xr, yr, 0, CRGBA(1,0,0), 2);
  }
}
