#include <CGnuPlotStyleCandlesticks.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleCandlesticks::
CGnuPlotStyleCandlesticks() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::CANDLESTICKS)
{
}

void
CGnuPlotStyleCandlesticks::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c1 = (plot->fillType() == CGnuPlotTypes::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  CRGBA  lc = lineStyle.calcColor(CRGBA(0,0,0));
  double lw = lineStyle.width();
  CRGBA  fc = lineStyle.calcColor(c1);

  double bw = plot->boxWidth().getSpacing(0.1);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if ((! isCalcColor && reals.size() < 5) || (isCalcColor && reals.size() < 6))
      continue;

    double x = reals[0];

    double bmin = reals[1];
    double wmin = reals[2];
    double wmax = reals[3];
    double bmax = reals[4];

    double bw1 = bw;

    int ind = 5;

    if ((! isCalcColor && reals.size() > 5) || (isCalcColor && reals.size() > 6))
      bw1 = reals[++ind];

    CRGBA lc1 = lc;
    CRGBA fc1 = lc;

    if (isCalcColor) {
      double x = reals[ind];

      lc1 = lineStyle.color().calcColor(plot, x);
      fc1 = lineStyle.color().calcColor(plot, x);
    }

    renderer->drawClipLine(CPoint2D(x, wmin), CPoint2D(x, bmin), lw, lc1);
    renderer->drawClipLine(CPoint2D(x, bmax), CPoint2D(x, wmax), lw, lc1);

    double x1 = x - bw1/2;
    double x2 = x + bw1/2;

    if (plot->whiskerBars() > 0) {
      renderer->drawClipLine(CPoint2D(x1, wmin), CPoint2D(x2, wmin), lw, lc1);
      renderer->drawClipLine(CPoint2D(x1, wmax), CPoint2D(x2, wmax), lw, lc1);
    }

    CBBox2D bbox(x1, bmin, x2, bmax);

    if      (plot->fillType() == CGnuPlotTypes::FillType::PATTERN)
      renderer->patternRect(bbox, plot->fillPattern(), fc1, CRGBA(1,1,1));
    else if (plot->fillType() == CGnuPlotTypes::FillType::SOLID)
      renderer->fillRect(bbox, fc1);
    else if (bmin > bmax)
      renderer->fillRect(bbox, fc1);

    renderer->drawRect(bbox, lc1, 1);
  }
}

CBBox2D
CGnuPlotStyleCandlesticks::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
