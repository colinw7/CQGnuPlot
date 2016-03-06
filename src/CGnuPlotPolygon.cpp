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

CGnuPlotPolygon *
CGnuPlotPolygon::
setData(const CGnuPlotPolygon *poly)
{
  (void) CGnuPlotGroupAnnotation::setData(poly);

  typePositions_ = poly->typePositions_;
  fs_            = poly->fs_;
  lt_            = poly->lt_;
  lw_            = poly->lw_;
  dash_          = poly->dash_;
  bbox_          = poly->bbox_;
  fc_            = poly->fc_;
  lc_            = poly->lc_;
  ppoints2D_     = poly->ppoints2D_;
  ppoints3D_     = poly->ppoints3D_;

  return this;
}

void
CGnuPlotPolygon::
addPoint (const CGnuPlotPosition &p)
{
  typePositions_.push_back(TypePosition(PointType::ABSOLUTE, p));

  ppoints3D_.clear();
  ppoints2D_.clear();
}

void
CGnuPlotPolygon::
addRPoint(const CGnuPlotPosition &p)
{
  typePositions_.push_back(TypePosition(PointType::RELATIVE, p));

  ppoints3D_.clear();
  ppoints2D_.clear();
}

void
CGnuPlotPolygon::
initClip()
{
  if (! typePositions_.empty())
    clip_ = ! typePositions_.front().second.isScreen();
}

void
CGnuPlotPolygon::
draw(CGnuPlotRenderer *renderer) const
{
  //if (ppoints2D_.empty())
  calcPoints(renderer);

  //---

  bool highlighted = (isHighlighted() || isSelected());

  if (isClip())
    renderer->setClip(group_->getClip());
  else
    renderer->resetClip();

  fc_ = CRGBA();

  if (this->getFillColor().isRGB()) {
    fc_ = getFillColor().color();

    fc_.setAlpha(getFillStyle().density());

    if (getFillColor().isRGB()) {
      CRGBA fc = fc_;

      if (highlighted) {
        fc = fc.getLightRGBA();
      }

      if (group_->is3D())
        renderer->fillClippedPolygon(ppoints3D_, fc);
      else
        renderer->fillClippedPolygon(ppoints2D_, fc);
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

  if (group_->is3D())
    renderer->drawClippedPolygon(ppoints3D_, lc, lw, dash_);
  else
    renderer->drawClippedPolygon(ppoints2D_, lc, lw, dash_);
}

void
CGnuPlotPolygon::
calcPoints(CGnuPlotRenderer *renderer) const
{
  ppoints3D_.clear();
  ppoints2D_.clear();

  bbox_.reset();

  int i = 0;

  if (group_->is3D()) {
    CPoint3D lastp;

    for (const auto &typePos : typePositions_) {
      CPoint3D p;

      if (i > 0 && typePos.first == PointType::RELATIVE)
        p = lastp + typePos.second.getDistance3D(renderer);
      else
        p = typePos.second.getPoint3D(renderer);

      CPoint2D p1 = renderer->transform2D(p);

      ppoints3D_.push_back(p);
      ppoints2D_.push_back(p1);

      bbox_.add(p1);

      lastp = p;

      ++i;
    }
  }
  else {
    CPoint2D lastp;

    for (const auto &typePos : typePositions_) {
      CPoint2D p;

      if (i > 0 && typePos.first == PointType::RELATIVE)
        p = lastp + typePos.second.getDistance2D(renderer);
      else
        p = typePos.second.getPoint2D(renderer);

      ppoints3D_.push_back(CPoint3D(p.x, p.y, 0));
      ppoints2D_.push_back(p);

      bbox_.add(p);

      lastp = p;

      ++i;
    }
  }
}

bool
CGnuPlotPolygon::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return bbox_.inside(mouseEvent.window());
}

CGnuPlotTipData
CGnuPlotPolygon::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%d", typePositions_.size()));
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

  int i = 0;

  for (const auto &p : typePositions_) {
    if      (i == 0)
      os << " from " << p.second;
    else if (p.first == PointType::ABSOLUTE)
      os << " to " << p.second;
    else
      os << " rto " << p.second;

    ++i;
  }

  os << " " << CStrUniqueMatch::valueToString<CGnuPlotTypes::DrawLayer>(layer_);

  // clip

  os << " lw " << lw_.getValue(1.0);

  // dashtype solid fc border"

  os << " lt " << lt_.getValue(-1);

  os << " fillstyle " << fs_;
}
