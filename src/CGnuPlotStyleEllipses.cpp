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
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotEllipseStyle &ellipseStyle = plot->ellipseStyle();
  const CGnuPlotLineStyle    &lineStyle    = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA lc = lineStyle.calcColor(group, CRGBA(1,0,0));
  CRGBA fc = lc;

  if (plot->fillStyle().isTransparent())
    fc.setAlpha(plot->fillStyle().density());

  keyColor_[plot->id()] = lc;

  //---

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 2)
      continue;

    double x = reals[0];
    double y = reals[1];

    if (renderer->isPseudo() && ! renderer->isInside(CPoint2D(x, y)))
      continue;

    double w, h;

    double a = ellipseStyle.angle();

    CRGBA lc1 = lc;

    if (isCalcColor || reals.size() > 5) {
      double z;

      if (reals.size() <= 6)
        z = reals[reals.size() - 1];
      else
        z = reals[5];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else {
        lc1 = lineStyle.calcColor(plot, z);

        keyColor_[plot->id()] = lc1;
      }
    }

    if      ((! isCalcColor && reals.size() == 2) || (isCalcColor && reals.size() == 3)) {
      // TODO: get from set circle
      w = ellipseStyle.size(0).getXDistance(renderer);
      h = ellipseStyle.size(1).getYDistance(renderer);
    }
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      w = reals[2];
      h = w;
    }
    else if ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      w = reals[2];
      h = reals[3];
    }
    else if ((! isCalcColor && reals.size() == 5) || (isCalcColor && reals.size() == 6)) {
      w = reals[2];
      h = reals[3];
      a = reals[4];
    }
    else if (reals.size() == 6) {
      w = reals[2];
      h = reals[3];
      a = reals[4];
    }
    else {
      std::cerr << "Bad ellipse points" << std::endl;
      continue;
    }

    //---

    double w1 = w;
    double h1 = h;

    if (ellipseStyle.isWidthY())
      w1 = renderer->pixelWidthToWindowWidth(renderer->windowHeightToPixelHeight(w1));

    if (ellipseStyle.isHeightX())
      h1 = renderer->pixelHeightToWindowHeight(renderer->windowWidthToPixelWidth(h1));

    //---

    // TODO: use ellipse cache

    CPoint2D c(x, y);

    if (plot->fillStyle().style() == CGnuPlotTypes::FillType::SOLID)
      renderer->fillEllipse(c, w1/2, h1/2, a, fc);

    if (plot->fillStyle().hasBorder())
      renderer->drawEllipse(c, w1/2, h1/2, a, lc1, 1);
  }
}

void
CGnuPlotStyleEllipses::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  //const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA lc = keyColor_[plot->id()];

  //double lw = lineStyle.calcWidth();

  double w = fabs(p2.x - p1.x);

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double h = renderer->pixelHeightToWindowHeight(font_size - 4);

  CPoint2D c = (p1 + p2)/2;

  renderer->drawEllipse(c, w/2, h/2, 0, lc, 1);
}

CBBox2D
CGnuPlotStyleEllipses::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
