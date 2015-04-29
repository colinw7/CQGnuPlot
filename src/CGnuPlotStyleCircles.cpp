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
    CRGBA fc1 = fc;

    bool is_angle = false;

    if      ((! isCalcColor && reals.size() == 2) || (isCalcColor && reals.size() == 3)) {
      r = 1; // TODO: get from set circle

      if (isCalcColor)
        fc1 = lineStyle.color().calcColor(plot, reals[2]);
    }
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      r = reals[2];

      if (isCalcColor)
        fc1 = lineStyle.color().calcColor(plot, reals[3]);
    }
    else if ((! isCalcColor && reals.size() == 5) || (isCalcColor && reals.size() == 6)) {
      is_angle = true;

      r  = reals[2];
      a1 = reals[3];
      a2 = reals[4];

      if (isCalcColor)
        fc1 = lineStyle.color().calcColor(plot, reals[5]);
    }
    else if (reals.size() == 6) {
      is_angle = true;

      r   = reals[2];
      a1  = reals[3];
      a2  = reals[4];
      fc1 = lineStyle.color().calcColor(plot, reals[5]);
    }
    else {
      std::cerr << "Bad circle points" << std::endl;
      continue;
    }

    double w = r;
    double h = w;

    if (! renderer->isPseudo())
      h = renderer->pixelHeightToWindowHeight(renderer->windowWidthToPixelWidth(w));

    CPoint2D c(x, y);

    if (is_angle) {
      renderer->fillPieSlice(c, 0, r, a1, a2, fc1);
    }
    else {
      if (plot->fillStyle().style() == CGnuPlotTypes::FillType::SOLID)
        renderer->fillEllipse(c, w, h, 0, fc1);

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
