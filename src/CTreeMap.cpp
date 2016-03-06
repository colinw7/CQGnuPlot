#include <CTreeMap.h>

CTreeMap::
CTreeMap()
{
}

CTreeMap::HierNode *
CTreeMap::
createRootNode(const std::string &name)
{
  assert(! root_);

  root_ = new HierNode(0, name);

  return root_;
}

CTreeMap::HierNode *
CTreeMap::
addHierNode(const std::string &name)
{
  return addHierNode(root_, name);
}

CTreeMap::HierNode *
CTreeMap::
addHierNode(HierNode *parent, const std::string &name)
{
  HierNode *hierNode = new HierNode(parent, name);

  //parent->addNode(hierNode);

  return hierNode;
}

CTreeMap::Node *
CTreeMap::
addNode(const std::string &name, double value)
{
  return addNode(root_, name, value);
}

CTreeMap::Node *
CTreeMap::
addNode(HierNode *parent, const std::string &name, double value)
{
  Node *node = new Node(parent, name, value);

  parent->addNode(node);

  return node;
}

void
CTreeMap::
place(double x, double y, double w, double h)
{
  root_->setPosition(x, y, w, h);
}

CTreeMap::HierNode *
CTreeMap::
getHierNodeAt(double x, double y)
{
  return getHierNodeAt(root_, x, y);
}

CTreeMap::HierNode *
CTreeMap::
getHierNodeAt(HierNode *hier, double x, double y)
{
  if (! hier->contains(x, y))
    return 0;

  const HierNode::Children &children = hier->getChildren();

  for (auto hier1 : children) {
    HierNode *hier2 = getHierNodeAt(hier1, x, y);

    if (hier2)
      return hier2;
  }

  return hier;
}

CTreeMap::Node *
CTreeMap::
getNodeAt(double x, double y)
{
  return getNodeAt(root_, x, y);
}

CTreeMap::Node *
CTreeMap::
getNodeAt(HierNode *hier, double x, double y)
{
  if (! hier->contains(x, y))
    return 0;

  if (hier->isOpen()) {
    const HierNode::Children &children = hier->getChildren();

    for (auto hier1 : children) {
      Node *node = getNodeAt(hier1, x, y);

      if (node)
        return node;
    }

    const Nodes &nodes = hier->getNodes();

    for (auto node : nodes)
      if (node->contains(x, y))
        return node;
  }

  return hier;
}

void
CTreeMap::
draw(CTreeMapPainter *painter)
{
  if (root_->isOpen())
    drawNodes(painter, root_, 0);
  else
    painter->drawHierNode(root_, 0);
}

void
CTreeMap::
drawNodes(CTreeMapPainter *painter, HierNode *hier, int depth)
{
  if (! hier->isOpen())
    return;

  const HierNode::Children &children = hier->getChildren();

  for (auto hierNode : children)
    drawNodes(painter, hierNode, depth + 1);

  //------

  const Nodes &nodes = hier->getNodes();

  for (auto node : nodes) {
    if (! node->placed()) continue;

    painter->drawNode(node, depth);
  }

  //------

  for (auto hierNode : children)
    painter->drawHierNode(hierNode, depth);
}

CTreeMap::HierNode *
CTreeMap::HierNode::
findChild(const std::string &name) const
{
  for (auto c : children_) {
    if (c->name() == name)
      return c;
  }

  return 0;
}
