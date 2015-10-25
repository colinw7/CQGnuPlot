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

  CGnuPlotColorSpec cs;

  cs.setRGB(bg);

  fs_.setBorderColor(cs); // which do we use ?

  fillColor_.setRGB(bg);
}

CBBox2D
CGnuPlotPolygon::
calcBBox() const
{
  bbox_ = CBBox2D();

  for (const auto &p : points_)
    bbox_.add(CPoint2D(p.x, p.y));

  return bbox_;
}

void
CGnuPlotPolygon::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  // clip if enabled and does not use screen coordinates
  //CBBox2D bbox = calcBBox();

  //renderer->setClip(group_->getClip());

  fc_ = CRGBA();

  if (this->getFillColor().isRGB()) {
    fc_ = getFillColor().color();

    fc_.setAlpha(getFillStyle().density());

    if (getFillColor().isRGB()) {
      CRGBA fc = fc_;

      if (highlighted) {
        fc = fc.getLightRGBA();
      }

      renderer->fillPolygon(this->getPoints(), fc);
    }
  }

  //---

  lc_ = this->getStrokeColor().getValue(CRGBA(0,0,0));

  CRGBA  lc = lc_;
  double lw = getLineWidth().getValue(0);

  if (highlighted) {
    lc = CRGBA(1,0,0);
    lw = 2;
  }

  renderer->drawPolygon(this->getPoints(), lw, lc_, CLineDash());
}

bool
CGnuPlotPolygon::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
}

CGnuPlotTipData
CGnuPlotPolygon::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%n", points_.size()));
  tip.setYStr("");

  tip.setBorderColor(lc_);
  tip.setXColor(lc_);

  tip.setBBox(bbox_);

  return tip;
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
