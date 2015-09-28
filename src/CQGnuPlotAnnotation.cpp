#include <CQGnuPlotAnnotation.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotAnnotation::
CQGnuPlotAnnotation(CQGnuPlotGroup *qgroup, CGnuPlotGroupAnnotation *obj) :
 qgroup_(qgroup), obj_(obj)
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

CQGnuPlotEnum::DrawLayerType
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

bool
CQGnuPlotAnnotation::
mouseTip(const CPoint2D &pixel, const CPoint2D &window, CGnuPlotTipData &tip)
{
  CGnuPlotTypes::InsideData insideData(window, pixel);

  if (! obj_->inside(insideData))
    return false;

  qgroup()->qwindow()->highlightObject(this);

  tip = obj_->tip();

  return true;
}
