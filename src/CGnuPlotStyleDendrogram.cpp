#include <CGnuPlotStyleDendrogram.h>
#include <CGnuPlotDendrogramStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleDendrogram::
CGnuPlotStyleDendrogram() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::DENDROGRAM)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotDendrogramStyleValue>("dendrogram");
}

void
CGnuPlotStyleDendrogram::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  value_ = CGnuPlotStyleValueMgrInst->getValue<CGnuPlotDendrogramStyleValue>(plot);

  if (! value_) {
    value_ = plot->app()->device()->createDendrogramStyleValue(plot);

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotDendrogramStyleValue>(plot, value_);
  }

  //---

  CDendrogram *dendrogram { 0 };

  if (! value_->isInited()) {
    value_->init();

    dendrogram = value_->dendrogram();

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
      double      value;

      if (name != "...") {
        if (! point.getValue(2, value))
          continue;
      }
      else {
        if (! point.getValue(2, op))
          continue;
      }

      //---

      // create nodes
      CDendrogram::HierNode *hierNode = 0;

      for (const auto &n : names) {
        if (! hierNode) {
          if (! dendrogram->root()) {
            hierNode = dendrogram->createRootNode(n);

            hierNode->setOpen(false);
          }
          else
            hierNode = dendrogram->root();
        }
        else {
          CDendrogram::HierNode *hierNode1 = hierNode->findChild(n);

          if (! hierNode1) {
            hierNode = dendrogram->createHierNode(hierNode, n);

            hierNode->setOpen(false);
          }
          else
            hierNode = hierNode1;
        }
      }

      if (name != "...") {
        if (! hierNode) {
          hierNode = dendrogram->createRootNode(name);

          hierNode->setOpen(false);
        }
        else
          (void) dendrogram->createNode(hierNode, name, value);
      }
      else {
        if (! hierNode)
          hierNode = dendrogram->root();

        if (op == "open")
          hierNode->setOpen(true);
      }
    }

    dendrogram->placeNodes();

    value_->setInited(true);
  }
  else
    dendrogram = value_->dendrogram();

  CDendrogram::HierNode *root = dendrogram->root();

  if (root) {
    drawNode(renderer, 0, root);

    drawNodes(renderer, root, 0);
  }
}

void
CGnuPlotStyleDendrogram::
drawNodes(CGnuPlotRenderer *renderer, CDendrogram::HierNode *hier, int depth)
{
  const CDendrogram::HierNode::Children &children = hier->getChildren();

  for (auto hierNode : children) {
    drawNode(renderer, hier, hierNode);

    drawNodes(renderer, hierNode, depth + 1);
  }

  //------

  const CDendrogram::Nodes &nodes = hier->getNodes();

  for (auto node : nodes)
    drawNode(renderer, hier, node);
}

void
CGnuPlotStyleDendrogram::
drawNode(CGnuPlotRenderer *renderer, CDendrogram::HierNode *hier, CDendrogram::Node *node)
{
  if (! node->isPlaced()) return;

  bool is_hier = dynamic_cast<CDendrogram::HierNode *>(node);

  double px1, py1;

  mapPoint(node->x(), node->yc(), px1, py1);

  double cs = value_->circleSize()/2.0;
  double tm = value_->textMargin();

  px1 += tm;

  CRGBA ellipseColor = CRGBA::fromRGBAI(158,202,225);

  renderer->drawEllipse(CPoint2D(px1, py1), cs, cs, 0, ellipseColor, 1);

  if (node->hasChildren() && ! node->isOpen())
    renderer->fillEllipse(CPoint2D(px1, py1), cs, cs, 0, ellipseColor);

  const std::string &name1 = node->name();

  if (is_hier)
    renderer->drawHAlignedText(CPoint2D(px1 - 2*cs, py1),
                               CGnuPlotTypes::HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                               CGnuPlotTypes::VAlignPos(CVALIGN_TYPE_CENTER, 0),
                               name1, CRGBA(0,0,0));
  else
    renderer->drawHAlignedText(CPoint2D(px1 + 2*cs, py1),
                               CGnuPlotTypes::HAlignPos(CHALIGN_TYPE_LEFT, 0),
                               CGnuPlotTypes::VAlignPos(CVALIGN_TYPE_CENTER, 0),
                               name1, CRGBA(0,0,0));

  if (hier) {
    double px2, py2;

    mapPoint(hier->x(), hier->yc(), px2, py2);

    double x1 = px2 + tm + cs     ; double y1 = py2;
    double x4 = px1 - cs          ; double y4 = py1;
    double x2 = x1 + (x4 - x1)/3.0; double y2 = y1;
    double x3 = x2 + (x4 - x1)/3.0; double y3 = y4;

    renderer->drawBezier(CPoint2D(x1, y1), CPoint2D(x2, y2),
                         CPoint2D(x3, y3), CPoint2D(x4, y4),
                         CRGBA(0.5,0.5,0.5), 1);
  }
}

void
CGnuPlotStyleDendrogram::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

CBBox2D
CGnuPlotStyleDendrogram::
fit(CGnuPlotPlot* /*plot*/)
{
  return CBBox2D(0, 0, 1, 1);
}

bool
CGnuPlotStyleDendrogram::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData)
{
  value_ = dynamic_cast<CGnuPlotDendrogramStyleValue *>(plot->styleValue("dendrogram"));
  if (! value_) return false;

  CDendrogram *dendrogram = value_->dendrogram();

  double x, y;

  unmapPoint(mouseEvent.window().x, mouseEvent.window().y, x, y);

  CDendrogram::Node *node = dendrogram->getNodeAtPoint(x, y, value_->circleSize());

  if (! node)
    return false;

  tipData.setXStr(node->name());
  tipData.setYStr(CStrUtil::strprintf("%g", node->size()));

  return true;
}

void
CGnuPlotStyleDendrogram::
mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent)
{
  value_ = dynamic_cast<CGnuPlotDendrogramStyleValue *>(plot->styleValue("dendrogram"));
  if (! value_) return;

  CDendrogram *dendrogram = value_->dendrogram();

  double x, y;

  unmapPoint(mouseEvent.window().x, mouseEvent.window().y, x, y);

  CDendrogram::Node *node = dendrogram->getNodeAtPoint(x, y, value_->circleSize());

  if (! node)
    return;

  node->setOpen(! node->isOpen());

  dendrogram->placeNodes();
}

void
CGnuPlotStyleDendrogram::
mapPoint(double x1, double y1, double &x2, double &y2) const
{
  // map 0->1 to 0-1 with margin
  x2 = CGnuPlotUtil::map(x1, 0, 1, value_->marginLeft  (), 1 - value_->marginRight());
  y2 = CGnuPlotUtil::map(y1, 0, 1, value_->marginBottom(), 1 - value_->marginTop  ());
}

void
CGnuPlotStyleDendrogram::
unmapPoint(double x1, double y1, double &x2, double &y2) const
{
  // map 0->1 with margin to 0-1
  x2 = CGnuPlotUtil::map(x1, value_->marginLeft  (), 1 - value_->marginRight(), 0, 1);
  y2 = CGnuPlotUtil::map(y1, value_->marginBottom(), 1 - value_->marginTop  (), 0, 1);
}
