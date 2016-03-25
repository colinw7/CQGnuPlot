#include <CGnuPlotStyleBivariate.h>
#include <CGnuPlotBivariateStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPointObject.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleBivariate::
CGnuPlotStyleBivariate() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BIVARIATE)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotBivariateStyleValue>("bivariate");
}

void
CGnuPlotStyleBivariate::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotBivariateStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotBivariateStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createBivariateStyleValue(plot);

    //value->init(plot->bivariateStyleValue());

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotBivariateStyleValue>(plot, value);
  }

  //---

  CGnuPlotStroke stroke(plot);

  //------

  typedef std::vector<CGnuPlotPointData> PointDatas;

  PointDatas pointDatas;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() != 3)
      continue;

    double x  = reals[0];
    double y1 = reals[1];
    double y2 = reals[2];

    CRGBA c1 = stroke.color();

    CGnuPlotPointData data;

    data.setPoint    (CPoint2D(x, y1));
    data.setSize     (y2 - y1);
    data.setPointType(CGnuPlotTypes::SymbolType::BIVARIATE);
    data.setLineWidth(stroke.width());
    data.setColor    (c1);

    pointDatas.push_back(data);
  }

  //----

  if (! renderer->isPseudo())
    plot->updatePointCacheSize(pointDatas.size());

  int pointNum = 0;

  for (const auto &data : pointDatas) {
    if (! renderer->isPseudo()) {
      CGnuPlotPointObject *point = plot->pointObjects()[pointNum];

      if (! point->testAndSetUsed()) {
        point->setData(data);
      }
      else {
        point->setPoint(data.point());
      }
    }
    else {
      renderer->drawPoint(data.point(), data.color());
      renderer->drawPoint(data.point() + CPoint2D(0, data.size().getValue(0)), data.color());
    }

    ++pointNum;
  }

  if (! renderer->isPseudo()) {
    for (const auto &point : plot->pointObjects())
      point->draw(renderer);
  }
}
