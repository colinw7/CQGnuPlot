#include <CGnuPlotEllipse.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

CGnuPlotEllipse::
CGnuPlotEllipse(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
}

CGnuPlotEllipse *
CGnuPlotEllipse::
setData(const CGnuPlotEllipse *ellipse)
{
  (void) CGnuPlotGroupAnnotation::setData(ellipse);

  p_     = ellipse->p_;
  angle_ = ellipse->angle_;
  size_  = ellipse->size_;
  units_ = ellipse->units_;
  fs_    = ellipse->fs_;
  lw_    = ellipse->lw_;

  return this;
}

void
CGnuPlotEllipse::
initClip()
{
  clip_ = ! p_.isScreen();
}

void
CGnuPlotEllipse::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  if (isClip())
    renderer->setClip(group_->getClip());
  else
    renderer->resetClip();

  center_ = this->getCenter().getPoint2D(renderer);

  CSize2D s = this->getSize().getSize(renderer);

  xr_ = s.width /2;
  yr_ = s.height/2;

  if (this->getFillColor().isRGB()) {
    CRGBA fc = this->getFillColor().color();

    if (highlighted) {
      fc = fc.getLightRGBA();
    }

    renderer->fillClippedEllipse(center_, xr_, yr_, angle_, fc);
  }

  CRGBA  c;
  double lw = 1;

  if      (this->getStrokeColor().isValid()) {
    c_ = this->getStrokeColor().getValue();

    c = c_;

    if (highlighted) {
      c  = CRGBA(1,0,0);
      lw = 2;
    }

    renderer->drawClippedEllipse(center_, xr_, yr_, angle_, c, lw, dash_);
  }
  else if (getFillStyle().calcColor(group_, c_)) {
    c = c_;

    if (highlighted) {
      c  = CRGBA(1,0,0);
      lw = 2;
    }

    renderer->drawClippedEllipse(center_, xr_, yr_, angle_, c, lw, dash_);
  }
  else if (highlighted) {
    c  = CRGBA(1,0,0);
    lw = 2;

    renderer->drawClippedEllipse(center_, xr_, yr_, angle_, c, lw, dash_);
  }

  bbox_ = CBBox2D(center_.x - xr_, center_.y - yr_, center_.x + xr_, center_.y + yr_);
}

bool
CGnuPlotEllipse::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return bbox_.inside(mouseEvent.window());
}

CGnuPlotTipData
CGnuPlotEllipse::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g, %g", center_.x, center_.y));
  tip.setYStr(CStrUtil::strprintf("%g, %g", xr_, yr_));

  tip.setBorderColor(c_);
  tip.setXColor(c_);

  tip.setBBox(bbox_);

  return tip;
}


void
CGnuPlotEllipse::
setBBox(const CBBox2D &bbox)
{
  p_    = bbox.getCenter();
  size_ = bbox.getSize();
}

void
CGnuPlotEllipse::
print(std::ostream &os) const
{
  os << " ellipse";

  os << " center " << p_;
  os << " size " << size_;
  os << " angle " << angle_;

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  //os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc bgnd "

  os << " fillstyle " << fs_;

  //os << " lt " << lt_.getValue(-1);
}
