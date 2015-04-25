#include <CGnuPlotStyleEllipses.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleEllipses::
CGnuPlotStyleEllipses() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ELLIPSES)
{
}

void
CGnuPlotStyleEllipses::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA lc = lineStyle.calcColor(CRGBA(1,0,0));
  CRGBA fc = lc;

  if (plot->fillStyle().isTransparent())
    fc.setAlpha(plot->fillStyle().density());

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 2)
      continue;

    double x = reals[0];
    double y = reals[1];

    double w = 0.5;
    double h = 0.3;
    double a = 0;

    CRGBA lc1 = lc;

    if      ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      w = reals[2];
      h = w;

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(plot, x);
      }
    }
    else if ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      w = reals[2];
      h = reals[3];

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(plot, x);
      }
    }
    else if ((! isCalcColor && reals.size() == 5) || (isCalcColor && reals.size() == 6)) {
      w = reals[2];
      h = reals[3];
      a = reals[4];

      if (isCalcColor) {
        double x = reals[5];

        lc1 = lineStyle.color().calcColor(plot, x);
      }
    }
    else
      continue;

    CPoint2D c(x, y);

    if (plot->fillStyle().style() == CGnuPlotTypes::FillType::SOLID)
      renderer->fillEllipse(c, w/2, h/2, a, fc);

    if (plot->fillStyle().hasBorder())
      renderer->drawEllipse(c, w/2, h/2, a, lc1, 1);
  }
}

CBBox2D
CGnuPlotStyleEllipses::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
