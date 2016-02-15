#include <CGnuPlotPosition.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CPoint3D
CGnuPlotPosition::
getPoint3D(CGnuPlotRenderer *renderer) const
{
  return getPoint(renderer, p_.x, p_.y, p_.z);
}

CPoint2D
CGnuPlotPosition::
getPoint2D(CGnuPlotRenderer *renderer) const
{
  CPoint3D p = getPoint(renderer, p_.x, p_.y, p_.z);

  return CPoint2D(p.x, p.y);
}

CPoint3D
CGnuPlotPosition::
getPoint(CGnuPlotRenderer *renderer, double x, double y, double z) const
{
  CPoint2D tw(x, y), gw(x, y), sw(x, y), cw(x, y);

  if (systemX_ == CGnuPlotTypes::CoordSys::SECOND ||
      systemY_ == CGnuPlotTypes::CoordSys::SECOND)
    renderer->secondToWindow(CPoint2D(x, y), tw);

  if (systemX_ == CGnuPlotTypes::CoordSys::GRAPH ||
      systemY_ == CGnuPlotTypes::CoordSys::GRAPH)
    renderer->graphToWindow(CPoint2D(x, y), gw);

  if (systemX_ == CGnuPlotTypes::CoordSys::SCREEN ||
      systemY_ == CGnuPlotTypes::CoordSys::SCREEN)
    renderer->screenToWindow(CPoint2D(x, y), sw);

  if (systemX_ == CGnuPlotTypes::CoordSys::CHARACTER ||
      systemY_ == CGnuPlotTypes::CoordSys::CHARACTER)
    renderer->charToWindow(CPoint2D(x, y), cw);

  //---

  if      (systemX_ == CGnuPlotTypes::CoordSys::SECOND) {
    x = tw.x;
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::GRAPH) {
    x = gw.x;
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::SCREEN) {
    x = sw.x;
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    x = cw.x;
  }

  //---

  if      (systemY_ == CGnuPlotTypes::CoordSys::SECOND) {
    y = tw.y;
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::GRAPH) {
    y = gw.y;
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::SCREEN) {
    y = sw.y;
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    y = cw.y;
  }

  //---

  if      (systemZ_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (systemZ_ == CGnuPlotTypes::CoordSys::GRAPH) {
    // TODO
  }
  else if (systemZ_ == CGnuPlotTypes::CoordSys::SCREEN) {
    // TODO
  }
  else if (systemZ_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    // TODO
  }

  return CPoint3D(x, y, z);
}

CPoint3D
CGnuPlotPosition::
getDistance3D(CGnuPlotRenderer *renderer) const
{
  CPoint3D p1 = getPoint(renderer, 0   , 0   , 0   );
  CPoint3D p2 = getPoint(renderer, p_.x, p_.y, p_.z);

  CPoint3D d = p2 - p1;

  return d;
}

CPoint2D
CGnuPlotPosition::
getDistance2D(CGnuPlotRenderer *renderer) const
{
  CPoint3D d = getDistance3D(renderer);

  return CPoint2D(d.x, d.y);
}
