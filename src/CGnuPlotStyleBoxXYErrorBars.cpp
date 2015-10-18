#include <CGnuPlotStyleBoxXYErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBoxBarObject.h>

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

  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  if (! renderer->isPseudo())
    plot->updateBoxBarCacheSize(plot->getPoints2D().size());

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

    COptRGBA fc;

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
          fc = lineStyle.calcColor(plot, z);
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
          fc = lineStyle.calcColor(plot, z);
      }
    }

    //fc.setAlpha(0.5));

    //----

    CBBox2D bbox(xl, yl, xh, yh);

    if (! renderer->isPseudo()) {
      CGnuPlotBoxBarObject *bar = plot->boxBarObjects()[i];

      bar->setBBox(bbox);

      bar->setValues(x, y);

      if (! bar->testAndSetUsed()) {
        CGnuPlotFillP   fill  (bar->fill  ()->dup());
        CGnuPlotStrokeP stroke(bar->stroke()->dup());

        if (fc.isValid())
          fill->setColor(fc.getValue());

        bar->setFill  (fill  );
        bar->setStroke(stroke);
      }
    }
    else {
      //CGnuPlotFill   fill;
      CGnuPlotStroke stroke;

      stroke.setEnabled(true);

      //renderer->fillRect  (bbox, fill  );
      renderer->strokeRect(bbox, stroke);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->boxBarObjects())
      bar->draw(renderer);
  }
}

CBBox2D
CGnuPlotStyleBoxXYErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
