#include <CGnuPlotCoordValue.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>

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
