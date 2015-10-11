#include <CGnuPlotStyleBoxes.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBarObject.h>

CGnuPlotStyleBoxes::
CGnuPlotStyleBoxes() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXES)
{
}

void
CGnuPlotStyleBoxes::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CBBox2D bbox = plot->getBBox();

  double ymin = bbox.getYMin();

  double bw = plot->boxWidth().getSpacing(plot->getXSpacing());

  double y2 = std::max(0.0, ymin);

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
    plot->updateBarCacheSize(n);

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

    if (plot->boxWidth().isCalc()) {
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
      CGnuPlotBarObject *bar = plot->barObjects()[i];

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
      //CGnuPlotFill   fill;
      CGnuPlotStroke stroke;

      stroke.setEnabled(true);

      //renderer->fillRect  (bbox, fill  );
      renderer->strokeRect(bbox, stroke);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->barObjects())
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

CBBox2D
CGnuPlotStyleBoxes::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
