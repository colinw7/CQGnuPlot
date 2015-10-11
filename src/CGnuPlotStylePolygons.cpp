#include <CGnuPlotStylePolygons.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotPolygonObject.h>
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
  typedef std::vector<CPoint2D>    Points;
  typedef std::vector<Points>      PointsArray;
  typedef std::vector<std::string> Names;

  PointsArray pointsArray;
  Names       names;

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

    Points points;

    for (int i = 0; i < int(values.size())/2; ++i) {
      double x = values[i*2 + 0];
      double y = values[i*2 + 1];

      points.push_back(CPoint2D(x, y));
    }

    pointsArray.push_back(points);

    if (! point.getValue(2, str))
      str = "";

    names.push_back(str);
  }

  if (! renderer->isPseudo() && plot->isCacheActive()) {
    plot->updatePolygonCacheSize(pointsArray.size());

    int i = 0;

    for (const auto &polygon : plot->polygonObjects()) {
      const Points      &points = pointsArray[i];
      const std::string &name   = names[i];

      polygon->setPoints(points);
      polygon->setText(name);

      if (! polygon->testAndSetUsed()) {
        CGnuPlotFillP   fill  (polygon->fill  ()->dup());
        CGnuPlotStrokeP stroke(polygon->stroke()->dup());

        polygon->setFill  (fill  );
        polygon->setStroke(stroke);
      }

      ++i;
    }

    for (const auto &polygon : plot->polygonObjects())
      polygon->draw(renderer);
  }
  else {
    CGnuPlotFill   fill  (plot);
    CGnuPlotStroke stroke(plot);

    for (const auto &points : pointsArray) {
      renderer->fillPolygon(points, fill.color());

      if (stroke.isEnabled())
        renderer->drawPolygon(points, stroke.width(), stroke.color(), stroke.lineDash());
    }
  }
}

CBBox2D
CGnuPlotStylePolygons::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
