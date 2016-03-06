#include <CGnuPlotStyleXErrorLines.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleXErrorLines::
CGnuPlotStyleXErrorLines() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::XERRORLINES)
{
}

void
CGnuPlotStyleXErrorLines::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA lc = stroke.color();

  //---

  double ph = 0;

  if (! renderer->isPseudo())
    ph = renderer->pixelHeightToWindowHeight(4);

  //---

  int      i = 0;
  CPoint2D p1(0,0);

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x  = reals[0];
    double y  = reals[1];
    double xl = x;
    double xh = x;

    CRGBA lc1 = lc;

    // x y xlow xhigh
    if      ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      xl = reals[2];
      xh = reals[3];

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
      double dx = reals[2];

      xl = x - dx;
      xh = x + dx;

      if (isCalcColor) {
        double z = reals[3];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc1 = lineStyle.calcColor(plot, z);
      }
    }

    CPoint2D p2 = CPoint2D(x, y);

    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), lc1, 1.0);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), lc1, 1.0);
    renderer->drawClipLine(CPoint2D(xh, y - ph), CPoint2D(xh, y + ph), lc1, 1.0);

    if (i > 0)
      renderer->drawLine(p1, p2, lc1, 1);

    p1 = p2;

    ++i;
  }
}
