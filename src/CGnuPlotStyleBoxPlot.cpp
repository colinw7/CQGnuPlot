#include <CGnuPlotStyleBoxPlot.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotTypes.h>
#include <CBoxWhisker.h>

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

  const CGnuPlotAxis *xaxis = group->getPlotAxis(CGnuPlotTypes::AxisType::X, 1, true);

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  CRGBA fc = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));
  CRGBA lc = CRGBA(0,0,0);

  if (fillStyle.isTransparent())
    fc.setAlpha(fillStyle.density());
  else
    fc = fillStyle.density()*fc + group->backgroundColor()*(1 - fillStyle.density());

  double bw = plot->boxWidth().getSpacing(0.1);

  double ww = bw;

  if (plot->bars().size() < 0.0)
    ww = bw;
  else {
    if (! renderer->isPseudo())
      ww = renderer->pixelWidthToWindowWidth(12*plot->bars().size());
  }

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

  const CGnuPlotBoxPlot &boxPlot = plot->getBoxPlot();

  CGnuPlotTypes::SymbolType outlierSymbol =
    CGnuPlotTypes::SymbolType(boxPlot.pointType().getValue(7));

  double ps = plot->pointSize();

  //----

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

  double ypos = 0;

  if (! renderer->isPseudo()) {
    CBBox3D bbox = group->getPlotBorderBox(1, 1, 1);

    ypos = bbox.getYMin();
  }

  //---

  // always use index for x (ignore x value) ??
  double ix = (syv.empty() && iyv.size() == 1 ? x : 1);

  for (const auto &yv : iyv) {
    CBoxWhisker whisker(yv.second);

    double x11 = ix - ww/2;
    double x21 = ix + ww/2;

    renderer->drawClipLine(CPoint2D(ix, whisker.min()), CPoint2D(ix, whisker.max()), 1, lc);

    renderer->drawClipLine(CPoint2D(x11, whisker.min()), CPoint2D(x21, whisker.min()), 1, lc);
    renderer->drawClipLine(CPoint2D(x11, whisker.max()), CPoint2D(x21, whisker.max()), 1, lc);

    double x12 = ix - bw/2;
    double x22 = ix + bw/2;

    CBBox2D bbox(x12, whisker.lower(), x22, whisker.upper());

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc, 1);

    renderer->drawClipLine(CPoint2D(x12, whisker.median()), CPoint2D(x22, whisker.median()), 1, lc);

    if (boxPlot.outliers()) {
      for (const auto &o : whisker.outliers())
        renderer->drawSymbol(CPoint2D(ix, whisker.value(o)), outlierSymbol, ps, lc);
    }

    if (renderer->isPseudo()) {
      renderer->drawSymbol(CPoint2D(ix - 1, 0), outlierSymbol, ps, lc);
      renderer->drawSymbol(CPoint2D(ix + 1, 0), outlierSymbol, ps, lc);
    }

    std::string s = xaxis->getValueStr(ix, ix);

    renderer->drawHAlignedText(CPoint2D(ix, ypos), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, -8, s, CRGBA(0,0,0));

    ++ix;
  }

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

  for (const auto &i : inds) {
    auto yv = syv[i];

    std::string s;

    for (const auto &so : sorder) {
      if (i == so.second) {
        s = so.first;
        break;
      }
    }

    CBoxWhisker whisker(yv);

    double x11 = ix - ww/2;
    double x21 = ix + ww/2;

    renderer->drawClipLine(CPoint2D(ix, whisker.min()), CPoint2D(ix, whisker.max()), 1, lc);

    renderer->drawClipLine(CPoint2D(x11, whisker.min()), CPoint2D(x21, whisker.min()), 1, lc);
    renderer->drawClipLine(CPoint2D(x11, whisker.max()), CPoint2D(x21, whisker.max()), 1, lc);

    double x12 = ix - bw/2;
    double x22 = ix + bw/2;

    CBBox2D bbox(x12, whisker.lower(), x22, whisker.upper());

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc, 1);

    renderer->drawClipLine(CPoint2D(x12, whisker.median()), CPoint2D(x22, whisker.median()), 1, lc);

    if (boxPlot.outliers()) {
      for (const auto &o : whisker.outliers())
        renderer->drawSymbol(CPoint2D(ix, whisker.value(o)), outlierSymbol, ps, lc);
    }

    if (renderer->isPseudo()) {
      renderer->drawSymbol(CPoint2D(ix - 1, 0), outlierSymbol, ps, lc);
      renderer->drawSymbol(CPoint2D(ix + 1, 0), outlierSymbol, ps, lc);
    }

    renderer->drawHAlignedText(CPoint2D(ix, ypos), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, 8, s, CRGBA(0,0,0));

    ++ix;
  }
}

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
