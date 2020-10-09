#include <CGnuPlotStyleCircles.h>
#include <CGnuPlotCirclesStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPieObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>

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
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotCirclesStyleValue>("circles");
}

void
CGnuPlotStyleCircles::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotCirclesStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotCirclesStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createCirclesStyleValue(plot);

    value->init(plot->circlesStyleValue());

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotCirclesStyleValue>(plot, value);
  }

  //---

  typedef std::vector<PieSlice> PieSlices;
  typedef std::vector<Ellipse>  Ellipses;

  PieSlices pieSlices;
  Ellipses  ellipses;

  //---

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CGnuPlotFill   fill(plot);
  CGnuPlotStroke stroke(plot);

  //---

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals) || reals.size() < 2)
      continue;

    double x = reals[0];
    double y = reals[1];

    double r  = 1.0;
    double a1 = 0.0;
    double a2 = 360.0;

    CRGBA lc1 = stroke.color();
    CRGBA fc1 = fill  .color();

    bool is_angle = false;

    if (isCalcColor || reals.size() > 5) {
      double z;

      if (reals.size() <= 6)
        z = reals[reals.size() - 1];
      else
        z = reals[5];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else {
        fc1 = lineStyle.calcColor(plot, z);
        lc1 = fc1;
      }
    }

    //---

    if      ((! isCalcColor && reals.size() == 2) || (isCalcColor && reals.size() == 3)) {
      // TODO: get from set circle
      r = 1;
    }
    else if ((! isCalcColor && reals.size() == 3) || (isCalcColor && reals.size() == 4)) {
      r = reals[2];
    }
    else if ((! isCalcColor && reals.size() == 5) || (isCalcColor && reals.size() == 6)) {
      is_angle = true;

      r  = reals[2];
      a1 = reals[3];
      a2 = reals[4];
    }
    else if (reals.size() == 6) {
      is_angle = true;

      r  = reals[2];
      a1 = reals[3];
      a2 = reals[4];
    }
    else {
      std::cerr << "Bad circle points" << std::endl;
      continue;
    }

    //---

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
        if (fill.type() == CGnuPlotTypes::FillType::SOLID)
          renderer->fillEllipse(c, w, h, 0, fc1);

        if (stroke.isEnabled())
          renderer->drawEllipse(c, w, h, 0, lc1, 1);
      }
    }
    else {
      COptRGBA lc2, fc2;

      if (fill.type() == CGnuPlotTypes::FillType::SOLID)
        fc2 = fc1;

      if (stroke.isEnabled())
        lc2 = lc1;

      if (is_angle) {
        pieSlices.push_back(PieSlice(c, r, a1, a2, lc2, fc2));
      }
      else {
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
      pie->setWedge (value->wedge());

      if (! pie->testAndSetUsed()) {
        CGnuPlotFillP   fill1  (pie->fill  ()->dup());
        CGnuPlotStrokeP stroke1(pie->stroke()->dup());

        if (o.lc.isValid())
          stroke1->setColor(o.lc.getValue());

        if (o.fc.isValid())
          fill1->setColor(o.fc.getValue());

        pie->setFill  (fill1  );
        pie->setStroke(stroke1);
      }

      ++i;
    }

    i = 0;

    for (const auto &o : ellipses) {
      CGnuPlotEllipseObject *ellipse = plot->ellipseObjects()[i];

      ellipse->setCenter(o.c);
      ellipse->setSize  (CSize2D(o.w, o.h));

      if (! ellipse->testAndSetUsed()) {
        CGnuPlotFillP   fill1  (ellipse->fill  ()->dup());
        CGnuPlotStrokeP stroke1(ellipse->stroke()->dup());

        if (o.lc.isValid())
          stroke1->setColor(o.lc.getValue());

        if (o.fc.isValid())
          fill1->setColor(o.fc.getValue());

        ellipse->setFill  (fill1  );
        ellipse->setStroke(stroke1);
      }

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
