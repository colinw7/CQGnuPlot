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

  rect_ = CBBox2D();

  for (const auto &p : points_)
    rect_ += p;

  CMathGeom2D::PolygonCentroid(points_, c_);
}

bool
CGnuPlotPolygonObject::
inside(const CPoint2D &p) const
{
  if (! rect_.inside(p))
    return false;

  return CMathGeom2D::PointInsideEvenOdd(p, points_);
}

void
CGnuPlotPolygonObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (hasFillPattern()) {
    CRGBA bg = fillBackground_.getValue(CRGBA(1,1,1)); // background ?
    CRGBA fg = fillColor_     .getValue(CRGBA(0,0,0));

    if (isClipped())
      renderer->patternClippedPolygon(points_, fillPattern(), fg, bg);
    else
      renderer->patternPolygon(points_, fillPattern(), fg, bg);
  }
  else if (fillColor_.isValid()) {
    if (isClipped())
      renderer->fillClippedPolygon(points_, fillColor());
    else
      renderer->fillPolygon(points_, fillColor());
  }

  if (lineColor_.isValid()) {
    if (isClipped())
      renderer->drawClippedPolygon(points_, lineWidth_, lineColor());
    else
      renderer->drawPolygon(points_, lineWidth_, lineColor());
  }

  if (text_ != "") {
    double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
    double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    renderer->drawText(c_ - CPoint2D(tw/2, th/2), text_, CRGBA(0,0,0));
  }
}
