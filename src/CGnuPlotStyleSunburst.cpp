#include <CGnuPlotStyleSunburst.h>
#include <CGnuPlotStyleSunburstPainter.h>
#include <CGnuPlotSunburstStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotDevice.h>
#include <CFontMgr.h>
#include <CGnuPlotPieObject.h>

class CGnuPlotStyleSunburstVisitor : public CSunburstVisitor {
 public:
  CGnuPlotStyleSunburstVisitor() { }

  int numNodes() const { return numNodes_; }

  void visitNode(CSunburst::Node *) {
    ++numNodes_;
  }

 private:
  int numNodes_ { 0 };
};

//------

CGnuPlotStyleSunburst::
CGnuPlotStyleSunburst() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::SUNBURST)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotSunburstStyleValue>("sunburst");
}

void
CGnuPlotStyleSunburst::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotSunburstStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotSunburstStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createSunburstStyleValue(plot);

    value->init();

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotSunburstStyleValue>(plot, value);
  }

  //---

  if (renderer->isPseudo())
    return;

  //---

  renderer->setFont(value->font());

  CSunburst *sunburst = value->sunburst();

  sunburst->setInnerRadius(value->innerRadius());
  sunburst->setStartAngle (value->startAngle ());

  if (! value->isInited()) {
    typedef std::map<std::string, CSunburst::HierNode *> GroupMap;

    sunburst->reset();

    GroupMap groups;

    sunburst->addRoot();

    int nr = 0;

    for (const auto &p : plot->getPoints2D()) {
      int nv = p.getNumValues();

      if (nv < 2) continue;

      std::string key;

      CSunburst::HierNode *parentNode = 0;

      for (int i = 0; i < nv - 2; ++i) {
        std::string group;

        if (! p.getValue(i + 1, group))
          continue;

        if (key != "")
          key += "#";

        key += group;

        auto p = groups.find(key);

        CSunburst::HierNode *hierNode = 0;

        if (p == groups.end()) {
          if (parentNode) {
            hierNode = sunburst->addHierNode(parentNode, group);

            hierNode->setColorId(-1);
          }
          else {
            hierNode = sunburst->addHierNode(group);

            ++nr;

            hierNode->setColorId(nr);
          }

          p = groups.insert(p, GroupMap::value_type(key, hierNode));
        }

        parentNode = (*p).second;
      }

      std::string name;

      if (! p.getValue(nv - 1, name))
        continue;

      double value;

      if (! p.getValue(nv, value))
        continue;

      std::string group;

      CSunburst::Node *node = 0;

      if (parentNode)
        node = sunburst->addNode(parentNode, name, value);
      else
        node = sunburst->addNode(name, value);

      node->setColorId(-1);
    }

    sunburst->pack();

    value->setInited(true);
  }

  //---

  if (! renderer->isPseudo()) {
    CGnuPlotStyleSunburstVisitor visitor;

    sunburst->visit(&visitor);

    plot->updatePieCacheSize(visitor.numNodes());
  }

  CGnuPlotStyleSunburstPainter painter(plot, renderer);

  painter.setBorderColor(value->borderColor());
  painter.setPalette    (value->palette());

  sunburst->draw(&painter);

  if (! renderer->isPseudo()) {
    for (const auto &pie : plot->pieObjects())
      pie->draw(renderer);
  }
}

void
CGnuPlotStyleSunburst::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

CBBox2D
CGnuPlotStyleSunburst::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
