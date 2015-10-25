#include <CGnuPlotStyleBoxPlot.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotBoxObject.h>

CGnuPlotStyleBoxPlot::
CGnuPlotStyleBoxPlot() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXPLOT)
{
}

void
CGnuPlotStyleBoxPlot::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  // get x value (first value of first set of values)
  // (if 4 or more values then discreete values)
  double x        = 0;
  bool   discrete = false;

  for (const auto &point : plot->getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double x1;

      if (point.getValue(1, x1))
        x = x1;

      if (point.getNumValues() >= 4)
        discrete = true;

      break;
    }
  }

  //----

  // get sorted y values (discrete or single set)
  typedef std::vector<double>       YVals;
  typedef std::map<int,YVals>       IYVals;
  typedef std::map<std::string,int> SOrder;
  typedef std::map<int,YVals>       SYVals;

  IYVals iyv;
  SYVals syv;
  SOrder sorder;

  for (const auto &point : plot->getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double y = 0, y1 = 0;

      if (point.getValue(2, y1))
        y = y1;

      if (discrete && point.getNumValues() >= 4) {
        // add integer or string discrete values
        int         i;
        std::string s;

        if      (point.getValue(4, i))
          iyv[i].push_back(y);
        else if (point.getValue(4, s)) {
          auto po = sorder.find(s);

          if (po == sorder.end())
            po = sorder.insert(po, SOrder::value_type(s, sorder.size()));

          int i = (*po).second;

          syv[i].push_back(y);
        }
      }
      else
        iyv[0].push_back(y);
    }
  }

  //---

  // get ymin
  double ypos = 0;

  if (! renderer->isPseudo()) {
    CBBox3D bbox = group->getPlotBorderBox(1, 1, 1);

    ypos = bbox.getYMin();
  }

  //----

  CGnuPlotFill   fill  (plot);
  CGnuPlotStroke stroke(plot);

  CRGBA fc = fill  .color();
  CRGBA lc = stroke.color();

  double bw = plot->boxWidth().getSpacing(0.1);

  double ww = bw;

  if (plot->bars().size() < 0.0)
    ww = bw;
  else {
    if (! renderer->isPseudo())
      ww = renderer->pixelWidthToWindowWidth(12*plot->bars().size());
  }

  const CGnuPlotBoxPlot &boxPlot = plot->getBoxPlot();

  CGnuPlotTypes::SymbolType outlierSymbol =
    CGnuPlotTypes::SymbolType(boxPlot.pointType().getValue(7));

  double ps = plot->pointSize();

  const CGnuPlotAxis *xaxis = group->getPlotAxis(CGnuPlotTypes::AxisType::X, 1, true);

  //---

  std::vector<int> inds;

  if (boxPlot.sorted()) {
    for (const auto &so : sorder)
      inds.push_back(so.second);
  }
  else {
    for (const auto &yv : syv)
      inds.push_back(yv.first);
  }

  //---

  CGnuPlotBoxObject *tempBoxObj = 0;

  if (! renderer->isPseudo())
    plot->updateBoxCacheSize(iyv.size() + inds.size());
  else
    tempBoxObj = new CGnuPlotBoxObject(plot);

  //---

  // always use index for x (ignore x value) ??
  int    ic = 0;
  double ix = (syv.empty() && iyv.size() == 1 ? x : 1);

  for (const auto &yv : iyv) {
    double ix1 = ix + ic;

    std::string s = xaxis->getValueStr(ix1, ix1);

    CGnuPlotBoxObject *boxObj;

    if (! renderer->isPseudo())
      boxObj = plot->boxObjects()[ic];
    else
      boxObj = tempBoxObj;

    boxObj->setValues(yv.second);
    boxObj->setX     (ix1);
    boxObj->setY     (ypos);

    if (renderer->isPseudo() || ! boxObj->testAndSetUsed()) {
      boxObj->setLineWidth(ww);
      boxObj->setBoxWidth (bw);
      boxObj->setValueStr (s);

      boxObj->outlierMark()->setType(outlierSymbol);
      boxObj->outlierMark()->setSize(ps);

      boxObj->stroke()->setWidth(1);
      boxObj->stroke()->setColor(lc);

      boxObj->fill()->setColor(fc);
    }

    if (renderer->isPseudo())
      boxObj->draw(renderer);

    ++ic;
  }

  //---

  for (const auto &i : inds) {
    double ix1 = ix + ic;

    auto yv = syv[i];

    std::string s;

    for (const auto &so : sorder) {
      if (i == so.second) {
        s = so.first;
        break;
      }
    }

    CGnuPlotBoxObject *boxObj;

    if (! renderer->isPseudo())
      boxObj = plot->boxObjects()[ic];
    else
      boxObj = tempBoxObj;

    boxObj->setValues(yv);
    boxObj->setX     (ix1);
    boxObj->setY     (ypos);

    if (renderer->isPseudo() || ! boxObj->testAndSetUsed()) {
      boxObj->setLineWidth(ww);
      boxObj->setBoxWidth (bw);
      boxObj->setValueStr (s);

      boxObj->outlierMark()->setType(outlierSymbol);
      boxObj->outlierMark()->setSize(ps);

      boxObj->stroke()->setWidth(1);
      boxObj->stroke()->setColor(lc);

      boxObj->fill()->setColor(fc);
    }

    if (renderer->isPseudo())
      boxObj->draw(renderer);

    ++ic;
  }

  if (! renderer->isPseudo()) {
    for (const auto &boxObj : plot->boxObjects())
      boxObj->draw(renderer);
  }

  delete tempBoxObj;
}

#if 0
CBBox2D
CGnuPlotStyleBoxPlot::
fit(CGnuPlotPlot *plot)
{
  CGnuPlotBBoxRenderer brenderer(plot->app()->renderer());

  draw2D(plot, &brenderer);

  CBBox2D bbox = brenderer.bbox();

  bbox.setXMin(0);
  bbox.setXMax(int(bbox.getXMax() + 0.5));

  return bbox;
}
#endif
