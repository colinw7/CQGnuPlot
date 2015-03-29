#include <CGnuPlotPolygon.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>

CGnuPlotPolygon::
CGnuPlotPolygon(CGnuPlotGroup *group) :
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
CGnuPlotPolygon::
calcBBox() const
{
  bbox_ = CBBox2D();

  for (const auto &p : points_)
    bbox_.add(p);

  return bbox_;
}

void
CGnuPlotPolygon::
draw(CGnuPlotRenderer *renderer) const
{
  // clip if enabled and does not use screen coordinates
  //CBBox2D bbox = calcBBox();

  //renderer->setClip(group_->getClip());

  const CGnuPlotPolygon *poly = this;

  if (poly->getFillColor().isRGB()) {
    CRGBA fc = getFillColor().color();

    fc.setAlpha(getFillStyle().density());

    if (getFillColor().isRGB())
      renderer->fillPolygon(poly->getPoints(), fc);
  }

  CRGBA lc = poly->getStrokeColor().getValue(CRGBA(0,0,0));

  renderer->drawPolygon(poly->getPoints(), getLineWidth().getValue(0), lc);
}

bool
CGnuPlotPolygon::
inside(const CPoint2D &p) const
{
  return bbox_.inside(p);
}

void
CGnuPlotPolygon::
print(std::ostream &os) const
{
  os << " polygon";

  for (uint i = 0; i < points_.size(); ++i) {
    if (i == 0)
      os << " from " << points_[i];
    else
      os << " to " << points_[i];
  }

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc border"

  os << " lt " << lt_.getValue(-1);

  os << " fillstyle " << fs_;
}
