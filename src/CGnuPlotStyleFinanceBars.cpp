#include <CGnuPlotStyleFinanceBars.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleFinanceBars::
CGnuPlotStyleFinanceBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::FINANCEBARS)
{
}

void
CGnuPlotStyleFinanceBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA  lc = lineStyle.calcColor(group, CRGBA(0,0,0));
  double lw = lineStyle.calcWidth();

  double sl = 0;

  if (! renderer->isPseudo())
    sl = renderer->pixelWidthToWindowWidth(4);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if ((! isCalcColor && reals.size() < 5) || (isCalcColor && reals.size() < 6))
      continue;

    double date  = reals[0];
    double open  = reals[1];
    double low   = reals[2];
    double high  = reals[3];
    double close = reals[4];

    if (renderer->isPseudo() && ! renderer->isInside(CPoint2D(date, low)))
      continue;

    CRGBA lc1 = lc;

    if (isCalcColor) {
      double x = reals[5];

      lc1 = lineStyle.calcColor(plot, x);
    }

    CPoint2D p1(date, low );
    CPoint2D p2(date, high);

    p1 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);
    p2 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);

    renderer->drawClipLine(p1, p2, lw, lc1);

    CPoint2D p3(date, open );
    CPoint2D p4(date, close);

    p3 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p3);
    p4 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p4);

    renderer->drawClipLine(p3, p3 - CPoint2D(sl, 0), lw, lc1);
    renderer->drawClipLine(p4, p4 + CPoint2D(sl, 0), lw, lc1);
  }
}

CBBox2D
CGnuPlotStyleFinanceBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
