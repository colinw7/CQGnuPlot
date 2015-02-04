#include <CGnuPlotPosition.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CPoint2D
CGnuPlotPosition::
getPoint(CGnuPlotRenderer *renderer) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
    return p_; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    double x = CGnuPlotUtil::map(p_.x, 0, 1, range.getXMin(), range.getXMax());
    double y = CGnuPlotUtil::map(p_.y, 0, 1, range.getYMin(), range.getYMax());

    return CPoint2D(x, y);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    double x = CGnuPlotUtil::map(p_.x, 0, 1, px1, px2);
    double y = CGnuPlotUtil::map(p_.y, 0, 1, py1, py2);

    return CPoint2D(x, y);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
    return p_; // TODO
  else {
    return p_;
  }
}
