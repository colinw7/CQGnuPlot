#ifndef CSunburst_H
#define CSunburst_H

#include <CRGBA.h>

#include <vector>
#include <string>
#include <map>
#include <algorithm>

class CSunburstPainter;

//------

class CSunburst {
 public:
  enum Order {
    ORDER_SIZE,
    ORDER_COUNT
  };

 public:
  class RootNode;
  class HierNode;
  class Node;

 public:
  CSunburst();

  void addRoot();

  HierNode *addHierNode(const std::string &name);
  HierNode *addHierNode(HierNode *parent, const std::string &name);

  Node *addNode(const std::string &name, double value);
  Node *addNode(HierNode *parent, const std::string &name, double value);

  HierNode *getHierNodeAt(double x, double y);
  Node     *getNodeAt    (double x, double y);

  void pack();

  void draw(CSunburstPainter *painter);

 private:
  Node *getNodeAt(HierNode *node, double x, double y) const;

  void drawNodes(CSunburstPainter *p, HierNode *hier);

  void drawNode(CSunburstPainter *p, Node *node);

  void draw(CSunburstPainter *p, double px, double py, double len);

 public:
  class Node {
   private:
    static uint nextId() {
      static int lastId = 0;

      return ++lastId;
    }

   public:
    Node(HierNode *parent, const std::string &name="", double size=1.0) :
     parent_(parent), id_(nextId()), name_(name), size_(size), r_(0.0), a_(0.0),
     dr_(0.0), da_(0.0), colorId_(0), placed_(false) {
    }

    virtual ~Node() { }

    HierNode *parent() const { return parent_; }

    uint id() const { return id_; }

    virtual bool isRoot() const { return false; }
    virtual bool isHier() const { return false; }

    const std::string &name() const { return name_; }

    virtual double size() const { return size_; }
    virtual void setSize(double size) { size_ = size; }

    virtual int depth() const { return 1; }

    virtual int numNodes() const { return 1; }

    double r () const { return r_ ; }
    double a () const { return a_ ; }
    double dr() const { return dr_; }
    double da() const { return da_; }

    int colorId() const { return colorId_; }
    virtual void setColorId(int colorId) { colorId_ = colorId; }

    virtual void setPosition(double r, double a, double dr, double da) {
      r_  = r ; a_  = a ;
      dr_ = dr; da_ = da;

      placed_ = true;
    }

    void unplace() { placed_ = false; }

    bool placed() const { return placed_; }

    static double normalizeAngle(double a) {
      while (a <    0.0) a += 360.0;
      while (a >= 360.0) a -= 360.0;

      return a;
    }

    bool pointInside(double x, double y) {
      if (! placed_) return false;

      double r = sqrt(x*x + y*y);

      if (r < r_ || r > r_ + dr_) return false;

      double a = normalizeAngle(180.0*atan2(y, x)/M_PI);

      double a1 = normalizeAngle(a_);
      double a2 = a1 + da_;

      if (a2 > a1) {
        if (a2 >= 360.0) {
          double da = a2 - 360.0; a -= da; a1 -= da; a2 = 360.0;
          a = normalizeAngle(a);
        }

        if (a < a1 || a > a2)
          return false;
      }
      else {
        if (a2 < 0.0) {
          double da = -a2; a += da; a1 += da; a2 = 0.0;

          a = normalizeAngle(a);
        }

        if (a < a2 || a > a1)
          return false;
      }

      return true;
    }

   protected:
    HierNode    *parent_;
    uint         id_;
    std::string  name_;
    double       size_;
    double       r_, a_;
    double       dr_, da_;
    int          colorId_;
    bool         placed_;
  };

  struct NodeNameCmp {
    // sort reverse alphabetic no case
    bool operator()(const Node *n1, const Node *n2) {
      const std::string &name1 = n1->name();
      const std::string &name2 = n2->name();

      int l1 = name1.size();
      int l2 = name2.size();

      for (int i = 0; i < std::max(l1, l2); ++i) {
        char c1 = (i < l1 ? tolower(name1[i]) : '\0');
        char c2 = (i < l2 ? tolower(name2[i]) : '\0');

        if (c1 > c2) return true;
        if (c1 < c2) return false;
      }

      return false;
    }
  };

