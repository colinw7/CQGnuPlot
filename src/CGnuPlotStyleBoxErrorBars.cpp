#include <CGnuPlotStyleBoxErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBarObject.h>

CGnuPlotStyleBoxErrorBars::
CGnuPlotStyleBoxErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXERRORBARS)
{
}

void
CGnuPlotStyleBoxErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CBBox2D bbox = plot->getBBox();

  double ymin = bbox.getYMin();

  double y2 = std::max(0.0, ymin);

  CRGBA lc = lineStyle.calcColor(group, CRGBA(1,0,0));
  CRGBA fc = (fillStyle.style() == CGnuPlotTypes::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  if (! renderer->isPseudo())
    plot->updateBarCacheSize(plot->getPoints2D().size());

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

    CRGBA lc1 = lc;

    if (colorVal.isValid()) {
      if (renderer->isPseudo())
        renderer->setCBValue(colorVal.getValue());
      else
        lc1 = lineStyle.calcColor(plot, colorVal.getValue());
    }

    CBBox2D bbox(x - dx/2, y2, x + dx/2, y);

    CRGBA fc1 = fc;

    //fc1.setAlpha(0.5); // ???

    CPoint2D p1(x, yl);
    CPoint2D p2(x, yh);

    double lw = lineStyle.calcWidth();

    if (! renderer->isPseudo()) {
      CGnuPlotBarObject *bar = plot->barObjects()[i];

      bar->setBBox(bbox);

      bar->setValues(x, y);

      CGnuPlotBarObject::EndBar endBar(p1, p2);

      endBar.setStartLine(true);
      endBar.setEndLine  (true);
      endBar.setLineWidth(lw);
      endBar.setLineColor(lc1);
      endBar.setEndWidth (dx/2);

      bar->clearEndBars();

      bar->addEndBar(endBar);

      if (! bar->isInitialized()) {
        bar->setFillType   (fillStyle.style());
        bar->setFillPattern(fillStyle.pattern());
        bar->setFillColor  (fc1);

        bar->setBorder   (fillStyle.hasBorder());
        bar->setLineColor(lc1);

        bar->setInitialized(true);
      }
    }
    else {
      renderer->drawRect(bbox, lc1, 1);

      renderer->drawClipLine(p1, p2, lw, lc1);

      double w = dx/2;

      renderer->drawClipLine(p1 - CPoint2D(w/2, 0), p1 + CPoint2D(w/2, 0), lw, lc1);
      renderer->drawClipLine(p2 - CPoint2D(w/2, 0), p2 + CPoint2D(w/2, 0), lw, lc1);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->barObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleBoxErrorBars::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double h = renderer->pixelHeightToWindowHeight(font_size - 4);

  CBBox2D hbbox(p1.x, p1.y - h/2, p2.x, p1.y + h/2);

  const CRGBA &lc = lineStyle.calcColor(group, CRGBA(0,0,0));
  double       lw = 1;

  if      (plot->fillStyle().style() == CGnuPlotPlot::FillType::PATTERN)
    renderer->patternRect(hbbox, plot->fillStyle().pattern(), lc, CRGBA(1,1,1));
  else if (plot->fillStyle().style() == CGnuPlotPlot::FillType::SOLID)
    renderer->fillRect(hbbox, lineStyle.calcColor(group, CRGBA(1,1,1)));

  renderer->drawRect(hbbox, lc, lw);
}

CBBox2D
CGnuPlotStyleBoxErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
