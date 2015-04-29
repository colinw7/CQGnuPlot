#include <CGnuPlotPosition.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CPoint2D
CGnuPlotPosition::
getPoint(CGnuPlotRenderer *renderer) const
{
  return getPoint(renderer, p_.x, p_.y);
}

CPoint2D
CGnuPlotPosition::
getPoint(CGnuPlotRenderer *renderer, double x, double y) const
{
  if      (systemX_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    x = CGnuPlotUtil::map(x, 0, 1, range.getXMin(), range.getXMax());
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    x = CGnuPlotUtil::map(x, 0, 1, px1, px2);
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    // TODO
  }

  if      (systemY_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    y = CGnuPlotUtil::map(y, 0, 1, range.getYMin(), range.getYMax());
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    y = CGnuPlotUtil::map(y, 0, 1, py2, py1);
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    // TODO
  }

  return CPoint2D(x, y);
}

CPoint2D
CGnuPlotPosition::
getDistance(CGnuPlotRenderer *renderer) const
{
  CPoint2D p1 = getPoint(renderer, 0   , 0   );
  CPoint2D p2 = getPoint(renderer, p_.x, p_.y);

  return (p2 - p1);
}
