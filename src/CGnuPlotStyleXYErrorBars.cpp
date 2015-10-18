#include <CGnuPlotStyleXYErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotErrorBarData.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleXYErrorBars::
CGnuPlotStyleXYErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::XYERRORBARS)
{
}

void
CGnuPlotStyleXYErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //---

  // calc bar size
  double bs = plot->barsSize();

  double bh = 0, bw = 0;

  if (! renderer->isPseudo()) {
    bw = renderer->pixelWidthToWindowWidth  (4*bs);
    bh = renderer->pixelHeightToWindowHeight(4*bs);
  }

  //---

  typedef std::vector<CGnuPlotErrorBarData> BarDatas;

  BarDatas barDatas;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    // TODO: index map (see CGnuPlotStyleErrorBars)

    double x = reals[0];
    double y = reals[1];

    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    CRGBA lc = stroke.color();

    // x y xlow xhigh ylow yhigh
    if      ((! isCalcColor && reals.size() == 6) || (isCalcColor && reals.size() == 7)) {
      xl = reals[2];
      xh = reals[3];
      yl = reals[4];
      yh = reals[5];

      if (isCalcColor) {
        double z = reals[6];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }
    // x y xdelta ydelta
    else if ((! isCalcColor && reals.size() == 4) || (isCalcColor && reals.size() == 5)) {
      double dx = reals[2];
      double dy = reals[3];

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (isCalcColor) {
        double z = reals[4];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          lc = lineStyle.calcColor(plot, z);
      }
    }

    //---

    CPoint2D p  (x , y);
    CPoint2D pxl(xl, y);
    CPoint2D pxh(xh, y);
    CPoint2D pyl(x, yl);
    CPoint2D pyh(x, yh);

    p   = group->mapLogPoint(plot->xind(), plot->yind(), 1, p  );
    pxl = group->mapLogPoint(plot->xind(), plot->yind(), 1, pxl);
    pxh = group->mapLogPoint(plot->xind(), plot->yind(), 1, pxh);
    pyl = group->mapLogPoint(plot->xind(), plot->yind(), 1, pyl);
    pyh = group->mapLogPoint(plot->xind(), plot->yind(), 1, pyh);

    //---

    CGnuPlotErrorBarData barData;

    barData.setX(p.x);
    barData.setY(p.y);

    barData.setXDirection(true);
    barData.setYDirection(true);

    barData.setXLow (pxl.x);
    barData.setXHigh(pxh.x);
    barData.setYLow (pyl.y);
    barData.setYHigh(pyh.y);

    barData.setBarWidth (bw);
    barData.setBarHeight(bh);
    barData.setPointSize(pointSize);
    barData.setPointType(pointType);
    barData.setLineColor(lc);
    barData.setLineWidth(1);

    barDatas.push_back(barData);
  }

  if (! renderer->isPseudo())
    plot->updateErrorBarCacheSize(barDatas.size());

  int i = 0;

  for (const auto &barData : barDatas) {
    if (! renderer->isPseudo()) {
      CGnuPlotErrorBarObject *bar = plot->errorBarObjects()[i];

      bar->setX(barData.x());
      bar->setY(barData.y());

      bar->setXDirection(true);
      bar->setYDirection(true);

      bar->setXLow (barData.xLow ());
      bar->setXHigh(barData.xHigh());
      bar->setYLow (barData.yLow ());
      bar->setYHigh(barData.yHigh());

      if (! bar->testAndSetUsed()) {
        bar->setBarWidth (barData.barWidth ());
        bar->setBarHeight(barData.barHeight());

        CGnuPlotStrokeP stroke(plot->createStroke());

        stroke->setColor(barData.lineColor());
        stroke->setWidth(barData.lineWidth());

        CGnuPlotMarkP mark(plot->createMark());

        mark->setType(barData.pointType());
        mark->setSize(barData.pointSize());

        bar->setStroke(stroke);
      }
    }
    else {
      barData.draw(renderer);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->errorBarObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleXYErrorBars::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotStroke stroke(plot);

  double                    pointSize = plot->pointSize();
  CGnuPlotTypes::SymbolType pointType = plot->pointType();

  //----

  renderer->drawLine(p1, p2, stroke.width(), stroke.color(), stroke.lineDash());

  double ph = renderer->pixelHeightToWindowHeight(3);

  CPoint2D dy(0, ph);

  renderer->drawLine(p1 - dy, p1 + dy, stroke.width(), stroke.color(), stroke.lineDash());
  renderer->drawLine(p2 - dy, p2 + dy, stroke.width(), stroke.color(), stroke.lineDash());

  CPoint2D pm = (p1 + p2)/2;

  renderer->drawSymbol(pm, pointType, pointSize, stroke.color(),
                       stroke.width(), /*pixelSize*/true);
}

CBBox2D
CGnuPlotStyleXYErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
