#include <CQGnuPlotAnnotation.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotAnnotation::
CQGnuPlotAnnotation(CGnuPlotGroupAnnotation *obj) :
 obj_(obj)
{
}

QColor
CQGnuPlotAnnotation::
getStrokeColor() const
{
  return CQUtil::rgbaToColor(obj_->getStrokeColor().getValue(CRGBA(0,0,0)));
}

void
CQGnuPlotAnnotation::
setStrokeColor(const QColor &c)
{
  obj_->setStrokeColor(CQUtil::colorToRGBA(c));
}

QColor
CQGnuPlotAnnotation::
getFillColor() const
{
  return CQUtil::rgbaToColor(obj_->getFillColor().color());
}

void
CQGnuPlotAnnotation::
setFillColor(const QColor &c)
{
  CGnuPlotColorSpec spec;

  spec.setRGB(CQUtil::colorToRGBA(c));

  obj_->setFillColor(spec);
}

CQGnuPlot::DrawLayerType
CQGnuPlotAnnotation::
getDrawLayer() const
{
  return CQGnuPlotUtil::drawLayerTypeConv(obj_->getLayer());
}

void
CQGnuPlotAnnotation::
setDrawLayer(const DrawLayerType &layer)
{
  obj_->setLayer(CQGnuPlotUtil::drawLayerTypeConv(layer));
}
