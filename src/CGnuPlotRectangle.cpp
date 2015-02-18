#include <CGnuPlotRectangle.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotRectangle::
CGnuPlotRectangle(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
  // default solid fill with background color and black border
  // TODO: how get background color
  fs_.setStyle(CGnuPlotTypes::FillType::SOLID);
  fs_.setBorder(true);

  fs_.setBorderColor(CRGBA(1,1,1)); // which do we use ?
  fillColor_.setRGB(CRGBA(1,1,1));
}

CBBox2D
CGnuPlotRectangle::
calcBBox() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  bbox_ = CBBox2D(0,0,1,1);

  if      (from_.isValid()) {
    if      (to_.isValid()) {
      CPoint2D from = from_.getValue().getPoint(renderer);
      CPoint2D to   = to_  .getValue().getPoint(renderer);

      bbox_ = CBBox2D(from, to);
    }
    else if (rto_.isValid())
      bbox_ = CBBox2D(from_.getValue().point(), from_.getValue().point() + rto_.getValue().point());
    else if (size_.isValid()) {
      CSize2D size = size_.getValue().getSize(renderer);

      CPoint2D s(size.width, size.height);

      bbox_ = CBBox2D(from_.getValue().point(), from_.getValue().point() + s);
    }
  }
  else if (center_.isValid()) {
    if (size_.isValid()) {
      CSize2D size = size_.getValue().getSize(renderer);

      CPoint2D s(size.width, size.height);

      bbox_ = CBBox2D(center_.getValue().point() - s/2, center_.getValue().point() + s/2);
    }
  }

  return bbox_;
}

void
CGnuPlotRectangle::
draw() const
{
  // clip if enabled and does not use screen coordinates
  CBBox2D bbox = calcBBox();

  CGnuPlotRenderer *renderer = group_->app()->renderer();

  renderer->setClip(group_->getClip());

  if      (getFillStyle().style() == CGnuPlotTypes::FillType::SOLID) {
    CRGBA fc = getFillColor().color();

    fc.setAlpha(getFillStyle().density());

    if (getFillColor().isRGB())
      renderer->fillClippedRect(bbox, fc);
  }
  else if (getFillStyle().style() == CGnuPlotTypes::FillType::PATTERN) {
    if (getFillColor().isRGB())
      renderer->patternRect(bbox, getFillStyle().pattern(),
                            getFillColor().color(), getStrokeColor());
  }

  renderer->drawClippedRect(bbox, getStrokeColor(), getLineWidth().getValue(0));
}

bool
CGnuPlotRectangle::
inside(const CPoint2D &p) const
{
  return bbox_.inside(p);
}

void
CGnuPlotRectangle::
print(std::ostream &) const
{
  // TODO
}
