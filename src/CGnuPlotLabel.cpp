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
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CHAlignType halign = getAlign();
  CVAlignType valign = CVALIGN_TYPE_BOTTOM;

  double a = angle_.getValue(0);

  if (getFont().isValid())
    renderer->setFont(getFont());
  else
    renderer->setFont(defFont_);

  double td = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharDescent());

  CPoint3D pos = getPos().getPoint3D(renderer);

  drawPos_ = renderer->transform(pos);

  CBBox2D tbbox;

  if (isEnhanced() || fabs(a) > 1E-6)
    tbbox = text_.calcBBox(renderer);
  else
    tbbox = renderer->getHAlignedTextBBox(getText().text());

  double w = tbbox.getWidth ();
  double h = tbbox.getHeight();

  bbox_ = CBBox2D(drawPos_.x, drawPos_.y, drawPos_.x + w, drawPos_.y + h);

  CPoint2D d(0, 0);

  if      (halign == CHALIGN_TYPE_LEFT) {
  }
  else if (halign == CHALIGN_TYPE_CENTER) {
    d.x -= w/2;
  }
  else if (halign == CHALIGN_TYPE_RIGHT) {
    d.x -= w;
  }

  if      (valign == CVALIGN_TYPE_BOTTOM) {
  }
  else if (valign == CVALIGN_TYPE_CENTER) {
    d.y -= h/2;
  }
  else if (valign == CVALIGN_TYPE_TOP) {
    d.y -= h;
  }

  bbox_.moveBy(d);
  bbox_.moveBy(CPoint2D(0, -td));

  if (textColor_.isPaletteZ())
    drawColor_ = textColor_.calcColor(group_, pos.z);
  else
    drawColor_ = textColor_.color();

  CRGBA c = drawColor_;

  if (highlighted)
    c = CRGBA(1,0,0);

  if (showPoint()) {
    // TODO: show point
    renderer->drawSymbol(drawPos_, CGnuPlotTypes::SymbolType::CROSS, 1, c, 1, true);
  }

  COptPoint2D o(CPoint2D(drawPos_.x, drawPos_.y));
//renderer->drawSymbol(o.getValue(), CGnuPlotTypes::SymbolType::PLUS, 1, CRGBA(1,0,0), 1, true);

  if (isEnhanced() || fabs(a) > 1E-6)
    text_.draw(renderer, bbox_, halign, c, a, o);
  else
    renderer->drawHTextInBox(bbox_, getText().text(), halign, c);

  if (hasBox())
    renderer->drawRotatedRect(bbox_, a, c, 1, o);
}

bool
CGnuPlotLabel::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
}

CGnuPlotTipData
CGnuPlotLabel::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g, %g", drawPos_.x, drawPos_.y));
  tip.setYStr(CStrUtil::strprintf("%s", getText().text().c_str()));

  CRGBA c = this->drawColor();

  tip.setBorderColor(c);
  tip.setXColor(c);

  tip.setRect(bbox_);

  return tip;
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
