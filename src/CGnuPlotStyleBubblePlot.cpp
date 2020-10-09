#include <CGnuPlotStyleBubblePlot.h>
#include <CGnuPlotBubbleStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleBubblePlot::
CGnuPlotStyleBubblePlot() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BUBBLEPLOT)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotBubbleStyleValue>("bubble");
}

void
CGnuPlotStyleBubblePlot::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotBubbleStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotBubbleStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createBubbleStyleValue(plot);

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotBubbleStyleValue>(plot, value);
  }

  //---

  typedef std::map<std::string, int> Ids;

  Ids ids;

  CBubblePack *pack = value->pack();

  if (! value->isInited()) {
    for (const auto &point : plot->getPoints2D()) {
      std::string name;
      double      value2;

      if (! point.getValue(1, name) || ! point.getValue(2, value2))
        continue;

      int id = -1;

      std::string idName;

      if (point.getValue(3, idName)) {
        auto p = ids.find(idName);

        if (p == ids.end()) {
          id = ids.size() + 1;

          ids[idName] = id;
        }
        else
          id = (*p).second;
      }

      pack->addNode(name, value2, id);
    }

    value->setInited(true);
  }

  //---

  CRGBA c(0.5, 0.5, 0.5, 0.5);

  double xmin, ymin, xmax, ymax;

  pack->boundingBox(xmin, ymin, xmax, ymax);

  double xc = (xmin + xmax)/2;
  double yc = (ymin + ymax)/2;
  double xs = xmax - xmin;
  double ys = ymax - ymin;

  double s = std::max(xs, ys);

  xmin = xc - s/2;
  ymin = yc - s/2;
  xmax = xc + s/2;
  ymax = yc + s/2;

  if (! renderer->isPseudo())
    plot->updateBubbleCacheSize(pack->nodes().size());

  int i = 0;

  for (const auto &n : pack->nodes()) {
    double x1 = CGnuPlotUtil::map(n->x() - n->radius(), xmin, xmax, -1, 1);
    double y1 = CGnuPlotUtil::map(n->y() - n->radius(), ymin, ymax, -1, 1);
    double x2 = CGnuPlotUtil::map(n->x() + n->radius(), xmin, xmax, -1, 1);
    double y2 = CGnuPlotUtil::map(n->y() + n->radius(), ymin, ymax, -1, 1);

    CPoint2D pc((x1 + x2)/2, (y1 + y2)/2);

    if (! renderer->isPseudo()) {
      CGnuPlotBubbleObject *bubble = plot->bubbleObjects()[i];

      bubble->setCenter(pc);
      bubble->setXRadius((x2 - x1)/2);
      bubble->setYRadius((y2 - y1)/2);
      bubble->setName(n->name());
      bubble->setValue(n->value());

      CRGBA c1 = (n->id() > 0 ? CGnuPlotStyleInst->indexColor(value->palette(), n->id()) : c);

      if (! bubble->testAndSetUsed()) {
        bubble->fill()->setType (CGnuPlotTypes::FillType::SOLID);
        bubble->fill()->setColor(c1);

        bubble->stroke()->setEnabled(false);
      }
    }
    else
      renderer->drawEllipse(pc, (x2 - x1)/2, (y2 - y1)/2, 0, c, 1);

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bubble : plot->bubbleObjects())
      bubble->draw(renderer);
  }
}

void
CGnuPlotStyleBubblePlot::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

CBBox2D
CGnuPlotStyleBubblePlot::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