  struct NodeSizeCmp {
    bool operator()(const Node *n1, const Node *n2) {
      return n1->size() < n2->size();
    }
  };

  struct NodeCountCmp {
    bool operator()(const Node *n1, const Node *n2) {
      return n1->numNodes() < n2->numNodes();
    }
  };

  typedef std::vector<Node *> Nodes;

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

    double size() const {
      double s = 0.0;

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        s += (*p)->size();

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        s += (*pn)->size();

      return s;
    }

    int depth() const {
      int depth = 1;

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        depth = std::max(depth, (*p)->depth() + 1);

      return depth;
    }

    int numNodes() const {
      int num = nodes_.size();

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        num += (*p)->numNodes();

      return std::max(num, 1);
    }

    const Nodes &getNodes() const { return nodes_; }

    const Children &getChildren() const { return children_; }

    int nodeIndex(Node *node) const {
      for (uint i = 0; i < nodes_.size(); ++i)
        if (nodes_[i] == node)
          return i;

      return -1;
    }

    int childIndex(Node *node) const {
      for (uint i = 0; i < children_.size(); ++i)
        if (children_[i] == node)
          return i;

      return -1;
    }

    void unplace() {
      unplaceNodes();
    }

    void unplaceNodes() {
      Node::unplace();

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        (*p)->unplaceNodes();

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        (*pn)->unplace();
    }

    void packNodes(RootNode *root, double ri, double ro, double dr, double a, double da) {
      int d = depth();

      if (dr <= 0.0)
        dr = (ro - ri)/d;

      double s = (root->order() == ORDER_SIZE ? size() : numNodes());

      double da1 = da/s;

      packSubNodes(root, ri, dr, a, da1);
    }

    void packSubNodes(RootNode *root, double ri, double dr, double a, double da) {
      // make single list of nodes to pack
      Nodes nodes;

      for (Children::const_iterator p = children_.begin(); p != children_.end(); ++p)
        nodes.push_back(*p);

      for (Nodes::const_iterator pn = nodes_.begin(); pn != nodes_.end(); ++pn)
        nodes.push_back(*pn);

      if (root->sort()) {
#if 0
        if (root->order() == ORDER_SIZE)
          std::sort(nodes.begin(), nodes.end(), NodeSizeCmp());
        else
          std::sort(nodes.begin(), nodes.end(), NodeCountCmp());
#else
        std::sort(nodes.begin(), nodes.end(), NodeNameCmp());
      }
#endif

      //---

      placed_ = true;

      // place each node
      double a1 = a;

      for (Nodes::const_iterator pn = nodes.begin(); pn != nodes.end(); ++pn) {
        Node *node = *pn;

        double s = (root->order() == ORDER_SIZE ? node->size() : node->numNodes());

        node->setPosition(ri, a1, dr, s*da);

        HierNode *hierNode = dynamic_cast<HierNode *>(node);

        if (hierNode)
          hierNode->packSubNodes(root, ri + dr, dr, a1, da);

        a1 += s*da;
      }
    }

    void addNode(Node *node) {
      nodes_.push_back(node);
    }

   private:
    Nodes     nodes_;
    Children  children_;
  };

  class RootNode : public HierNode {
   public:
    RootNode(const std::string &name="") :
     HierNode(0, name), sort_(true), order_(ORDER_SIZE) {
    }

    bool isRoot() const override { return true; }

    bool sort() const { return sort_; }
    void setSort(bool sort) { sort_ = sort; }

    Order order() const { return order_; }
    void setOrder(Order order) { order_ = order; }

    void packNodes(double ri=0.5, double ro=1.0, double dr=0.0, double a=0.0, double da=360.0) {
      HierNode::packNodes(this, ri, ro, dr, a, da);
    }

   private:
    bool  sort_;
    Order order_;
  };

 private:
  std::vector<RootNode *> roots_;
};

//------

class CSunburstPainter {
 public:
  CSunburstPainter() { }

  virtual ~CSunburstPainter() { }

  virtual void drawNode(CSunburst::Node *node) = 0;
};

#endif
