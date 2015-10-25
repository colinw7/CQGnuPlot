#include <CGnuPlotStyleEllipses.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

namespace {

struct Ellipse {
  Ellipse(const CPoint2D &c_, double w_, double h_, double a_, const CRGBA &lc_, const CRGBA &fc_) :
   c(c_), w(w_), h(h_), a(a_), lc(lc_), fc(fc_) {
  }

  const CPoint2D c;
  double         w, h;
  double         a;
  CRGBA          lc, fc;
};

}

//------

CGnuPlotStyleEllipses::
CGnuPlotStyleEllipses() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ELLIPSES)
{
}

void
CGnuPlotStyleEllipses::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  typedef std::vector<Ellipse> Ellipses;

  Ellipses  ellipses;

  //---

  const CGnuPlotEllipseStyle &ellipseStyle = plot->ellipseStyle();
  const CGnuPlotLineStyle    &lineStyle    = plot->lineStyle();

  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  keyColor_[plot->id()] = stroke.color();

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

    CRGBA lc1 = stroke.color();

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

    CPoint2D c(x, y);

    ellipses.push_back(Ellipse(c, w1/2, h1/2, a, lc1, fill.color()));
  }

  //---

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    plot->updateEllipseCacheSize(ellipses.size());

    int i = 0;

    for (const auto &o : ellipses) {
      CGnuPlotEllipseObject *ellipse = plot->ellipseObjects()[i];

      ellipse->setCenter(o.c);
      ellipse->setSize  (CSize2D(o.w, o.h));

      if (! ellipse->testAndSetUsed()) {
        ellipse->setAngle(o.a);

        CGnuPlotFillP   fill  (ellipse->fill  ()->dup());
        CGnuPlotStrokeP stroke(ellipse->stroke()->dup());

        stroke->setColor(o.lc);
        fill  ->setColor(o.fc);

        ellipse->setFill  (fill  );
        ellipse->setStroke(stroke);
      }

      ++i;
    }
  }
  else {
    for (const auto &o : ellipses) {
      fill  .setColor(o.fc);
      stroke.setColor(o.lc);

      renderer->fillEllipse  (o.c, o.w, o.h, o.a, fill);
      renderer->strokeEllipse(o.c, o.w, o.h, o.a, stroke);
    }
  }

  //---

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    for (const auto &ellipse : plot->ellipseObjects())
      ellipse->draw(renderer);
  }
}

void
CGnuPlotStyleEllipses::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CRGBA lc = keyColor_[plot->id()];

  double w = fabs(p2.x - p1.x);

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double h = renderer->pixelHeightToWindowHeight(font_size - 4);

  CPoint2D c = (p1 + p2)/2;

  renderer->drawEllipse(c, w/2, h/2, 0, lc, 1);
}
