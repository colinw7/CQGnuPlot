#include <CGnuPlotStyleBoxXYErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBarObject.h>

CGnuPlotStyleBoxXYErrorBars::
CGnuPlotStyleBoxXYErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXXYERRORBARS)
{
}

void
CGnuPlotStyleBoxXYErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA lc = lineStyle.calcColor(plot->group(), CRGBA(0,0,0));
  CRGBA fc = (plot->fillType() == CGnuPlotTypes::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  if (! renderer->isPseudo())
    plot->updateBarCacheSize(plot->getPoints2D().size());

  int i = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    CRGBA fc1 = fc;

    // x y xlow xhigh ylow yhigh
    if      ((! isCalcColor && reals.size() == 6) || (isCalcColor && reals.size() == 7)) {
      xl = reals[2];
      xh = reals[3];
      yl = reals[4];
      yh = reals[5];

      if (isCalcColor) {
        double z = reals[6];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          fc1 = lineStyle.calcColor(plot, z);
      }
    }
    // x y xdelta ydelta
    else if ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      double dx = reals[2];
      double dy = reals[3];

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double z = reals[4];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          fc1 = lineStyle.calcColor(plot, z);
      }
    }

    CBBox2D bbox(xl, yl, xh, yh);

    fc1.setAlpha(0.5);

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = plot->barObjects()[i];

      bar->setBBox(bbox);

      bar->setValues(x, y);

      if (! bar->isInitialized()) {
        bar->setFillType   (plot->fillType());
        bar->setFillPattern(plot->fillPattern());
        bar->setFillColor  (fc1);

        bar->setBorder   (true);
        bar->setLineColor(lc);

        bar->setInitialized(true);
      }
    }
    else
      renderer->drawRect(bbox, lc, 1);

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->barObjects())
      bar->draw(renderer);
  }
}

CBBox2D
CGnuPlotStyleBoxXYErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
