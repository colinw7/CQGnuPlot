#include <CGnuPlotLabel.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CFontMgr.h>

CFontPtr CGnuPlotLabel::defFont_;

CGnuPlotLabel::
CGnuPlotLabel(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
  if (! defFont_.isValid())
    defFont_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotLabel::
draw(CGnuPlotRenderer *renderer) const
{
  CHAlignType halign = getAlign();
//CVAlignType valign = (getFront() ? CVALIGN_TYPE_TOP : CVALIGN_TYPE_CENTER);
  CVAlignType valign = CVALIGN_TYPE_CENTER;

  if (font_.isValid())
    renderer->setFont(font_);
  else
    renderer->setFont(defFont_);

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

  if (showPoint_) {
    // TODO: show point
  }

  CRGBA c = textColor_.color();

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

void
CGnuPlotLabel::
print(std::ostream &os) const
{
  os << " \"";

  text_.print(os);

  os << "\"" << " at " << pos_;

  if (hypertext_)
    os << " hypertext";

  if      (align_ == CHALIGN_TYPE_LEFT  ) os << " left";
  else if (align_ == CHALIGN_TYPE_CENTER) os << " center";
  else if (align_ == CHALIGN_TYPE_RIGHT ) os << " right";

  if (angle_ < 0)
    os << " not rotated";
  else
    os << " rotated by " << angle_ << " degrees";

  if      (layer_ == DrawLayer::FRONT)
    os << " front";
  else if (layer_ == DrawLayer::BACK)
    os << " back";
  else if (layer_ == DrawLayer::BEHIND)
    os << " behind";

  if (font_.isValid())
    os << " font \"" << font_ << "\"";

  if (textColor_.isValid())
    os << " textcolor " << textColor_;

  if (showPoint_) {
    os << " point with";

    if (lineType_ >= 0)
      os << " linetype " << lineType_;

    if (pointType_ >= 0)
      os << " pointtype " << pointType_;

    if (pointSize_ >= 0)
      os << " pointsize " << pointSize_;
    else
      os << " pointsize default";
  }
  else
    os << " nopoint ";

  if (offset_.isValid())
    os << " offset " << offset_.getValue();

  if (box_)
   os << " boxed";
}

