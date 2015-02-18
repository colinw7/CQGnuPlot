#include <CQGnuPlotRectangle.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotRectangle::
CQGnuPlotRectangle(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotRectangle(group)
{
  setObjectName("rectangle");
}

QPointF
CQGnuPlotRectangle::
getFrom() const
{
  CBBox2D bbox = CGnuPlotRectangle::calcBBox();

  return CQUtil::toQPoint(bbox.getLL());
}

void
CQGnuPlotRectangle::
setFrom(const QPointF &p)
{
  CGnuPlotRectangle::setFrom(CQUtil::fromQPoint(p));
}

QPointF
CQGnuPlotRectangle::
getTo() const
{
  CBBox2D bbox = CGnuPlotRectangle::calcBBox();

  return CQUtil::toQPoint(bbox.getUR());
}

void
CQGnuPlotRectangle::
setTo(const QPointF &p)
{
  CGnuPlotRectangle::setTo(CQUtil::fromQPoint(p));
}

double
CQGnuPlotRectangle::
getLineWidth() const
{
  return CGnuPlotRectangle::getLineWidth().getValue(0);
}

CQGnuPlot::CQFillType
CQGnuPlotRectangle::
fillType() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotRectangle::getFillStyle().style());
}

void
CQGnuPlotRectangle::
setFillType(const CQGnuPlot::CQFillType &type)
{
  CGnuPlotFillStyle fs = CGnuPlotRectangle::getFillStyle();

  fs.setStyle(CQGnuPlotUtil::fillTypeConv(type));

  CGnuPlotRectangle::setFillStyle(fs);
}

void
CQGnuPlotRectangle::
draw() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  CGnuPlotRectangle::draw();

  if (isSelected())
    renderer->drawRect(bbox_, CRGBA(1,0,0), 2);
}
