#include <CGnuPlotStyleVectors.h>
#include <CGnuPlotArrowObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleVectors::
CGnuPlotStyleVectors() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::VECTORS)
{
}

void
CGnuPlotStyleVectors::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle  &lineStyle  = plot->lineStyle();
  const CGnuPlotArrowStyle &arrowStyle = plot->arrowStyle();

  bool isCalcColor = lineStyle.isCalcColor();
  bool isVarArrow  = plot->arrowStyle().isVariable();

  uint numExtra = 0;

  if (isCalcColor) ++numExtra;
  if (isVarArrow ) ++numExtra;

  typedef std::vector<CGnuPlotArrowData> ArrowDatas;

  ArrowDatas arrowDatas;

  CRGBA lc = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    CPoint2D from, to;

    int pos = 0;

    if      (reals.size() == 4 + numExtra) {
      from = CPoint2D(reals[0], reals[1]);
      to   = from + CPoint2D(reals[2], reals[3]);

      pos = 4;
    }
    else if ((reals.size() == 6 + numExtra)) {
      from = CPoint2D(reals[0], reals[1]); // 2=z
      to   = from + CPoint2D(reals[3], reals[4]); // 5=z

      pos = 6;
    }
    else
      continue;

    CRGBA lc1 = lc;

    if (isCalcColor) {
      double z = reals[pos++];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        lc1 = lineStyle.calcColor(plot, z);
    }

    CGnuPlotArrowStyle as = arrowStyle;

    if (isVarArrow) {
      double x = reals[pos++];

      as = plot->app()->arrowStyle(x);

      if (as.lineColor().isValid())
        lc1 = as.lineColor().getValue();
    }

    if (to == from) continue;

    if      (! renderer->isPseudo() && plot->isCacheActive()) {
      CGnuPlotArrowData arrowData;

      arrowData.setStyle(as);

      arrowData.setLineColor(lc1);

      arrowData.setFrom(CPoint3D(from.x, from.y, 0));
      arrowData.setTo  (CPoint3D(to  .x, to  .y, 0));

      arrowDatas.push_back(arrowData);
    }
    else if (! renderer->isPseudo()) {
      CGnuPlotArrow arrow(plot->group());

      arrow.setStyle(as);

      arrow.setLineColor(lc1);

      arrow.setFrom(CPoint3D(from.x, from.y, 0));
      arrow.setTo  (CPoint3D(to  .x, to  .y, 0));

      arrow.draw(renderer);
    }
    else {
      renderer->drawPoint(from, lc1);
      renderer->drawPoint(to  , lc1);
    }
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    plot->updateArrowCacheSize(arrowDatas.size());

    int i = 0;

    for (const auto &arrowData : arrowDatas) {
      CGnuPlotArrowObject *arrow = plot->arrowObjects()[i];

      if (! arrow->testAndSetUsed()) {
        arrow->setData(arrowData);
      }
      else {
        arrow->setFrom(arrowData.getFrom());
        arrow->setTo  (arrowData.getTo  ());
      }

      ++i;
    }
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    for (const auto &arrow : plot->arrowObjects())
      arrow->draw(renderer);
  }
}

void
CGnuPlotStyleVectors::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle  &lineStyle  = plot->lineStyle();
  const CGnuPlotArrowStyle &arrowStyle = plot->arrowStyle();

  CRGBA lc = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  //---

  CGnuPlotArrow arrow(plot->group());

  arrow.setStyle(arrowStyle);

  arrow.setLineColor(lc);

  arrow.setFrom(CPoint3D(p1.x, p1.y, 0));
  arrow.setTo  (CPoint3D(p2.x, p2.y, 0));

  arrow.draw(renderer);
}

CBBox2D
CGnuPlotStyleVectors::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
