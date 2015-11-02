#include <CGnuPlotStyleBoxErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBoxBarObject.h>

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

  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  CBBox2D bbox = plot->bbox();

  double ymin = bbox.getYMin();

  double y2 = std::max(0.0, ymin);

  //---

  if (! renderer->isPseudo())
    plot->updateBoxBarCacheSize(plot->getPoints2D().size());

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

    //---

    COptRGBA lc;

    if (colorVal.isValid()) {
      if (renderer->isPseudo())
        renderer->setCBValue(colorVal.getValue());
      else
        lc = lineStyle.calcColor(plot, colorVal.getValue());
    }

    //---

    CBBox2D bbox(x - dx/2, y2, x + dx/2, y);

    CPoint2D p1(x, yl);
    CPoint2D p2(x, yh);

    if (! renderer->isPseudo()) {
      CGnuPlotBoxBarObject *bar = plot->boxBarObjects()[i];

      bar->setBBox(bbox);

      bar->setValues(x, y);

      bar->setVertical(true);

      if (! bar->testAndSetUsed()) {
        CGnuPlotFillP   fill  (bar->fill  ()->dup());
        CGnuPlotStrokeP stroke(bar->stroke()->dup());

        bar->setFill  (fill  );
        bar->setStroke(stroke);

        //---

        bar->clearEndBars();

        CGnuPlotEndBarP endBar = bar->addEndBar(p1, p2);

        endBar->setStartLine(true);
        endBar->setEndLine  (true);
        endBar->setEndWidth (dx/2);

        CGnuPlotStrokeP endStroke(bar->stroke()->dup());

        endBar->setStroke(endStroke);
      }
    }
    else {
      CGnuPlotStroke stroke;

      stroke.setEnabled(true);

      renderer->strokeRect(bbox, stroke);

      renderer->strokeClipLine(p1, p2, stroke);

      double w = dx/2;

      renderer->strokeClipLine(p1 - CPoint2D(w/2, 0), p1 + CPoint2D(w/2, 0), stroke);
      renderer->strokeClipLine(p2 - CPoint2D(w/2, 0), p2 + CPoint2D(w/2, 0), stroke);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->boxBarObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleBoxErrorBars::
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
