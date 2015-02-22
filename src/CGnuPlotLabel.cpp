#include <CGnuPlotLabel.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CFontMgr.h>

CGnuPlotLabel::
CGnuPlotLabel(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotLabel::
draw() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  CHAlignType halign = getAlign();
//CVAlignType valign = (getFront() ? CVALIGN_TYPE_TOP : CVALIGN_TYPE_CENTER);
  CVAlignType valign = CVALIGN_TYPE_CENTER;

  if (font_.isValid())
    renderer->setFont(font_);

  if (enhanced_)
    bbox_ = text_.calcBBox(renderer).moveBy(getPos());
  else
    bbox_ = renderer->getHAlignedTextBBox(getText().text()).moveBy(getPos());

  CPoint2D pos = getPos();

  CPoint2D d(0, 0);

  double w = bbox_.getWidth ();
  double h = bbox_.getHeight();

  double dh = std::max(bbox_.getTop() - pos.y, 0.0);

  h -= dh;

  if      (halign == CHALIGN_TYPE_LEFT) {
  }
  else if (halign == CHALIGN_TYPE_CENTER) {
    d.x -= w/2;
  }
  else if (halign == CHALIGN_TYPE_RIGHT) {
    d.x -= w;
  }

  if      (valign == CVALIGN_TYPE_TOP) {
  }
  else if (valign == CVALIGN_TYPE_CENTER) {
    d.y += h/2;
  }
  else if (valign == CVALIGN_TYPE_BOTTOM) {
    d.y += h;
  }

  bbox_.moveBy(d);

  pos += d;

  CRGBA c = getStrokeColor();

  if (enhanced_) {
    CBBox2D bbox = bbox_;

    bbox.setYMax(pos.y);

    text_.draw(renderer, bbox, halign, c);
  }
  else
    renderer->drawHTextInBox(bbox_, getText().text(), halign, c);
}

bool
CGnuPlotLabel::
inside(const CPoint2D &p) const
{
  return bbox_.inside(p);
}
