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

  if      (systemX_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    x = CGnuPlotUtil::map(s_.width, 0, 1, range.getXMin(), range.getXMax());
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    x = CGnuPlotUtil::map(s_.width, 0, 1, px1, px2);
  }
  else if (systemX_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    CFontPtr font = renderer->getFont();

    x = renderer->pixelWidthToWindowWidth(font->getStringWidth("X")*s_.width);
  }

  if      (systemY_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    y = CGnuPlotUtil::map(s_.height, 0, 1, range.getYMin(), range.getYMax());
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    y = CGnuPlotUtil::map(s_.height, 0, 1, py1, py2);
  }
  else if (systemY_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    CFontPtr font = renderer->getFont();

    y = renderer->pixelHeightToWindowHeight(font->getCharHeight()*s_.height);
  }
#endif

  CGnuPlotPosition p1(CPoint3D(s_.width, s_.height, 0), systemX_);
  CGnuPlotPosition p2(CPoint3D(s_.width, s_.height, 0), systemY_);

  double x = p1.getDistance2D(renderer).x;
  double y = p2.getDistance2D(renderer).y;

  return CSize2D(x, y);
}
