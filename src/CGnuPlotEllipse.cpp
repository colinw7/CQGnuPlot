#include <CGnuPlotEllipse.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>

void
CGnuPlotEllipse::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  center_ = this->getCenter().getPoint2D(renderer);

  if (this->getFillColor().isRGB()) {
    CRGBA fc = this->getFillColor().color();

    if (highlighted) {
      fc = fc.getLightRGBA();
    }

    renderer->fillEllipse(center_, this->getRX(), this->getRY(), 0, fc);
  }

  CRGBA  c;
  double lw = 1;

  xr_ = this->getRX();
  yr_ = this->getRY();

  bbox_ = CBBox2D(center_.x - xr_, center_.y - yr_, center_.x + xr_, center_.y + yr_);

  if      (this->getStrokeColor().isValid()) {
    c_ = this->getStrokeColor().getValue();

    c = c_;

    if (highlighted) {
      c  = CRGBA(1,0,0);
      lw = 2;
    }

    renderer->drawEllipse(center_, xr_, yr_, angle_, c, lw);
  }
  else if (getFillStyle().calcColor(group_, c_)) {
    c = c_;

    if (highlighted) {
      c  = CRGBA(1,0,0);
      lw = 2;
    }

    renderer->drawEllipse(center_, xr_, yr_, angle_, c, lw);
  }
  else if (highlighted) {
    c  = CRGBA(1,0,0);
    lw = 2;

    renderer->drawEllipse(center_, xr_, yr_, angle_, c, lw);
  }
}

bool
CGnuPlotEllipse::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
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
print(std::ostream &os) const
{
  os << " ellipse";

  os << " center " << p_;
  os << " size " << 2*rx_ << ", " << 2*ry_;
  os << " angle " << angle_;

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  //os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc bgnd "

  os << " fillstyle " << fs_;

  //os << " lt " << lt_.getValue(-1);
}
