#include <CGnuPlotStyleParallelAxes.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotStyleParallelAxes::
CGnuPlotStyleParallelAxes() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::PARALLELAXES)
{
}

void
CGnuPlotStyleParallelAxes::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  typedef std::vector<double>       Reals;
  typedef std::map<int, Reals>      IReals;
  typedef std::pair<double, double> MinMax;
  typedef std::map<int, MinMax>     IMinMax;

  IReals  ireals;
  IMinMax irange;

  // group values by index
  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    int nr = (isCalcColor ? reals.size() - 1 : reals.size());

    for (int i = 0; i < nr; ++i)
      ireals[i].push_back(reals[i]);
  }

  // calc range for index
  for (const auto &ir : ireals) {
    int          i     = ir.first;
    const Reals &reals = ir.second;

    COptReal rmin, rmax;

    for (const auto &r : reals) {
      rmin.updateMin(r);
      rmax.updateMax(r);
    }

    irange[i] = MinMax(rmin.getValue(0), rmax.getValue(0));
  }

  int nr = irange.size();

  //------

  // draw lines between points
  uint np = plot->numPoints2D();

  for (uint i = 0; i < np; ++i) {
    const CGnuPlotPoint &point = plot->getPoint2D(i);

    std::vector<double> reals;

    (void) point.getReals(reals);

    int nr = (isCalcColor ? reals.size() - 1 : reals.size());

    std::vector<double> ireals;
    std::vector<CRGBA>  colors;

    for (int j = 0; j < nr; ++j) {
      const MinMax &mm = irange[j];

      double ymin = mm.first;
      double ymax = mm.second;

      const CGnuPlotAxisData &paxis = group->paxis(j + 1);

      if (paxis.min().isValid()) ymin = paxis.min().getValue();
      if (paxis.max().isValid()) ymax = paxis.max().getValue();

      double ir;

      if (! paxis.isReverse())
        ir = CGnuPlotUtil::map(reals[j], ymin, ymax, 0, 1);
      else
        ir = CGnuPlotUtil::map(reals[j], ymax, ymin, 0, 1);

      ireals.push_back(ir);
    }

    CRGBA c;

    if (isCalcColor)
      c = lineStyle.calcColor(plot, reals[nr]);
    else
      c = lineStyle.calcColor(plot, i);

    for (uint j = 1; j < ireals.size(); ++j)
      renderer->drawClipLine(CPoint2D(j, ireals[j - 1]), CPoint2D(j + 1, ireals[j]), 1.0, c);
  }

  //------

  // draw axes
  CBBox2D clip  = renderer->clip();
  CBBox2D range = renderer->range();

  for (const auto &ir : irange) {
    int           i  = ir.first;
    const MinMax &mm = ir.second;

    double ymin = mm.first;
    double ymax = mm.second;

    const CGnuPlotAxisData &paxis = group->paxis(i + 1);

    if (paxis.min().isValid()) ymin = paxis.min().getValue();
    if (paxis.max().isValid()) ymax = paxis.max().getValue();

    CGnuPlotAxis *yaxis = group->getPlotAxis('p', i + 1);

    if (! paxis.isReverse())
      yaxis->setRange(ymin, ymax);
    else
      yaxis->setRange(ymax, ymin);

    yaxis->setDrawTickMark(paxis.showTics());
    yaxis->setDrawMinorTickMark(false);
    yaxis->setLabelInside(i > nr/2);

    CBBox2D range1(CBBox2D(range.getXMin(), ymin, range.getXMax(), ymax));

    renderer->setRange(range1);
    renderer->setReverse(false, false); // TODO

    renderer->setClip(CBBox2D(clip.getXMin(), std::max(ymin, mm.first),
                              clip.getXMax(), std::min(ymax, mm.second)));

    yaxis->drawAxis(renderer, i + 1);
  }

  renderer->setRange(range);
  renderer->setClip(clip);
}

void
CGnuPlotStyleParallelAxes::
drawAxes(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  plot->group()->drawXAxis(renderer, 1, false);
}

CBBox2D
CGnuPlotStyleParallelAxes::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
