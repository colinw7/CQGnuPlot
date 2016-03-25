#include <CGnuPlotStyleBoxes.h>
#include <CGnuPlotBoxesStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBoxBarObject.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleBoxes::
CGnuPlotStyleBoxes() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXES)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotBoxesStyleValue>("boxes");
}

void
CGnuPlotStyleBoxes::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotBoxesStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotBoxesStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createBoxesStyleValue(plot);

    value->init();

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotBoxesStyleValue>(plot, value);
  }

  //---

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CBBox2D bbox = plot->bbox2D();

  double ymin = bbox.getYMin();

  double y2 = std::max(0.0, ymin);

  double bw = value->getSpacing();

  //---

  int n = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double y = reals[1];

    if (IsNaN(y))
      continue;

    ++n;
  }

  if (! renderer->isPseudo())
    plot->updateBoxBarCacheSize(n);

  //---

  int i = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x = reals[0];
    double y = reals[1];

    if (IsNaN(y))
      continue;

    uint ind = 2;

    double bw1 = bw;

    if (value->isCalc()) {
      if ((isCalcColor && reals.size() == 4) || (! isCalcColor && reals.size() == 3)) {
        bw1 = reals[ind++];
      }
    }

    //---

    COptRGBA lc;

    if (isCalcColor && ind < reals.size()) {
      double z = reals[ind];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        lc = lineStyle.calcColor(plot, z);
    }

    //---

    CBBox2D bbox(x - bw1/2, y2, x + bw1/2, y);

    if (! renderer->isPseudo()) {
      CGnuPlotBoxBarObject *bar = plot->boxBarObjects()[i];

      bar->setBBox(bbox);

      bar->setValues(x, y);

      if (! bar->testAndSetUsed()) {
        CGnuPlotFillP   fill  (bar->fill  ()->dup());
        CGnuPlotStrokeP stroke(bar->stroke()->dup());

        if (lc.isValid())
          stroke->setColor(lc.getValue());

        bar->setFill  (fill  );
        bar->setStroke(stroke);
      }
    }
    else {
      CGnuPlotStroke stroke;

      stroke.setEnabled(true);

      renderer->strokeRect(bbox, stroke);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->boxBarObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleBoxes::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double h = renderer->pixelHeightToWindowHeight(font_size - 4);

  CBBox2D hbbox(p1.x, p1.y - h/2, p2.x, p1.y + h/2);

  stroke.setWidth(1);

  renderer->fillRect  (hbbox, fill  );
  renderer->strokeRect(hbbox, stroke);
}
