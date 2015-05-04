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

    double xr = CGnuPlotCircle::getRadius().getXDistance(renderer);
    double yr = CGnuPlotCircle::getRadius().getYDistance(renderer);

    double a1 = CGnuPlotCircle::arcStart().getValue(0);
    double a2 = CGnuPlotCircle::arcEnd  ().getValue(360);

    if (CGnuPlotCircle::arcStart().isValid() || CGnuPlotCircle::arcEnd().isValid())
      renderer->drawPieSlice(center, 0, xr, a1, a2, 2, CRGBA(1,0,0));
    else
      renderer->drawEllipse(center, xr, yr, 0, CRGBA(1,0,0), 2);
  }
}
