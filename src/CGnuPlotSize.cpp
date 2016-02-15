#include <CGnuPlotSize.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CSize2D
CGnuPlotSize::
getSize(CGnuPlotRenderer *renderer) const
{
#if 0
  double x = s_.width;
  double y = s_.height;

  CPoint2D tw(x, y), gw(x, y), sw(x, y), cw(x, y);

  if (systemX_ == CGnuPlotTypes::CoordSys::SECOND ||
      systemY_ == CGnuPlotTypes::CoordSys::SECOND)
    renderer->secondToWindow(CPoint2D(s_.width, s_.height), tw);

  if (systemX_ == CGnuPlotTypes::CoordSys::GRAPH ||
      systemY_ == CGnuPlotTypes::CoordSys::GRAPH)
    renderer->graphToWindow(CPoint2D(s_.width, s_.height), gw);

  if (systemX_ == CGnuPlotTypes::CoordSys::SCREEN ||
      systemY_ == CGnuPlotTypes::CoordSys::SCREEN)
    renderer->screenToWindow(CPoint2D(s_.width, s_.height), sw);

  if (systemX_ == CGnuPlotTypes::CoordSys::CHARACTER ||
      systemY_ == CGnuPlotTypes::CoordSys::CHARACTER)
    renderer->screenToChar(CPoint2D(s_.width, s_.height), cw);

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
#endif

  CGnuPlotPosition p1(CPoint3D(s_.width, s_.height, 0), systemX_);
  CGnuPlotPosition p2(CPoint3D(s_.width, s_.height, 0), systemY_);

  double x = p1.getDistance2D(renderer).x;
  double y = p2.getDistance2D(renderer).y;

  return CSize2D(x, y);
}
