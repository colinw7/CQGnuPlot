#include <CGnuPlotStyleCircles.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotRenderer.h>

namespace {

struct PieSlice {
  PieSlice(const CPoint2D &c_, double r_, double a1_, double a2_,
           const COptRGBA &lc_, const COptRGBA &fc_) :
   c(c_), r(r_), a1(a1_), a2(a2_), lc(lc_), fc(fc_) {
  }

  CPoint2D c;
  double   r;
  double   a1, a2;
  COptRGBA lc, fc;
};

struct Ellipse {
  Ellipse(const CPoint2D &c_, double w_, double h_, const COptRGBA &lc_, const COptRGBA &fc_) :
   c(c_), w(w_), h(h_), lc(lc_), fc(fc_) {
  }

  const CPoint2D c;
  double         w, h;
  COptRGBA       lc, fc;
};

}

//------

CGnuPlotStyleCircles::
CGnuPlotStyleCircles() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::CIRCLES)
{
}

void
CGnuPlotStyleCircles::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  typedef std::vector<PieSlice> PieSlices;
  typedef std::vector<Ellipse>  Ellipses;

  PieSlices pieSlices;
  Ellipses  ellipses;

  //---

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

    if (renderer->isPseudo() || ! plot->isCacheActive()) {
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
    else {
      if (is_angle) {
        COptRGBA fc2(fc1);
        COptRGBA lc2(lc1);

        pieSlices.push_back(PieSlice(c, r, a1, a2, lc2, fc2));
      }
      else {
        COptRGBA lc2, fc2;

        if (plot->fillStyle().style() == CGnuPlotTypes::FillType::SOLID)
          fc2 = fc1;

        if (plot->fillStyle().hasBorder())
          lc2 = lc1;

        ellipses.push_back(Ellipse(c, w, h, lc2, fc2));
      }
    }
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    plot->updatePieCacheSize    (pieSlices.size());
    plot->updateEllipseCacheSize(ellipses .size());

    int i = 0;

    for (const auto &o : pieSlices) {
      CGnuPlotPieObject *pie = plot->pieObjects()[i];

      pie->setCenter(o.c);
      pie->setRadius(o.r);
      pie->setAngle1(o.a1);
      pie->setAngle2(o.a2);

      if (o.lc.isValid())
        pie->setLineColor(o.lc.getValue());

      if (o.fc.isValid())
        pie->setFillColor(o.fc.getValue());

      ++i;
    }

    i = 0;

    for (const auto &o : ellipses) {
      CGnuPlotEllipseObject *ellipse = plot->ellipseObjects()[i];

      ellipse->setCenter(o.c);
      ellipse->setSize  (CSize2D(o.w, o.h));

      if (o.lc.isValid())
        ellipse->setLineColor(o.lc.getValue());

      if (o.fc.isValid())
        ellipse->setFillColor(o.fc.getValue());

      ++i;
    }
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    for (const auto &pie : plot->pieObjects())
      pie->draw(renderer);

    for (const auto &ellipse : plot->ellipseObjects())
      ellipse->draw(renderer);
  }
}

CBBox2D
CGnuPlotStyleCircles::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}