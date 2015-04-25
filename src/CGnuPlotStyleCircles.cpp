#include <CGnuPlotStyleCircles.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleCircles::
CGnuPlotStyleCircles() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::CIRCLES)
{
}

void
CGnuPlotStyleCircles::
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

    double r  = 1.0;
    double a1 = 0.0;
    double a2 = 360.0;

    CRGBA lc1 = lc;

    bool is_angle = false;

    if      ((! isCalcColor && reals.size() == 2) || (isCalcColor && reals.size() == 3)) {
      r = 1; // TODO: get from set circle

      if (isCalcColor) {
        double x = reals[2];

        lc1 = lineStyle.color().calcColor(plot, x);
      }
    }
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      r = reals[2];

      if (isCalcColor) {
        double x = reals[3];

        lc1 = lineStyle.color().calcColor(plot, x);
      }
    }
    else if ((! isCalcColor && reals.size() == 5) || (isCalcColor && reals.size() == 6)) {
      r  = reals[2];
      a1 = reals[3];
      a2 = reals[4];

      is_angle = true;

      if (isCalcColor) {
        double x = reals[5];

        lc1 = lineStyle.color().calcColor(plot, x);
      }
    }
    else
      continue;

    double w = r;
    double h = w;

    if (! renderer->isPseudo())
      h = renderer->pixelHeightToWindowHeight(renderer->windowWidthToPixelWidth(w));

    CPoint2D c(x, y);

    if (is_angle) {
      renderer->fillPieSlice(c, 0, r, a1, a2, fc);
    }
    else {
      if (plot->fillStyle().style() == CGnuPlotTypes::FillType::SOLID)
        renderer->fillEllipse(c, w, h, 0, fc);

      if (plot->fillStyle().hasBorder())
        renderer->drawEllipse(c, w, h, 0, lc1, 1);
    }
  }
}

CBBox2D
CGnuPlotStyleCircles::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
