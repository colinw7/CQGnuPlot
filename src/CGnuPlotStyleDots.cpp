#include <CGnuPlotStyleDots.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleDots::
CGnuPlotStyleDots() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::DOTS)
{
}

void
CGnuPlotStyleDots::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotStroke stroke(plot);

  CRGBA c = stroke.color();

  for (const auto &point : plot->getPoints2D()) {
    CPoint2D p;

    if (point.getPoint(p))
      renderer->drawPoint(p, c);
  }
}
