#ifndef CTreeMap_H
#define CTreeMap_H

#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

class CTreeMapPainter;

//------

class CTreeMap {
 public:
  class HierNode;
  class Node;

 public:
  CTreeMap();

  HierNode *root() const { return root_; }

  HierNode *createRootNode(const std::string &name="");

  HierNode *addHierNode(const std::string &name);
  HierNode *addHierNode(HierNode *parent, const std::string &name);

  Node *addNode(const std::string &name, double value);
  Node *addNode(HierNode *parent, const std::string &name, double value);

  HierNode *getHierNodeAt(double x, double y);
  Node     *getNodeAt    (double x, double y);

  void place(double x, double y, double w, double h);

  void draw(CTreeMapPainter *painter);

 private:
  HierNode *getHierNodeAt(HierNode *hier, double x, double y);
  Node     *getNodeAt    (HierNode *hier, double x, double y);

  void drawNodes(CTreeMapPainter *painter, HierNode *parent, int depth);

 public:
  class Node {
   private:
    static uint nextId() {
      static int lastId = 0;

      return ++lastId;
    }

   public:
    Node(HierNode *parent, const std::string &name="", double size=1.0) :
     parent_(parent), id_(nextId()), name_(name), size_(size), x_(0.0), y_(0.0), w_(1.0), h_(1.0),
     colorId_(0), placed_(false) {
    }

    virtual ~Node() { }

    HierNode *parent() const { return parent_; }

    uint id() const { return id_; }

    virtual bool isHier() const { return false; }

    const std::string &name() const { return name_; }

    virtual double size() const { return size_; }
    virtual void setSize(double size) { size_ = size; }

    virtual bool isOpen() const { return true; }
    virtual void setOpen(bool) { }

    double x() const { return x_; }
    double y() const { return y_; }
    double w() const { return w_; }
    double h() const { return h_; }

    int colorId() const { return colorId_; }

    virtual void setColorId(int id) { colorId_ = id; }

    virtual void setPosition(double x, double y, double w, double h) {
      x_ = x; y_ = y;
      w_ = w; h_ = h;

      //std::cerr << "Node: " << name() << " @ ( " << x_ << "," << y_ << ")" <<
      //             " [" << w_ << "," << h_ << "]" << std::endl;

      placed_ = true;
    }

    void unplace() { placed_ = false; }

    bool placed() const { return placed_; }

    virtual bool contains(double x, double y) {
      return (x >= x_ && x <= (x_ + w_) && y >= y_ && y <= (y_ + h_));
    }

   protected:
    HierNode    *parent_;
    uint         id_;
    std::string  name_;
    double       size_;
    double       x_, y_;
    double       w_, h_;
    int          colorId_;
    bool         placed_;
  };

  //---

  struct NodeCmp {
    bool operator()(const Node *n1, const Node *n2) {
      return n1->size() < n2->size();
    }
  };

  //---

  typedef std::vector<Node *> Nodes;

  //---

  class HierNode : public Node {
   public:
    typedef std::vector<HierNode *> Children;

   public:
    HierNode(HierNode *parent=0, const std::string &name="") :
     Node(parent, name) {
      if (parent_)
        parent_->children_.push_back(this);
    }

    bool isHier() const override { return true; }

    double size() const override {
      double s = 0.0;

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        s += (*p)->size();

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        s += (*pn)->size();

      return s;
    }

    const Nodes &getNodes() const { return nodes_; }

    const Children &getChildren() const { return children_; }

    bool isOpen() const override { return open_; }
    void setOpen(bool b) override { open_ = b; }

    HierNode *findChild(const std::string &name) const;

    void packNodes(double x, double y, double w, double h) {
      // make single list of nodes to pack
      Nodes nodes;

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        nodes.push_back(*p);

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        nodes.push_back(*pn);

      // sort nodes by size
      std::sort(nodes.begin(), nodes.end(), NodeCmp());

      //std::cerr << name() << std::endl;
      //for (uint i = 0; i < nodes.size(); ++i)
      //  std::cerr << " " << nodes[i]->name() << ":" << nodes[i]->size() << std::endl;

      packSubNodes(x, y, w, h, nodes);
    }

    void packSubNodes(double x, double y, double w, double h, const Nodes &nodes) {
      // place nodes
      int n = nodes.size();
      if (n == 0) return;

      if (n >= 2) {
        int n1 = n/2;

        Nodes  nodes1, nodes2;
        double size1 = 0.0, size2 = 0.0;

        for (int i = 0; i < n1; ++i) {
          size1 += nodes[i]->size();

          nodes1.push_back(nodes[i]);
        }

        for (int i = n1; i <  n; ++i) {
          size2 += nodes[i]->size();

          nodes2.push_back(nodes[i]);
        }

        // split area = (w*h) if largest direction
        // e.g. split at w1. area1 = w1*h; area2 = (w - w1)*h;
        // area1/area2 = w1/(w - w1) = size1/size2;
        // w1*size2 = w*size1 - w1*size1;
        // w1 = (w*size1)/(size1 + size2);

        double f = size1/(size1 + size2);

        if (w >= h) {
          double w1 = f*w;

          packSubNodes(x     , y,     w1, h, nodes1);
          packSubNodes(x + w1, y, w - w1, h, nodes2);
        }
        else {
          double h1 = f*h;

          packSubNodes(x, y     , w,     h1, nodes1);
          packSubNodes(x, y + h1, w, h - h1, nodes2);
        }
      }
      else {
        Node *node = nodes[0];

        node->setPosition(x, y, w, h);
      }
    }

    void setPosition(double x, double y, double w, double h) override {
      Node::setPosition(x, y, w, h);

      packNodes(x, y, w, h);
    }

    void addNode(Node *node) {
      nodes_.push_back(node);
    }

   private:
    Nodes    nodes_;
    Children children_;
    bool     open_ { true };
  };

 private:
  HierNode *root_ { 0 };
};

//------

class CTreeMapPainter {
 public:
  CTreeMapPainter() { }

  virtual ~CTreeMapPainter() { }

  virtual void drawNode(CTreeMap::Node *node, int depth) = 0;

  virtual void drawHierNode(CTreeMap::HierNode * /*node*/, int /*depth*/) { }
};

#endif
