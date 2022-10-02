#include <CGnuPlotStyleHierBubblePlot.h>
#include <CGnuPlotHierBubbleStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBubbleObject.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotUtil.h>

CGnuPlotStyleHierBubblePlot::
CGnuPlotStyleHierBubblePlot() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::HIERBUBBLEPLOT)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotHierBubbleStyleValue>("hier_bubble");
}

void
CGnuPlotStyleHierBubblePlot::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotHierBubbleStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotHierBubbleStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createHierBubbleStyleValue(plot);

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotHierBubbleStyleValue>(plot, value);
  }

  //---

  typedef std::map<std::string, int> Ids;

  Ids ids;

  CHierBubblePack *pack { 0 };

  if (! value->isInited()) {
    value->init();

    pack = value->pack();

    for (const auto &point : plot->getPoints2D()) {
      if (point.getNumValues() < 2)
        continue;

      // get names
      std::string name;

      if (! point.getValue(1, name))
        continue;

      std::vector<std::string> names;

      auto p = name.find('/');

      while (p != std::string::npos) {
        std::string lhs = name.substr(0, p);
        std::string rhs = name.substr(p + 1);

        names.push_back(lhs);

        name = rhs;

        p = name.find('/');
      }

      //---

      std::string op;
      double      value2;

      if (name != "...") {
        if (! point.getValue(2, value2))
          continue;
      }
      else {
        if (! point.getValue(2, op))
          continue;
      }

      //---

      // create nodes
      CHierBubblePack::HierNode *hierNode = 0;

      for (const auto &n : names) {
        if (! hierNode) {
          if (! pack->root()) {
            hierNode = pack->createRootNode(n);

            //hierNode->setOpen(false);
          }
          else
            hierNode = pack->root();
        }
        else {
          CHierBubblePack::HierNode *hierNode1 = hierNode->findChild(n);

          if (! hierNode1) {
            hierNode = pack->createHierNode(hierNode, n);

            //hierNode->setOpen(false);
          }
          else
            hierNode = hierNode1;
        }
      }

      if (name != "...") {
        if (! hierNode) {
          hierNode = pack->createRootNode(name);

          //hierNode->setOpen(false);
        }
        else
          (void) pack->createNode(hierNode, name, value2);
      }
      else {
        if (! hierNode)
          hierNode = pack->root();

        if (op == "open") {
          //hierNode->setOpen(true);
        }
      }
    }

    pack->packNodes();

    value->setInited(true);
  }
  else
    pack = value->pack();

  CHierBubblePack::HierNode *root = pack->root();

  if (root)
    drawNodes(plot, renderer, root, 0);
}

