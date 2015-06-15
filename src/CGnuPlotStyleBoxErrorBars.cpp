#include <CGnuPlotStyleBoxErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBarObject.h>

CGnuPlotStyleBoxErrorBars::
CGnuPlotStyleBoxErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXERRORBARS)
{
}

void
CGnuPlotStyleBoxErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CBBox2D bbox = plot->getBBox();

  double ymin = bbox.getYMin();

  double y2 = std::max(0.0, ymin);

  //CRGBA lc = CRGBA(0,0,0);
  CRGBA fc = (fillStyle.style() == CGnuPlotTypes::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  if (! renderer->isPseudo())
    plot->updateBarCacheSize(plot->getPoints2D().size());

  int i = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    COptReal colorVal;

    if (isCalcColor && ! reals.empty()) {
      colorVal = reals.back();

      reals.pop_back();
    }

    while (reals.size() < 3)
      reals.push_back(0.0);

    double x  = reals[0];
    double y  = reals[1];
    double dx = plot->boxWidth().getSpacing(plot->getXSpacing());
    double dy = 0.0;
    double yl = y;
    double yh = y;

    // x y ydelta
    if      (reals.size() == 3) {
      dy = reals[2];

      yl = y - dy;
      yh = y + dy;
    }
    else if (reals.size() == 4) {
      // x y ydelta xdelta
      if (! plot->boxWidth().isAutoWidth()) {
        dx = reals[2];
        dy = reals[3];

        yl = y - dy;
        yh = y + dy;
      }
      // x y ylow yhigh
      else {
        yl = reals[2];
        yh = reals[3];
      }
    }
    // x y ylow yhigh xdelta
    else if (reals.size() >= 5) {
      yl = reals[2];
      yh = reals[3];
      dx = reals[4];
    }

    CRGBA fc1 = fc;

    if (colorVal.isValid()) {
      fc1 = lineStyle.calcColor(plot, colorVal.getValue());
    }

    CBBox2D bbox(x - dx/2, y2, x + dx/2, y);

    CRGBA fc2 = fc1;

    fc2.setAlpha(0.5); // ???

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = plot->barObjects()[i];

      bar->setBBox (bbox);
      bar->setValue(y);

      if (! bar->isInitialized()) {
        bar->setFillType   (fillStyle.style());
        bar->setFillPattern(fillStyle.pattern());
        bar->setFillColor  (fc2);

        bar->setBorder   (fillStyle.hasBorder());
        bar->setLineColor(fc1);

        bar->setInitialized(true);
      }
    }
    else
      renderer->drawRect(bbox, fc1, 1);

    double lw = lineStyle.calcWidth();

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), lw, fc1);

    double w = dx/2;

    renderer->drawClipLine(CPoint2D(x - w/2, yl), CPoint2D(x + w/2, yl), lw, fc1);
    renderer->drawClipLine(CPoint2D(x - w/2, yh), CPoint2D(x + w/2, yh), lw, fc1);

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->barObjects())
      bar->draw(renderer);
  }
}

CBBox2D
CGnuPlotStyleBoxErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
