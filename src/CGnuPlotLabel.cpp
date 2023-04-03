#include <CGnuPlotLabel.h>

CGnuPlotLabel::
CGnuPlotLabel(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
  data_ = new CGnuPlotLabelData(group);

  data_->setAlign(CHALIGN_TYPE_LEFT);
}

CGnuPlotLabel *
CGnuPlotLabel::
setData(const CGnuPlotLabel *label)
{
  (void) CGnuPlotGroupAnnotation::setData(label);

  data_ = label->data_->dup();

  return this;
}

void
CGnuPlotLabel::
setData(CGnuPlotLabelData *data)
{
  delete data_;

  data_ = data;
}

void
CGnuPlotLabel::
initClip()
{
  clip_ = ! getPos().isScreen();
}

void
CGnuPlotLabel::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  data_->draw(renderer, group_, highlighted);
}

bool
CGnuPlotLabel::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return data_->inside(mouseEvent);
}

const CBBox2D &
CGnuPlotLabel::
bbox() const
{
  return data_->bbox();
}

CGnuPlotTipData
CGnuPlotLabel::
tip() const
{
  return data_->tip();
}

void
CGnuPlotLabel::
print(std::ostream &os) const
{
  os << " \"";

  CGnuPlotText ptext(getText());

  ptext.print(os);

  os << "\"" << " at " << getPos();

  if (hasHypertext())
    os << " hypertext";

  if      (getAlign() == CHALIGN_TYPE_LEFT  ) os << " left";
  else if (getAlign() == CHALIGN_TYPE_CENTER) os << " center";
  else if (getAlign() == CHALIGN_TYPE_RIGHT ) os << " right";

  if (getAngle() < 0)
    os << " not rotated";
  else
    os << " rotated by " << getAngle() << " degrees";

  if      (getLayer() == CGnuPlotTypes::DrawLayer::FRONT)
    os << " front";
  else if (getLayer() == CGnuPlotTypes::DrawLayer::BACK)
    os << " back";
  else if (getLayer() == CGnuPlotTypes::DrawLayer::BEHIND)
    os << " behind";

  if (getFont())
    os << " font \"" << getFont() << "\"";

  if (textColor().isValid())
    os << " textcolor " << textColor();

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

  if (boxStroke() && boxStroke()->isEnabled())
   os << " boxed";
}
