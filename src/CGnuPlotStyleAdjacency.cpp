#include <CGnuPlotStyleAdjacency.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotStyleAdjacencyRenderer.h>

CGnuPlotStyleAdjacency::
CGnuPlotStyleAdjacency() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ADJACENCY)
{
}

void
CGnuPlotStyleAdjacency::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  plot->group()->setMargin(CGnuPlotMargin(0, 0, 0, 0));

  CAdjacency *adjacency = plot->adjacencyData().adjacency();

  if (! adjacency) {
    adjacency = new CAdjacency;

    int state = 0;

    for (const auto &point : plot->getPoints2D()) {
      if (point.isDiscontinuity())
        ++state;

      // node id, name and group
      if      (state == 0) {
        if (point.getNumValues() != 3)
          continue;

        int id = 0;

        if (! point.getValue(1, id))
          continue;

        std::string name;

        if (! point.getValue(2, name))
          continue;

        int group;

        if (! point.getValue(3, group))
          continue;

        adjacency->addNode(id, name, group);
      }
      // source, target, value
      else if (state == 1) {
        int source = 0;

        if (! point.getValue(1, source))
          continue;

        int target = 0;

        if (! point.getValue(2, target))
          continue;

        int value = 0;

        if (! point.getValue(3, value))
          continue;

        adjacency->connectNodes(source, target, value);
      }
    }

    adjacency->sortNodes();

    plot->setAdjacency(adjacency);
  }

  CGnuPlotStyleAdjacencyRenderer *arenderer = plot->adjacencyData().renderer();

  if (! arenderer) {
    arenderer = new CGnuPlotStyleAdjacencyRenderer(renderer, adjacency);

    plot->setAdjacencyRenderer(arenderer);
  }

  arenderer->setRenderer(renderer);

  arenderer->clearRects();

  adjacency->draw(arenderer);
}

bool
CGnuPlotStyleAdjacency::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotTypes::InsideData &insideData, CGnuPlotTipData &tipData)
{
  CGnuPlotStyleAdjacencyRenderer *arenderer = plot->adjacencyData().renderer();

  int         value;
  std::string name1, name2;
  CRGBA       c;

  if (! arenderer->getValueAtPos(insideData.window, value, name1, name2, c))
    return false;

  if (value <= 0)
    return false;

  tipData.setBorderColor(c);

  if (name1 != name2) {
    tipData.setXStr(name1);
    tipData.setYStr(name2 + " = " + CStrUtil::strprintf("%d", value));
  }
  else {
    tipData.setXStr(name1);
    tipData.setYStr("");
  }

  return true;
}

void
CGnuPlotStyleAdjacency::
mousePress(CGnuPlotPlot *, const CGnuPlotTypes::InsideData &)
{
}

CBBox2D
CGnuPlotStyleAdjacency::
fit(CGnuPlotPlot *)
{
  return CBBox2D(0, 0, 1, 1);
}
