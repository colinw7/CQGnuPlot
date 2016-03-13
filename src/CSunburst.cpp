#include <CSunburst.h>
#include <CBBox2D.h>

CSunburst::
CSunburst()
{
}

CSunburst::
~CSunburst()
{
  reset();
}

void
CSunburst::
reset()
{
  for (auto &r : roots_)
    delete r;

  roots_.clear();
}

void
CSunburst::
pack()
{
  if       (roots_.size() == 1)
    roots_[0]->packNodes(innerRadius(), 1.0, 0.0, startAngle(), 360);
  else  if (roots_.size() == 2) {
    double dr = 0.9/5;

    roots_[0]->packNodes(0.1, 1.0, dr, startAngle()      , 180);
    roots_[1]->packNodes(0.1, 1.0, dr, startAngle() + 180, 180);
  }
}

void
CSunburst::
addRoot()
{
  RootNode *root = new RootNode;

  roots_.push_back(root);
}

CSunburst::HierNode *
CSunburst::
addHierNode(const std::string &name)
{
  return addHierNode(roots_[0], name);
}

CSunburst::HierNode *
CSunburst::
addHierNode(HierNode *parent, const std::string &name)
{
  HierNode *hierNode = new HierNode(parent, name);

  //parent->addNode(hierNode);

  return hierNode;
}

CSunburst::Node *
CSunburst::
addNode(const std::string &name, double value)
{
  return addNode(roots_[0], name, value);
}

CSunburst::Node *
CSunburst::
addNode(HierNode *parent, const std::string &name, double value)
{
  Node *node = new Node(parent, name, value);

  parent->addNode(node);

  return node;
}

CSunburst::Node *
CSunburst::
getNodeAt(HierNode *hier, double x, double y) const
{
  const Nodes &nodes = hier->getNodes();

  for (auto node : nodes)
    if (node->pointInside(x, y))
      return node;

  //------

  const HierNode::Children &children = hier->getChildren();

  for (auto hierNode : children) {
    if (hierNode->pointInside(x, y))
      return hierNode;

    Node *node = getNodeAt(hierNode, x, y);

    if (node)
      return node;
  }

  return 0;
}

//---

void
CSunburst::
draw(CSunburstPainter *painter)
{
  for (const auto &r : roots_)
    drawNodes(painter, r);
}

void
CSunburst::
drawNodes(CSunburstPainter *painter, HierNode *hier)
{
  const Nodes &nodes = hier->getNodes();

  for (auto node : nodes)
    drawNode(painter, node);

  //------

  const HierNode::Children &children = hier->getChildren();

  for (auto hierNode : children) {
    drawNode(painter, hierNode);

    drawNodes(painter, hierNode);
  }
}

void
CSunburst::
drawNode(CSunburstPainter *painter, Node *node)
{
  if (! node->placed()) return;

  painter->drawNode(node);
}

//---

void
CSunburst::
visit(CSunburstVisitor *visitor)
{
  for (const auto &r : roots_)
    visitNodes(visitor, r);
}

void
CSunburst::
visitNodes(CSunburstVisitor *visitor, HierNode *hier)
{
  const Nodes &nodes = hier->getNodes();

  for (auto node : nodes)
    visitNode(visitor, node);

  //------

  const HierNode::Children &children = hier->getChildren();

  for (auto hierNode : children) {
    visitNode(visitor, hierNode);

    visitNodes(visitor, hierNode);
  }
}

void
CSunburst::
visitNode(CSunburstVisitor *visitor, Node *node)
{
  if (! node->placed()) return;

  visitor->visitNode(node);
}
