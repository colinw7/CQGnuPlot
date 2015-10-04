#include <CGnuPlotStyleXYErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleXYErrorBars::
CGnuPlotStyleXYErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::XYERRORBARS)
{
}

void
CGnuPlotStyleXYErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA lc = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double pw = 0, ph = 0;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (4);
    ph = renderer->pixelHeightToWindowHeight(4);
  }

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

    CRGBA lc1 = lc;

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
          lc1 = lineStyle.calcColor(plot, z);
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
          lc1 = lineStyle.calcColor(plot, z);
      }
    }

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, lc1);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, lc1);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, lc1);
  }
}

void
CGnuPlotStyleXYErrorBars::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  double lw = lineStyle.calcWidth();
  double h  = renderer->pixelHeightToWindowHeight(3);

  renderer->drawLine(p1, p2, lw, c);

  CPoint2D dy(0, h);

  renderer->drawLine(p1 - dy, p1 + dy, lw, c);
  renderer->drawLine(p2 - dy, p2 + dy, lw, c);
}

CBBox2D
CGnuPlotStyleXYErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
