#include <CGnuPlotStyleDendrogram.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CDendrogram.h>

class CGnuPlotStyleDendrogramValue : public CGnuPlotPlot::StyleValue {
 public:
  CGnuPlotStyleDendrogramValue() {
    dendrogram_ = new CDendrogram;
  }

 ~CGnuPlotStyleDendrogramValue() {
    delete dendrogram_;
  }

  CDendrogram *dendrogram() const { return dendrogram_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    delete dendrogram_;

    dendrogram_ = new CDendrogram;
  }

 private:
  CDendrogram *dendrogram_ { 0 };
  bool         inited_     { false };
};

//------

CGnuPlotStyleDendrogram::
CGnuPlotStyleDendrogram() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::DENDROGRAM)
{
}

void
CGnuPlotStyleDendrogram::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotStyleDendrogramValue *value =
    dynamic_cast<CGnuPlotStyleDendrogramValue *>(plot->styleValue("dendrogram"));

  if (! value) {
    value = new CGnuPlotStyleDendrogramValue;

    plot->setStyleValue("dendrogram", value);
  }

  //---

  CDendrogram *dendrogram { 0 };

  if (! value->isInited()) {
    value->init();

    dendrogram = value->dendrogram();

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

    value->setInited(true);
  }
  else
    dendrogram = value->dendrogram();

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

#if 0
  {
    double px1, py1, px2, py2;

    mapPoint(node->x()             , node->yc() - root->dy()/2, &px1, &py1);
    mapPoint(node->x() + root->dx(), node->yc() + root->dy()/2, &px2, &py2);

    CBBox2D rect(px1, py1, px2, py2));

    renderer->fillRect(rect, CRGBA(0.9, 0.9, 0.9));
  }
#endif

  double px1, py1;

  mapPoint(node->x(), node->yc(), px1, py1);

  px1 += textMargin_;

  double cs = circleSize_/2.0;

  //path.addEllipse(QRectF(px1 - cs, py1 - cs, circleSize_, circleSize_));
  //renderer->setPen(node_outline);

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

    double x1 = px2 + textMargin_ + cs; double y1 = py2;
    double x4 = px1 - cs              ; double y4 = py1;
    double x2 = x1 + (x4 - x1)/3.0    ; double y2 = y1;
    double x3 = x2 + (x4 - x1)/3.0    ; double y3 = y4;

    //path.moveTo(QPointF(x1, y1));
    //path.cubicTo(QPointF(x2, y2), QPointF(x3, y3), QPointF(x4, y4));

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
  CGnuPlotStyleDendrogramValue *value =
    dynamic_cast<CGnuPlotStyleDendrogramValue *>(plot->styleValue("dendrogram"));
  if (! value) return false;

  CDendrogram *dendrogram = value->dendrogram();

  double x, y;

  unmapPoint(mouseEvent.window().x, mouseEvent.window().y, x, y);

  CDendrogram::Node *node = dendrogram->getNodeAtPoint(x, y, circleSize_);

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
  CGnuPlotStyleDendrogramValue *value =
    dynamic_cast<CGnuPlotStyleDendrogramValue *>(plot->styleValue("dendrogram"));
  if (! value) return;

  CDendrogram *dendrogram = value->dendrogram();

  double x, y;

  unmapPoint(mouseEvent.window().x, mouseEvent.window().y, x, y);

  CDendrogram::Node *node = dendrogram->getNodeAtPoint(x, y, circleSize_);

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
  x2 = CGnuPlotUtil::map(x1, 0, 1, marginLeft_  , 1 - marginRight_);
  y2 = CGnuPlotUtil::map(y1, 0, 1, marginBottom_, 1 - marginTop_  );
}

void
CGnuPlotStyleDendrogram::
unmapPoint(double x1, double y1, double &x2, double &y2) const
{
  // map 0->1 with margin to 0-1
  x2 = CGnuPlotUtil::map(x1, marginLeft_  , 1 - marginRight_, 0, 1);
  y2 = CGnuPlotUtil::map(y1, marginBottom_, 1 - marginTop_  , 0, 1);
}
