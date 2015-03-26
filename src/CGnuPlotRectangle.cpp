#include <CGnuPlotRectangle.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>

CGnuPlotRectangle::
CGnuPlotRectangle(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
  // default solid fill with background color and black border
  fs_.setStyle(CGnuPlotTypes::FillType::SOLID);
  fs_.setBorder(true);

  CRGBA bg = (group_ ? group_->window()->backgroundColor() : CRGBA(1,1,1));

  fs_.setBorderColor(bg); // which do we use ?
  fillColor_.setRGB(bg);
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
draw(CGnuPlotRenderer *renderer) const
{
  // clip if enabled and does not use screen coordinates
  CBBox2D bbox = calcBBox();

  renderer->setClip(group_->getClip());

  CRGBA lc = getStrokeColor().getValue(CRGBA(0,0,0));

  if      (getFillStyle().style() == CGnuPlotTypes::FillType::SOLID) {
    CRGBA fc = getFillColor().color();

    fc.setAlpha(getFillStyle().density());

    if (getFillColor().isRGB())
      renderer->fillClippedRect(bbox, fc);
  }
  else if (getFillStyle().style() == CGnuPlotTypes::FillType::PATTERN) {
    if (getFillColor().isRGB())
      renderer->patternRect(bbox, getFillStyle().pattern(), getFillColor().color(), lc);
  }

  renderer->drawClippedRect(bbox, lc, getLineWidth().getValue(0));
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
