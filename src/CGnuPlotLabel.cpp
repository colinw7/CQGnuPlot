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

  if (getFont().isValid())
    renderer->setFont(getFont());
  else
    renderer->setFont(defFont_);

  CPoint3D pos  = getPos().getPoint3D(renderer);
  CPoint2D pos1 = renderer->transform(pos);

  if (isEnhanced())
    bbox_ = text_.calcBBox(renderer).moveBy(pos1);
  else
    bbox_ = renderer->getHAlignedTextBBox(getText().text()).moveBy(pos1);

  CPoint2D d(0, 0);

  double w = bbox_.getWidth ();
  double h = bbox_.getHeight();

  double dh = std::max(bbox_.getTop() - pos1.y, 0.0);

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

  pos1 += d;

  if (showPoint()) {
    // TODO: show point
  }

  CRGBA c = textColor_.color();

  if (isEnhanced()) {
    CBBox2D bbox = bbox_;

    bbox.setYMax(pos1.y);

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

  if (hasHypertext())
    os << " hypertext";

  if      (getAlign() == CHALIGN_TYPE_LEFT  ) os << " left";
  else if (getAlign() == CHALIGN_TYPE_CENTER) os << " center";
  else if (getAlign() == CHALIGN_TYPE_RIGHT ) os << " right";

  if (getAngle() < 0)
    os << " not rotated";
  else
    os << " rotated by " << getAngle() << " degrees";

  if      (layer_ == DrawLayer::FRONT)
    os << " front";
  else if (layer_ == DrawLayer::BACK)
    os << " back";
  else if (layer_ == DrawLayer::BEHIND)
    os << " behind";

  if (getFont().isValid())
    os << " font \"" << getFont() << "\"";

  if (textColor_.isValid())
    os << " textcolor " << textColor_;

  if (showPoint()) {
    os << " point with";

    if (lineType() >= 0)
      os << " linetype " << lineType();

    if (pointType() >= 0)
      os << " pointtype " << pointType();

    if (pointSize() >= 0)
      os << " pointsize " << pointSize();
    else
      os << " pointsize default";
  }
  else
    os << " nopoint ";

  if (getOffset().isValid())
    os << " offset " << getOffset().getValue();

  if (box_)
   os << " boxed";
}

