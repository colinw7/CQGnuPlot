#include <CGnuPlotStyleTestPalette.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotStyleTestPalette::
CGnuPlotStyleTestPalette() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::TEST_PALETTE)
{
}

void
CGnuPlotStyleTestPalette::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  renderer->setRegion(CBBox2D(0, 0, 1, 1));

  double px1, py1, px2, py2;

  renderer->windowToPixel(0.0, 0.0, &px1, &py1);
  renderer->windowToPixel(1.0, 1.0, &px2, &py2);

  double wx1, wy1, wx2, wy2;

  renderer->pixelToWindow(0, py1 + 32, &wx1, &wy1);
  renderer->pixelToWindow(0, py1 + 64, &wx2, &wy2);

  bool   first = true;
  double r1, g1, b1, m1, x1;

  for (double i = px1; i <= px2; i += 1.0) {
    double wx, wy;

    renderer->pixelToWindow(i, 0, &wx, &wy);

    CRGBA c = plot->group()->palette()->getColor(wx);

    renderer->drawLine(CPoint2D(wx, wy1), CPoint2D(wx, wy2), 0.0, c);

    //double x = (i - px1)/(px2 - px1);

    double x2 = wx;
    double r2 = c.getRed  ();
    double g2 = c.getGreen();
    double b2 = c.getBlue ();
    double m2 = c.getGray();

    if (! first) {
      renderer->drawLine(CPoint2D(x1, r1), CPoint2D(x2, r2), 0, CRGBA(1,0,0));
      renderer->drawLine(CPoint2D(x1, g1), CPoint2D(x2, g2), 0, CRGBA(0,1,0));
      renderer->drawLine(CPoint2D(x1, b1), CPoint2D(x2, b2), 0, CRGBA(0,0,1));
      renderer->drawLine(CPoint2D(x1, m1), CPoint2D(x2, m2), 0, CRGBA(0,0,0));
    }

    x1 = x2;
    r1 = r2;
    g1 = g2;
    b1 = b2;
    m1 = m2;

    first = false;
  }

  renderer->drawRect(CBBox2D(0.0, wy1, 1.0, wy2), CRGBA(0,0,0), 1);

#if 0
  CGnuPlotAxis *xaxis = group()->getPlotAxis('x', 1);
  CGnuPlotAxis *yaxis = group()->getPlotAxis('y', 1);

  xaxis->drawAxis(0.0);
  yaxis->drawAxis(0.0);

  xaxis->drawGrid(0.0, 1.0);
  yaxis->drawGrid(0.0, 1.0);
#endif
}

CBBox2D
CGnuPlotStyleTestPalette::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
