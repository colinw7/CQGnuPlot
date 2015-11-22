#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>

CGnuPlotPolygonObject::
CGnuPlotPolygonObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
}

void
CGnuPlotPolygonObject::
setPoints(const Points &points)
{
  points_ = points;

  bbox_ = CBBox2D();

  for (const auto &p : points_)
    bbox_ += p;

  CMathGeom2D::PolygonCentroid(points_, center_);
}

bool
CGnuPlotPolygonObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  const CPoint2D &p = mouseEvent.window();

  if (! bbox_.inside(p))
    return false;

  return CMathGeom2D::PointInsideEvenOdd(p, points_);
}

void
CGnuPlotPolygonObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    fill   = fill_  ->dup();
    stroke = stroke_->dup();

    if (fill->type() == CGnuPlotTypes::FillType::PATTERN)
      fill->setBackground(fill->background().getLightRGBA());
    else
      fill->setColor(fill->color().getLightRGBA());

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  if (! isAliased())
    renderer->setAntiAlias(false);

  if (isClipped()) {
    renderer->fillClippedPolygon  (points_, *fill  );
    renderer->strokeClippedPolygon(points_, *stroke);
  }
  else {
    renderer->fillPolygon  (points_, *fill  );
    renderer->strokePolygon(points_, *stroke);
  }

  if (! isAliased())
    renderer->setAntiAlias(true);

  if (text_ != "") {
    CRGBA tc(0,0,0);

    if (highlighted) {
      tc = CRGBA(1,0,0);
    }

    double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
    double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    renderer->drawText(center_ - CPoint2D(tw/2, th/2), text_, tc);
  }
}

CGnuPlotTipData
CGnuPlotPolygonObject::
tip() const
{
  std::string tipText = tipText_;

  if (tipText == "" && text_ != "")
    tipText = text_;

  if (tipText == "")
    tipText = CStrUtil::strprintf("%d Points", points_.size());

  CGnuPlotTipData tip;

  tip.setXStr(tipText);
  tip.setYStr("");

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  tip.setBBox(bbox_);

  return tip;
}
