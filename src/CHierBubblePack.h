#ifndef CHierBubblePack_H
#define CHierBubblePack_H

#include <CirclePack.h>

class CHierBubblePack {
 public:
  class Node : public CircleNode {
   private:
    static uint nextId() {
      static int lastId = 0;

      return ++lastId;
    }

   public:
    Node(const std::string &name, double size) :
     CircleNode(), id_(nextId()), name_(name), size_(size) {
      double r = sqrt(size/(2*M_PI));

      setRadius(r);
    }

    virtual ~Node() { }

    uint id() const { return id_; }

    const std::string &name() const { return name_; }

    virtual double size() const { return size_; }

    void setRadius(double r) { r_ = r; nr_ = r_; }

    double radiusOrig() const { return r_; }

    void setPosition(double x, double y) override {
      CircleNode::setPosition(x, y);

      placed_ = true;
    }

    bool contains(double x, double y) const {
      double dx = this->x() - x;
      double dy = this->y() - y;

      return hypot(dx, dy) <= this->radius();
    }

    bool placed() const { return placed_; }

    friend bool operator<(const Node &n1, const Node &n2) {
      return n1.r_ < n2.r_;
    }

   protected:
    uint        id_     { 0 };
    std::string name_;
    double      size_   { 1 };
    double      nr_     { 0 };
    bool        placed_ { false };
  };

  struct NodeCmp {
    bool operator()(const Node *n1, const Node *n2) {
      return (*n1) < (*n2);
    }
  };

  typedef std::vector<Node *> Nodes;

  class HierNode : public Node {
   public:
    typedef std::vector<HierNode *> Children;

   public:
    HierNode(HierNode *parent=0, const std::string &name="") :
     Node(name, 0), parent_(parent) {
      if (parent_)
        parent_->children_.push_back(this);
    }

    const Nodes &getNodes() const { return nodes_; }

    const Children &getChildren() const { return children_; }

    double size() const override {
      double s { 0.0 };

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        s += (*p)->size();

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        s += (*pn)->size();

      return s;
    }

    HierNode *findChild(const std::string &name) const {
      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        if ((*p)->name() == name)
          return *p;

      return 0;
    }

    void packNodes() {
      // pack child hier nodes first
      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        (*p)->packNodes();

      //---

      // make single list of nodes to pack
      Nodes packNodes;

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        packNodes.push_back(*p);

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        packNodes.push_back(*pn);

      // sort nodes
      std::sort(packNodes.begin(), packNodes.end(), NodeCmp());

      // pack nodes
      for (Nodes::const_iterator pp = packNodes.begin(); pp != packNodes.end(); ++pp)
        pack_.addNode(*pp);

      //---

      // get bounding circle
      double xc, yc, r;

      pack_.boundingCircle(xc, yc, r);

      // set center and radius
      x_ = xc;
      y_ = yc;

      setRadius(r);

      //setRadius(std::max(std::max(fabs(xmin), xmax), std::max(fabs(ymin), ymax)));
    }

    void addNode(Node *node) {
      nodes_.push_back(node);
    }

    void setPosition(double x, double y) {
      double dx = x - this->x();
      double dy = y - this->y();

      Node::setPosition(x, y);

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn) {
        Node *node = *pn;

        node->setPosition(node->x() + dx, node->y() + dy);
      }

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p) {
        HierNode *hierNode = *p;

        hierNode->setPosition(hierNode->x() + dx, hierNode->y() + dy);
      }
    }

    Node *getNodeAtPoint(double x, double y) const {
      // check hier children first
      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p) {
        Node *node = (*p)->getNodeAtPoint(x, y);

        if (node)
          return node;
      }

      // check children
      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn) {
        if ((*pn)->contains(x, y))
          return *pn;
      }

      // check self
      if (contains(x, y))
        return const_cast<HierNode *>(this);

      return 0;
    }

   private:
    typedef CirclePack<Node> Pack;

    HierNode *parent_;
    Nodes     nodes_;
    Pack      pack_;
    Children  children_;
  };

 public:
  CHierBubblePack() { }

  HierNode *root() const { return root_; }

  HierNode *createRootNode(const std::string &name) {
    assert(! root_);

    root_ = new HierNode(0, name);

    return root_;
  }

  HierNode *createHierNode(HierNode *hier, const std::string &name) {
    assert(hier);

    return new HierNode(hier, name);
  }

  Node *createNode(HierNode *hier, const std::string &name, double size) {
    Node *node = new Node(name, size);

    hier->addNode(node);

    return node;
  }

  void packNodes() {
    root_->packNodes();
  }

  Node *getNodeAtPoint(double x, double y) const {
   if (root_)
      return root_->getNodeAtPoint(x, y);
    else
      return 0;
  }

 private:
  HierNode *root_ { 0 };
};

#endif
