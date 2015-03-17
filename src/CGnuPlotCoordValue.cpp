#include <CGnuPlotCoordValue.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotUtil.h>

double
CGnuPlotCoordValue::
getXValue(CGnuPlotRenderer *renderer) const
{
  double x = value_;

  if      (system_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    x = CGnuPlotUtil::map(value_, 0, 1, range.getXMin(), range.getXMax());
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    x = CGnuPlotUtil::map(value_, 0, 1, px1, px2);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    // TODO
  }

  return x;
}

double
CGnuPlotCoordValue::
getYValue(CGnuPlotRenderer *renderer) const
{
  double y = value_;

  if      (system_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    y = CGnuPlotUtil::map(value_, 0, 1, range.getYMin(), range.getYMax());
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    y = CGnuPlotUtil::map(value_, 0, 1, py1, py2);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    // TODO
  }

  return y;
}

double
CGnuPlotCoordValue::
pixelXValue(CGnuPlotRenderer *renderer) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
    return value_; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH)
    return value_; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    const CISize2D &s = renderer->window()->size();

    return value_*s.width;
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
    return value_;
  else
    return renderer->windowWidthToPixelWidth(value_);
}

double
CGnuPlotCoordValue::
pixelYValue(CGnuPlotRenderer *renderer) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
    return value_; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH)
    return value_; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    const CISize2D &s = renderer->window()->size();

    return value_*s.height;
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
    return value_;
  else
    return renderer->windowHeightToPixelHeight(value_);
}
