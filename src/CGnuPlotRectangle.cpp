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

  CGnuPlotColorSpec cs;

  cs.setRGB(bg);

  fs_.setBorderColor(cs); // which do we use ?

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
      CPoint2D from = from_.getValue().getPoint2D(renderer);
      CPoint2D to   = to_  .getValue().getPoint2D(renderer);

      bbox_ = CBBox2D(from, to);
    }
    else if (rto_.isValid()) {
      const CPoint3D &p1 = from_.getValue().point();
      CPoint3D        p2 = p1 + rto_.getValue().point();

      bbox_ = CBBox2D(CPoint2D(p1.x, p1.y), CPoint2D(p2.x, p2.y));
    }
    else if (size_.isValid()) {
      CSize2D size = size_.getValue().getSize(renderer);

      CPoint3D s(size.width, size.height, 0);

      const CPoint3D &p1 = from_.getValue().point();
      CPoint3D        p2 = p1 + s;

      bbox_ = CBBox2D(CPoint2D(p1.x, p1.y), CPoint2D(p2.x, p2.y));
    }
  }
  else if (center_.isValid()) {
    if (size_.isValid()) {
      CSize2D size = size_.getValue().getSize(renderer);

      CPoint3D s(size.width, size.height, 0);

      CPoint3D p1 = center_.getValue().point() - s/2;
      CPoint3D p2 = center_.getValue().point() + s/2;

      bbox_ = CBBox2D(CPoint2D(p1.x, p1.y), CPoint2D(p2.x, p2.y));
    }
  }

  return bbox_;
}

CGnuPlotTipData
CGnuPlotRectangle::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g, %g -> %g, %g",
                bbox_.getXMin(), bbox_.getYMin(), bbox_.getXMax(), bbox_.getYMax()));

  tip.setBorderColor(lc_);
  tip.setXColor(lc_);

  tip.setBBox(bbox_);

  return tip;
}

void
CGnuPlotRectangle::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  // clip if enabled and does not use screen coordinates
  CBBox2D bbox = calcBBox();

  renderer->setClip(group_->getClip());

  lc_ = getStrokeColor().getValue(CRGBA(0,0,0));

  CRGBA  lc = lc_;
  double lw = getLineWidth().getValue(0);;

  if (highlighted) {
    lc = CRGBA(1,0,0);
    lw = 2;
  }

  if      (getFillStyle().style() == CGnuPlotTypes::FillType::SOLID) {
    CRGBA fc = getFillColor().color();

    fc.setAlpha(getFillStyle().density());

    if (highlighted) {
      fc = CRGBA(1,0,0);
    }

    if (getFillColor().isRGB())
      renderer->fillClippedRect(bbox, fc);
  }
  else if (getFillStyle().style() == CGnuPlotTypes::FillType::PATTERN) {
    CRGBA fc = getFillColor().color();

    if (highlighted) {
      fc = CRGBA(1,0,0);
    }

    if (getFillColor().isRGB())
      renderer->patternRect(bbox, getFillStyle().pattern(), fc, lc);
  }

  renderer->drawClippedRect(bbox, lc, lw);
}

bool
CGnuPlotRectangle::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
}

void
CGnuPlotRectangle::
print(std::ostream &os) const
{
  os << " rect";

  if      (from_.isValid()) {
    if (to_.isValid()) {
      os << " from " << from_.getValue() << " to " << to_.getValue();
    }
    else if (rto_.isValid()) {
      os << " from " << from_.getValue() << " rto " << rto_.getValue();
    }
    else if (size_.isValid()) {
      os << " from " << from_.getValue() << " size " << size_.getValue();
    }
  }
  else if (center_.isValid()) {
    if (size_.isValid()) {
      os << " center " << center_.getValue() << " size " << size_.getValue();
    }
  }

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc bgnd "

  os << " fillstyle " << fs_;

  os << " lt " << lt_.getValue(-1);
}
