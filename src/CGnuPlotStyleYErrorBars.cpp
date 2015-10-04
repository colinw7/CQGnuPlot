#include <CGnuPlotStyleYErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleYErrorBars::
CGnuPlotStyleYErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::YERRORBARS)
{
}

void
CGnuPlotStyleYErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA lc = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double pw = 0;

  if (! renderer->isPseudo())
    pw = renderer->pixelWidthToWindowWidth(4);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double yl = y;
    double yh = y;

    CRGBA lc1 = lc;

    // x y ylow yhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      yl = reals[2];
      yh = reals[3];

      if (isCalcColor) {
        double z = reals[4];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc1 = lineStyle.calcColor(plot, z);
      }
    }
    // x y xdelta
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      double dy = reals[2];

      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double z = reals[3];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc1 = lineStyle.calcColor(plot, z);
      }
    }

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, lc1);
  }
}

CBBox2D
CGnuPlotStyleYErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
