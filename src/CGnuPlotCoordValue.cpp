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
  CPoint2D w(x, 0);

  if      (system_ == CGnuPlotTypes::CoordSys::SECOND) {
    renderer->secondToWindow(CPoint2D(x, 0.0), w);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    renderer->graphToWindow(CPoint2D(x, 0.0), w);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    renderer->screenToWindow(CPoint2D(x, 0.0), w);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    renderer->charToWindow(CPoint2D(x, 0.0), w);
  }

  return w.x;
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
  CPoint2D w(0, y);

  if      (system_ == CGnuPlotTypes::CoordSys::SECOND) {
    renderer->secondToWindow(CPoint2D(0.0, y), w);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::GRAPH) {
    renderer->graphToWindow(CPoint2D(0.0, y), w);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::SCREEN) {
    renderer->screenToWindow(CPoint2D(0.0, y), w);
  }
  else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER) {
    renderer->charToWindow(CPoint2D(0.0, y), w);
  }

  return w.y;
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
