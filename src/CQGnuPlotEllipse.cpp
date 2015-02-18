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
  return CQUtil::toQPoint(CGnuPlotEllipse::getCenter());
}

void
CQGnuPlotEllipse::
setCenter(const QPointF &p)
{
  CGnuPlotEllipse::setCenter(CQUtil::fromQPoint(p));
}

void
CQGnuPlotEllipse::
draw() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  CGnuPlotEllipse::draw();

  if (isSelected())
    renderer->drawEllipse(CGnuPlotEllipse::getCenter(), getRX(), getRY(), 0, CRGBA(1,0,0), 2);
}
