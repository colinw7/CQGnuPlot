#include <CGnuPlotStyleAdjacency.h>
#include <CGnuPlotAdjacencyStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotStyleAdjacencyRenderer.h>

CGnuPlotStyleAdjacency::
CGnuPlotStyleAdjacency() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ADJACENCY)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotAdjacencyStyleValue>("adjacency");
}

void
CGnuPlotStyleAdjacency::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotAdjacencyStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotAdjacencyStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createAdjacencyStyleValue(plot);

    value->initRenderer(renderer);

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotAdjacencyStyleValue>(plot, value);
  }

  //---

  plot->group()->setMargin(CGnuPlotMargin(0, 0, 0, 0));

  CAdjacency *adjacency = value->adjacency();

  if (! value->isInited()) {
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

        int value3 = 0;

        if (! point.getValue(3, value3))
          continue;

        adjacency->connectNodes(source, target, value3);
      }
    }

    adjacency->sortNodes();

    value->setInited(true);
  }

  CGnuPlotStyleAdjacencyRenderer *arenderer = value->renderer();

  arenderer->setRenderer(renderer);
  arenderer->setPalette (value->palette());

  arenderer->clearRects();

  adjacency->draw(arenderer);
}

bool
CGnuPlotStyleAdjacency::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData)
{
  CGnuPlotAdjacencyStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotAdjacencyStyleValue>(plot);
  if (! value) return false;

  CGnuPlotStyleAdjacencyRenderer *arenderer = value->renderer();

  int         ivalue;
  std::string name1, name2;
  CRGBA       c;

  if (! arenderer->getValueAtPos(mouseEvent.window(), ivalue, name1, name2, c))
    return false;

  if (ivalue <= 0)
    return false;

  tipData.setBorderColor(c);

  if (name1 != name2) {
    tipData.setXStr(name1);
    tipData.setYStr(name2 + " = " + CStrUtil::strprintf("%d", ivalue));
  }
  else {
    tipData.setXStr(name1);
    tipData.setYStr("");
  }

  return true;
}

void
CGnuPlotStyleAdjacency::
mousePress(CGnuPlotPlot *, const CGnuPlotMouseEvent &)
{
}

CBBox2D
CGnuPlotStyleAdjacency::
fit(CGnuPlotPlot *)
{
  return CBBox2D(0, 0, 1, 1);
}