void
CGnuPlotStyleHierBubblePlot::
drawNodes(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
          CHierBubblePack::HierNode *hier, int depth)
{
  CGnuPlotHierBubbleStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotHierBubbleStyleValue>(plot);
  if (! value) return;

  //---

  CRGBA c(0.5, 0.5, 0.5, 0.5);

  //QFontMetrics fm(renderer->font());

  //------

  double r = hier->radiusOrig();

  double px1, py1, px2, py2;

  mapPoint(hier->x() - r, hier->y() + r, px1, py1);
  mapPoint(hier->x() + r, hier->y() - r, px2, py2);

  //QPainterPath path;
  //path.addEllipse(QRectF(px1, py1, px2 - px1, py2 - py1));
  //renderer->setPen(border_color);
  //renderer->setBrush(bg_colors[depth]);
  //renderer->drawPath(path);

  CRGBA c1 = CGnuPlotStyleInst->indexColor(value->palette(), 4* depth      + 2);
  CRGBA c2 = CGnuPlotStyleInst->indexColor(value->palette(), 4*(depth + 1) + 1);
  CRGBA c3 = CGnuPlotStyleInst->indexColor(value->palette(), 4*(depth + 1) + 3);

  renderer->fillEllipse(CBBox2D(px1, py1, px2, py2), c1);
  renderer->drawEllipse(CBBox2D(px1, py1, px2, py2), CRGBA(0, 0, 0), 0);

  //------

  const CHierBubblePack::HierNode::Children &children = hier->getChildren();

  for (auto hierNode : children)
    drawNodes(plot, renderer, hierNode, depth + 1);

  //------

  const CHierBubblePack::Nodes &nodes = hier->getNodes();

  uint nn = uint(nodes.size());

  int in = 0;

  for (auto node : nodes) {
    if (! node->placed()) continue;

    double rn = node->radiusOrig();

    double pnx1, pny1, pnx2, pny2;

    mapPoint(node->x() - rn, node->y() + rn, pnx1, pny1);
    mapPoint(node->x() + rn, node->y() - rn, pnx2, pny2);

  //QPainterPath path;
  //path.addEllipse(QRectF(pnx1, pny1, pnx2 - pnx1, pny2 - pny1));
  //renderer->setPen(border_color);
  //renderer->setBrush(fg_color);
  //renderer->drawPath(path);

  //CRGBA c1 = (node->id() > 0 ? CGnuPlotStyleInst->indexColor(value->palette(), node->id()) : c);
  //CRGBA ce = CGnuPlotUtil::map(in, 0, nn - 1, c1, c2);
    CRGBA ce = c2 + (1.0*in)*(c3 - c2)/(nn - 1);

    renderer->fillEllipse(CBBox2D(pnx1, pny1, pnx2, pny2), ce);
    renderer->drawEllipse(CBBox2D(pnx1, pny1, pnx2, pny2), CRGBA(0, 0, 0), 0);

    //---

    //renderer->setPen(QColor(0, 0, 0));

#if 0
    char buffer[32];

    sprintf(buffer, "%d", node->id());

    int tw = fm.horizontalAdvance(buffer);

    renderer->drawText(pnx1 - tw/2, pny1 + fm.descent(), buffer);
#else
    mapPoint(node->x(), node->y(), pnx1, pny1);

#if 0
    int len = node->name().size();

    for (int i = len; i >= 1; --i) {
      std::string name1 = node->name().substr(0, i);

      int tw = fm.horizontalAdvance(name1.c_str());
      if (tw > 2*(pnx2 - pnx1)) continue;

      renderer->drawText(pnx1 - tw/2, pny1 + fm.descent(), name1.c_str());

      break;
    }
#endif

    CRGBA tc = ce.bwContrast();

    renderer->drawHAlignedText(CPoint2D(pnx1, pny1),
                               CGnuPlotTypes::HAlignPos(CHALIGN_TYPE_CENTER, 0),
                               CGnuPlotTypes::VAlignPos(CVALIGN_TYPE_CENTER, 0),
                               node->name(), tc);
#endif

    ++in;
  }
}

void
CGnuPlotStyleHierBubblePlot::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

#if 0
CBBox2D
CGnuPlotStyleHierBubblePlot::
fit(CGnuPlotPlot * /*plot*/)
{
  return CBBox2D(0, 0, 1, 1);
}
#endif

bool
CGnuPlotStyleHierBubblePlot::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData)
{
  CGnuPlotHierBubbleStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotHierBubbleStyleValue>(plot);
  if (! value) return false;

  CHierBubblePack *pack = value->pack();

  double x, y;

  unmapPoint(mouseEvent.window().x, mouseEvent.window().y, x, y);

  CHierBubblePack::Node *node = pack->getNodeAtPoint(x, y);

  if (! node)
    return false;

  tipData.setXStr(node->name());
  tipData.setYStr(CStrUtil::strprintf("%g", node->size()));

  return true;
}

void
CGnuPlotStyleHierBubblePlot::
mapPoint(double x1, double y1, double &x2, double &y2) const
{
  // map 0->1 to 0-1 with margin
  x2 = CGnuPlotUtil::map(x1, 0, 1, marginLeft_  , 1 - marginRight_);
  y2 = CGnuPlotUtil::map(y1, 0, 1, marginBottom_, 1 - marginTop_  );
}

void
CGnuPlotStyleHierBubblePlot::
unmapPoint(double x1, double y1, double &x2, double &y2) const
{
  // map 0->1 with margin to 0-1
  x2 = CGnuPlotUtil::map(x1, marginLeft_  , 1 - marginRight_, 0, 1);
  y2 = CGnuPlotUtil::map(y1, marginBottom_, 1 - marginTop_  , 0, 1);
}
