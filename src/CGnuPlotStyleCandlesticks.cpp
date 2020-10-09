#include <CGnuPlotStyleCandlesticks.h>
#include <CGnuPlotCandlesticksStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBoxBarObject.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleCandlesticks::
CGnuPlotStyleCandlesticks() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::CANDLESTICKS)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotCandlesticksStyleValue>("candlesticks");
}

void
CGnuPlotStyleCandlesticks::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotCandlesticksStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotCandlesticksStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createCandlesticksStyleValue(plot);

    value->init();

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotCandlesticksStyleValue>(plot, value);
  }

  //---

  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  double bw = value->getSpacing();

  //---

  if (! renderer->isPseudo())
    plot->updateBoxBarCacheSize(plot->getPoints2D().size());

  int i = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    uint minN = (isCalcColor ? 6 : 5);

    while (reals.size() < minN)
      reals.push_back(0);

    double x = reals[0];

    double bmin = reals[1];
    double wmin = reals[2];
    double wmax = reals[3];
    double bmax = reals[4];

    if (renderer->isPseudo() && ! renderer->isInside(CPoint2D(x, bmin)))
      continue;

    double bw1 = bw;

    int ind = 5;

    if ((! isCalcColor && reals.size() > 5) || (isCalcColor && reals.size() > 6))
      bw1 = reals[++ind];

    CRGBA lc1 = stroke.color();
    CRGBA fc1 = fill  .color();

    if (isCalcColor) {
      double z = reals[ind];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else {
        lc1 = lineStyle.calcColor(plot, z);
        fc1 = lineStyle.calcColor(plot, z);
      }
    }

    //---

    CPoint2D p1(x, wmin);
    CPoint2D p2(x, bmin);
    CPoint2D p3(x, bmax);
    CPoint2D p4(x, wmax);

    p1 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);
    p2 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);
    p3 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p3);
    p4 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p4);

    double x1 = p1.x - bw1/2;
    double x2 = p1.x + bw1/2;

    CBBox2D bbox(x1, p2.y, x2, p3.y);

    CGnuPlotTypes::FillType fillType = fill.type();

    if (fillType == CGnuPlotTypes::FillType::EMPTY) {
      if (bmin > bmax)
        fillType = CGnuPlotTypes::FillType::SOLID;
    }

    if (! renderer->isPseudo()) {
      CGnuPlotBoxBarObject *bar = plot->boxBarObjects()[i];

      bar->setBBox(bbox);

      bar->setValues(p1.x, bmin);

      bar->setVertical(true);

      if (! bar->testAndSetUsed()) {
        CGnuPlotFillP   fill1  (bar->fill  ()->dup());
        CGnuPlotStrokeP stroke1(bar->stroke()->dup());

        fill1  ->setType (fillType);
        fill1  ->setColor(fc1);
        stroke1->setColor(lc1);

        bar->setFill  (fill1  );
        bar->setStroke(stroke1);

        //---

        bar->clearEndBars();

        CGnuPlotEndBarP endBar1 = bar->addEndBar(p1, p2);
        CGnuPlotEndBarP endBar2 = bar->addEndBar(p3, p4);

        bool hasBars = plot->whiskerBars().isValid();

        endBar1->setStartLine(hasBars); endBar1->setEndLine(false  );
        endBar2->setStartLine(false  ); endBar2->setEndLine(hasBars);

        if (hasBars) {
          endBar1->setEndWidth(bw1*plot->whiskerBars().getValue());
          endBar2->setEndWidth(bw1*plot->whiskerBars().getValue());
        }

        CGnuPlotStrokeP endStroke1(bar->stroke()->dup());
        CGnuPlotStrokeP endStroke2(bar->stroke()->dup());

        endBar1->setStroke(endStroke1);
        endBar2->setStroke(endStroke2);
      }
    }
    else {
      renderer->drawClipLine(p1, p2, lc1, stroke.width());
      renderer->drawClipLine(p3, p4, lc1, stroke.width());

      if (plot->whiskerBars() > 0) {
        renderer->drawClipLine(CPoint2D(x1, p1.y), CPoint2D(x2, p1.y), lc1, stroke.width());
        renderer->drawClipLine(CPoint2D(x1, p4.y), CPoint2D(x2, p4.y), lc1, stroke.width());
      }

      if (fillType != CGnuPlotTypes::FillType::EMPTY) {
        CGnuPlotFill fill1 = fill;

        fill1.setType (fillType);
        fill1.setColor(fc1);

        renderer->fillClippedRect(bbox, fill1);
      }

      renderer->drawClippedRect(bbox, lc1, 1);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->boxBarObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleCandlesticks::
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
