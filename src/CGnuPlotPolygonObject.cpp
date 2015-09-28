#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>

CGnuPlotPolygonObject::
CGnuPlotPolygonObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
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
inside(const CGnuPlotTypes::InsideData &data) const
{
  const CPoint2D &p = data.window;

  if (! bbox_.inside(p))
    return false;

  return CMathGeom2D::PointInsideEvenOdd(p, points_);
}

void
CGnuPlotPolygonObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  if (hasFillPattern()) {
    CRGBA bg = fillBackground_.getValue(CRGBA(1,1,1)); // background ?
    CRGBA fg = fillColor_     .getValue(CRGBA(0,0,0));

    if (highlighted) {
      fg = fg.getLightRGBA();
    }

    if (isClipped())
      renderer->patternClippedPolygon(points_, fillPattern(), fg, bg);
    else
      renderer->patternPolygon(points_, fillPattern(), fg, bg);
  }
  else if (fillColor_.isValid()) {
    CRGBA fc = fillColor();

    lc_ = fc;

    if (highlighted) {
      fc = fc.getLightRGBA();
    }

    if (isClipped())
      renderer->fillClippedPolygon(points_, fc);
    else
      renderer->fillPolygon(points_, fc);
  }

  if (highlighted || lineColor_.isValid()) {
    if (lineColor_.isValid())
      lc_ = lineColor();
    else
      lc_ = CRGBA(0,0,0);

    CRGBA  lc = lc_;
    double lw = lineWidth_;

    if (highlighted) {
      lc = CRGBA(1,0,0);
      lw = 2;
    }

    if (isClipped())
      renderer->drawClippedPolygon(points_, lw, lc, CLineDash());
    else
      renderer->drawPolygon(points_, lw, lc, CLineDash());
  }

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

  tip.setBorderColor(lc_);
  tip.setXColor(lc_);

  tip.setRect(bbox_);

  return tip;
}
