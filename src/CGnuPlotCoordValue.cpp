#include <CGnuPlotCoordValue.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotUtil.h>

double
CGnuPlotCoordValue::
getXValue(CGnuPlotRenderer *renderer) const
{
  return getXValue(renderer, value_);
}

double
CGnuPlotCoordValue::
getXValue(CGnuPlotRenderer *renderer, double x) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    x = CGnuPlotUtil::map(x, 0, 1, range.getXMin(), range.getXMax());
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    x = CGnuPlotUtil::map(x, 0, 1, px1, px2);
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
  return getXValue(renderer, value_);
}

double
CGnuPlotCoordValue::
getYValue(CGnuPlotRenderer *renderer, double y) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND) {
    // TODO
  }
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    const CBBox2D &range = renderer->range();

    y = CGnuPlotUtil::map(y, 0, 1, range.getYMin(), range.getYMax());
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    y = CGnuPlotUtil::map(y, 0, 1, py2, py1);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    // TODO
  }

  return y;
}

double
CGnuPlotCoordValue::
getXDistance(CGnuPlotRenderer *renderer) const
{
  return getXValue(renderer, value_) - getXValue(renderer, 0);
}

double
CGnuPlotCoordValue::
getYDistance(CGnuPlotRenderer *renderer) const
{
  return getYValue(renderer, value_) - getYValue(renderer, 0);
}

double
CGnuPlotCoordValue::
pixelXValue(CGnuPlotRenderer *renderer) const
{
  return pixelXValue(renderer, value_);
}

double
CGnuPlotCoordValue::
pixelXValue(CGnuPlotRenderer *renderer, double x) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
    return x; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH)
    return x; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    const CISize2D &s = renderer->window()->size();

    return x*(s.width - 1);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
    return x;
  else
    return renderer->windowWidthToPixelWidth(x);
}

double
CGnuPlotCoordValue::
pixelYValue(CGnuPlotRenderer *renderer) const
{
  return pixelXValue(renderer, value_);
}

double
CGnuPlotCoordValue::
pixelYValue(CGnuPlotRenderer *renderer, double y) const
{
  if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
    return y; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH)
    return y; // TODO
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    const CISize2D &s = renderer->window()->size();

    return y*(s.height - 1);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
    return y;
  else
    return renderer->windowHeightToPixelHeight(y);
}
