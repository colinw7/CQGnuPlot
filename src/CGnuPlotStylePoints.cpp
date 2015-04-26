#include <CGnuPlotStylePoints.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStylePoints::
CGnuPlotStylePoints() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::POINTS)
{
}

void
CGnuPlotStylePoints::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c = lineStyle.calcColor(CRGBA(1,0,0));

  //---

  double size = plot->pointSize();

  uint pointNum = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    CPoint2D p(0, 0);

    uint valueNum = 0;

    if      (reals.size() == 1) {
      p.x = pointNum;
      p.y = reals[valueNum++];
    }
    else if (reals.size() >= 2) {
      p.x = reals[valueNum++];
      p.y = reals[valueNum++];
    }

    plot->group()->mapLogPoint(p);

    double size1 = size;

    if ((! isCalcColor && valueNum < reals.size()) ||
        (  isCalcColor && valueNum < reals.size() - 1))
      size1 = reals[valueNum++];

    CRGBA c1 = c;

    if (isCalcColor && valueNum < reals.size()) {
      double x = reals[valueNum++];

      c1 = lineStyle.color().calcColor(plot, x);
    }

    renderer->drawSymbol(p, plot->pointType(), size1, c1);
  }
}

void
CGnuPlotStylePoints::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  //bool isCalcColor = lineStyle.color().isCalc();

  CRGBA c = lineStyle.calcColor(CRGBA(1,0,0));

  //---

  double size = plot->pointSize();

  uint pointNum = 0;

  for (const auto &ip : plot->getPoints3D()) {
    for (const auto &point : ip.second) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      CPoint3D p(0, 0, 0);

      uint valueNum = 0;

      if      (reals.size() == 2) {
        p.x = reals[valueNum++];
        p.y = reals[valueNum++];
        p.z = pointNum;
      }
      else if (reals.size() >= 3) {
        p.x = reals[valueNum++];
        p.y = reals[valueNum++];
        p.z = reals[valueNum++];
      }

      renderer->drawSymbol(p, plot->pointType(), size, c);
    }
  }
}

void
CGnuPlotStylePoints::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CPoint2D pm = (p1 + p2)/2;

  renderer->drawSymbol(pm, plot->pointType(), plot->pointSize(),
                       lineStyle.calcColor(CRGBA(1,0,0)));
}

CBBox2D
CGnuPlotStylePoints::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
