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

CGnuPlotRectangle *
CGnuPlotRectangle::
setData(const CGnuPlotRectangle *rect)
{
  (void) CGnuPlotGroupAnnotation::setData(rect);

  from_   = rect->from_;
  to_     = rect->to_;
  rto_    = rect->rto_;
  center_ = rect->center_;
  size_   = rect->size_;
  fs_     = rect->fs_;
  lt_     = rect->lt_;
  lw_     = rect->lw_;
  dash_   = rect->dash_;
  bbox_   = rect->bbox_;

  return this;
}

CBBox2D
CGnuPlotRectangle::
calcBBox() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  bbox_ = CBBox2D(0,0,1,1);

  if      (from_) {
    if      (to_) {
      CPoint2D from = from_.value().getPoint2D(renderer);
      CPoint2D to   = to_  .value().getPoint2D(renderer);

      bbox_ = CBBox2D(from, to);
    }
    else if (rto_) {
      const CPoint3D &p1 = from_.value().point();
      CPoint3D        p2 = p1 + rto_.value().point();

      bbox_ = CBBox2D(CPoint2D(p1.x, p1.y), CPoint2D(p2.x, p2.y));
    }
    else if (size_) {
      CSize2D size = size_.value().getSize(renderer);

      CPoint3D s(size.getWidth(), size.getHeight(), 0);

      const CPoint3D &p1 = from_.value().point();
      CPoint3D        p2 = p1 + s;

      bbox_ = CBBox2D(CPoint2D(p1.x, p1.y), CPoint2D(p2.x, p2.y));
    }
  }
  else if (center_) {
    if (size_) {
      CSize2D size = size_.value().getSize(renderer);

      CPoint3D s(size.getWidth(), size.getHeight(), 0);

      CPoint3D p1 = center_.value().point() - s/2;
      CPoint3D p2 = center_.value().point() + s/2;

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
initClip()
{
  if (getFrom())
    clip_ = ! getFrom().value().isScreen();
}

void
CGnuPlotRectangle::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  // clip if enabled and does not use screen coordinates
  CBBox2D bbox = calcBBox();

  if (isClip())
    renderer->setClip(group_->getClip());
  else
    renderer->resetClip();

  lc_ = getStrokeColor().value_or(CRGBA(0,0,0));

  CRGBA  lc = lc_;
  double lw = getLineWidth().value_or(0);

  if (highlighted) {
    lc = CRGBA(1,0,0);
    lw = 2;
  }

  if      (getFillStyle().style() == CGnuPlotTypes::FillType::SOLID) {
    CRGBA fc = getFillColor().color();

    fc.setAlpha(getFillStyle().density());

    if (highlighted) {
      fc = fc.getLightRGBA();
    }

    if (getFillColor().isRGB())
      renderer->fillClippedRect(bbox, fc);
  }
  else if (getFillStyle().style() == CGnuPlotTypes::FillType::PATTERN) {
    CRGBA fc = getFillColor().color();

    if (highlighted) {
      fc = fc.getLightRGBA();
    }

    if (getFillColor().isRGB())
      renderer->patternRect(bbox, getFillStyle().pattern(), fc, lc);
  }

  renderer->drawClippedRect(bbox, lc, lw, dash_);
}

bool
CGnuPlotRectangle::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return bbox_.inside(mouseEvent.window());
}

void
CGnuPlotRectangle::
setBBox(const CBBox2D &bbox)
{
  CPoint2D p1 = bbox.getMin();
  CPoint2D p2 = bbox.getMax();

  if      (from_) {
    if      (to_) {
      from_ = CGnuPlotPosition(CPoint3D(p1.x, p1.y, 0));
      to_   = CGnuPlotPosition(CPoint3D(p2.x, p2.y, 0));
    }
    else if (rto_) {
      from_ = CGnuPlotPosition(CPoint3D(p1.x, p1.y, 0));
      rto_  = CGnuPlotPosition(CPoint3D(p2.x - p1.x, p2.y - p1.y, 0));
    }
    else if (size_) {
      from_ = CGnuPlotPosition(CPoint3D(p1.x, p1.y, 0));
      size_ = CSize2D(p2.x - p1.x, p2.y - p1.y);
    }
  }
  else if (center_) {
    CPoint2D c = bbox.getCenter();

    if (size_) {
      size_   = CSize2D(p2.x - p1.x, p2.y - p1.y);
      center_ = CGnuPlotPosition(CPoint3D(c.x, c.y, 0));
    }
  }
}

void
CGnuPlotRectangle::
print(std::ostream &os) const
{
  os << " rect";

  if      (from_) {
    if (to_) {
      os << " from " << from_.value() << " to " << to_.value();
    }
    else if (rto_) {
      os << " from " << from_.value() << " rto " << rto_.value();
    }
    else if (size_) {
      os << " from " << from_.value() << " size " << size_.value();
    }
  }
  else if (center_) {
    if (size_) {
      os << " center " << center_.value() << " size " << size_.value();
    }
  }

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  os << " lw " << lw_.value_or(1.0);

  // dashtype solid fc bgnd "

  os << " fillstyle " << fs_;

  os << " lt " << lt_.value_or(-1);
}
