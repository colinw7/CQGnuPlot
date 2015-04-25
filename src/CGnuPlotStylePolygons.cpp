#include <CGnuPlotStylePolygons.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStylePolygons::
CGnuPlotStylePolygons() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::POLYGONS)
{
}

void
CGnuPlotStylePolygons::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CRGBA fc(0.5,0.5,0.5,0.5);
  CRGBA lc(0,0,0);

  for (const auto &point : plot->getPoints2D()) {
    std::string str;

    if (! point.getValue(1, str))
      continue;

    std::vector<std::string> words;

    CStrUtil::addWords(str, words);

    std::vector<double> values;

    for (const auto &w : words) {
      double r;

      if (CStrUtil::toReal(w, &r))
        values.push_back(r);
    }

    std::vector<CPoint2D> points;

    for (int i = 0; i < int(values.size())/2; ++i) {
      double x = values[i*2 + 0];
      double y = values[i*2 + 1];

      points.push_back(CPoint2D(x, y));
    }

    renderer->fillPolygon(points, fc);
    renderer->drawPolygon(points, 1, lc);
  }
}

CBBox2D
CGnuPlotStylePolygons::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
