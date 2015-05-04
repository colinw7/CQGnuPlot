#include <CAdjacency.h>
#include <algorithm>

// TODO: set group colors
static CRGBA colors[] = {
  CRGBA(0x88/255.0,0x88/255.0,0x88/255.0),
  CRGBA(0xFF/255.0,0x7F/255.0,0x0E/255.0),
  CRGBA(0x2C/255.0,0xA0/255.0,0x2C/255.0),
  CRGBA(0xD6/255.0,0x27/255.0,0x28/255.0),
  CRGBA(0x94/255.0,0x67/255.0,0xBD/255.0),
  CRGBA(0x8C/255.0,0x56/255.0,0x4B/255.0),
  CRGBA(0xEA/255.0,0xCF/255.0,0xE2/255.0),
  CRGBA(0xD1/255.0,0xD1/255.0,0xD1/255.0),
  CRGBA(0xBC/255.0,0xBD/255.0,0x22/255.0),
  CRGBA(0x4C/255.0,0xDA/255.0,0xD6/255.0),
  CRGBA(0x1F/255.0,0x77/255.0,0xB4/255.0),
};

CAdjacency::
CAdjacency() :
 nodes_(), sort_(SORT_NAME), ts_(0), cs_(0), ix_(-1), iy_(-1), maxValue_(0)
{
}

void
CAdjacency::
addNode(int id, const std::string &name, int group)
{
  CAdjacencyNode *node = new CAdjacencyNode(id, name, group);

  nodes_[node->id()] = node;
}

void
CAdjacency::
connectNodes(int source, int target, int value)
{
  auto p1 = nodes_.find(source);
  auto p2 = nodes_.find(target);

  if (p1 == nodes_.end() || p2 == nodes_.end()) {
    std::cerr << "Node not found" << std::endl;
    return;
  }

  CAdjacencyNode *node1 = (*p1).second;
  CAdjacencyNode *node2 = (*p2).second;

  node1->addNode(node2, value);
  node2->addNode(node1, value);
}

void
CAdjacency::
sortNodes()
{
  maxValue_ = 0;

  sortedNodes_.clear();

  for (auto pnode : nodes_) {
    CAdjacencyNode *node = pnode.second;

    sortedNodes_.push_back(node);

    maxValue_ = std::max(maxValue_, node->maxCount());
  }

  if      (sort_ == SORT_NAME)
    std::sort(sortedNodes_.begin(), sortedNodes_.end(), CAdjacencyNodeNameCmp());
  else if (sort_ == SORT_GROUP)
    std::sort(sortedNodes_.begin(), sortedNodes_.end(), CAdjacencyNodeGroupCmp());
  else if (sort_ == SORT_COUNT)
    std::sort(sortedNodes_.begin(), sortedNodes_.end(), CAdjacencyNodeCountCmp());
}

void
CAdjacency::
draw(CAdjacencyRenderer *renderer)
{
  int pw = renderer->pixelWidth ();
  int ph = renderer->pixelHeight();

  //---

  auto mapX = [&] (double x) -> double {
    return x/(1.0*pw);
  };

  auto mapY = [&] (double y) -> double {
    return (ph - 1 - y)/(1.0*ph);
  };

  auto pixelPointToWindow = [&] (CPoint2D &p) {
    p.x = mapX(p.x);
    p.y = mapY(p.y);
  };

  auto pixelRectToWindow = [&] (CBBox2D &r) {
    double x1 = mapX(r.getXMin());
    double y1 = mapY(r.getYMin());
    double x2 = mapX(r.getXMax());
    double y2 = mapY(r.getYMax());

    r = CBBox2D(x1, y2, x2, y1);
  };

  if (sortedNodes_.empty())
    return;

  //---

  renderer->setFontSize(8);

  ts_ = 0;

  for (auto node : sortedNodes_)
    ts_ = std::max(ts_, renderer->textWidth(node->name()));

  ts_ += 2;

  //---

  int nn = sortedNodes_.size();

  cs_ = std::max(std::min(renderer->pixelWidth(), renderer->pixelHeight()) - ts_, 0)/(1.0*nn);

  //---

  double py = ts_;

  for (auto node : sortedNodes_) {
    CPoint2D p(0, py + cs_ - renderer->charDescent());

    pixelPointToWindow(p);

    renderer->drawText(p, node->name(), CRGBA(0,0,0));

    py += cs_;
  }

  double px = ts_;

  for (auto node : sortedNodes_) {
    CPoint2D p(px + cs_, ts_ - 2);

    pixelPointToWindow(p);

    renderer->drawRotatedText(p, node->name(), 90,
                              CHALIGN_TYPE_LEFT, CVALIGN_TYPE_BOTTOM, CRGBA(0,0,0));

    px += cs_;
  }

  //---

  double g = 200.0/255.0;

  CBBox2D r(ts_, ts_, ts_ + nn*cs_, ts_ + nn*cs_);

  pixelRectToWindow(r);

  renderer->fillRect(r, CRGBA(g,g,g));

  //---

  py = ts_;

  for (auto node1 : sortedNodes_) {
    double px = ts_;

    for (auto node2 : sortedNodes_) {
      int value = node1->nodeValue(node2);

      double grey = 0xEE/255.0;

      CRGBA bc = CRGBA(grey,grey,grey);

      if      (node1 == node2)
        bc = colors[node1->group()];
      else if (value) {
        CRGBA c1 = colors[node1->group()];
        CRGBA c2 = colors[node2->group()];

        double s = (1.0*maxValue_ - value)/maxValue_;

        double r = (c1.getRed  () + c2.getRed  () + s*grey)/3;
        double g = (c1.getGreen() + c2.getGreen() + s*grey)/3;
        double b = (c1.getBlue () + c2.getBlue () + s*grey)/3;

        bc = CRGBA(r, g, b);
      }

      CRGBA pc = bc.getLightRGBA();

      CBBox2D r(px, py, px + cs_, py + cs_);

      pixelRectToWindow(r);

      renderer->fillRect(r, bc);
      renderer->drawRect(r, pc);

      px += cs_;
    }

    py += cs_;
  }
}