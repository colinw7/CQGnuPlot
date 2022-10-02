#include <CGnuPlotStyleStackedArea.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleStackedArea::
CGnuPlotStyleStackedArea() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::STACKEDAREA)
{
}

void
CGnuPlotStyleStackedArea::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  typedef std::vector<CGnuPlot::Points2D> Polygons;

  Polygons polygons;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 2)
      continue;

    double x = reals[0];

    int nr = int(reals.size() - 1);

    while (int(polygons.size()) < nr)
      polygons.push_back(CGnuPlot::Points2D());

    double y = 0;

    for (int i = 0; i < nr; ++i) {
      CGnuPlot::Points2D &poly = polygons[i];

      if (poly.empty())
        poly.push_back(CPoint2D(x, 0));

      y += reals[i + 1];

      poly.push_back(CPoint2D(x, y));
    }
  }

  int np = int(polygons.size());

  for (int i = 0; i < np; ++i) {
    CGnuPlot::Points2D &poly = polygons[i];

    double x = poly.back().x;

    poly.push_back(CPoint2D(x, 0));
  }

  for (int i = 0; i < np; ++i) {
    CGnuPlot::Points2D &poly = polygons[np - i - 1];

    CRGBA c = CGnuPlotStyleInst->indexColor("subtle", i + 1);

    renderer->fillPolygon(poly, c);
  }
}
