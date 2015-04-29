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
  if (fillColor_.isValid())
    renderer->fillPolygon(points_, fillColor_.getValue());

  if (lineColor_.isValid())
    renderer->drawPolygon(points_, lineWidth_, lineColor_.getValue());

  double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
  double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

  renderer->drawText(c_ - CPoint2D(tw/2, th/2), text_, CRGBA(0,0,0));
}
