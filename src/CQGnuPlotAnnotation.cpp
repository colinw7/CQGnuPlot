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

CQGnuPlotAnnotation::
~CQGnuPlotAnnotation()
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
isClip() const
{
  return obj_->isClip();
}

void
CQGnuPlotAnnotation::
setClip(bool b)
{
  return obj_->setClip(b);
}

bool
CQGnuPlotAnnotation::
mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip)
{
  if (! obj_->inside(mouseEvent))
    return false;

  qgroup()->qwindow()->highlightObject(this);

  tip = obj_->tip();

  return true;
}

void
CQGnuPlotAnnotation::
move(int key)
{
  CBBox2D bbox = obj_->bbox();

  if      (key == Qt::Key_Left)
    bbox.moveBy(-CPoint2D(bbox.getWidth()/10, 0));
  else if (key == Qt::Key_Right)
    bbox.moveBy( CPoint2D(bbox.getWidth()/10, 0));
  else if (key == Qt::Key_Down)
    bbox.moveBy(-CPoint2D(0, bbox.getHeight()/10));
  else if (key == Qt::Key_Up)
    bbox.moveBy( CPoint2D(0, bbox.getHeight()/10));

  obj_->setBBox(bbox);
}
