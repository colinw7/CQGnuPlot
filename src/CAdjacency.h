#ifndef CAdjacency_H
#define CAdjacency_H

#include <CPoint2D.h>
#include <CAlignType.h>
#include <CBBox2D.h>
#include <CRGBA.h>
#include <map>
#include <vector>

class CAdjacencyRenderer;

class CAdjacencyNode {
 public:
  CAdjacencyNode(int id, const std::string &name, int group) :
   id_(id), name_(name), group_(group) {
  }

  int                id   () const { return id_   ; }
  const std::string &name () const { return name_ ; }
  int                group() const { return group_; }

  int count() const { return count_; }

  int maxCount() const { return maxCount_; }

  void addNode(CAdjacencyNode *node, int count) {
    nodes_[node->id()] = NodeValue(node, count);

    count_ += count;

    maxCount_ = std::max(maxCount_, count);
  }

  int nodeValue(CAdjacencyNode *node) const {
    if (node == this) return 1;

    NodeMap::const_iterator p = nodes_.find(node->id());

    if (p == nodes_.end())
      return 0;

    return (*p).second.second;
  }

 private:
  typedef std::pair<CAdjacencyNode *,int> NodeValue;
  typedef std::map<int,NodeValue>         NodeMap;

  int         id_ { 0 };
  std::string name_;
  int         group_ { 0 };
  int         count_ { 0 };
  int         maxCount_ { 0 };
  NodeMap     nodes_;
};

struct CAdjacencyNodeNameCmp {
  // sort reverse alphabetic no case
  bool operator()(const CAdjacencyNode *n1, const CAdjacencyNode *n2) {
    return nameCmp(n1, n2);
  }

  static bool nameCmp(const CAdjacencyNode *n1, const CAdjacencyNode *n2) {
    const std::string &name1 = n1->name();
    const std::string &name2 = n2->name();

    int l1 = int(name1.size());
    int l2 = int(name2.size());

    for (int i = 0; i < std::max(l1, l2); ++i) {
      char c1 = char(i < l1 ? tolower(name1[i]) : '\0');
      char c2 = char(i < l2 ? tolower(name2[i]) : '\0');

      if (c1 < c2) return true;
      if (c1 > c2) return false;
    }

    return false;
  }

};

struct CAdjacencyNodeCountCmp {
  bool operator()(const CAdjacencyNode *n1, const CAdjacencyNode *n2) {
    if (n1->count() > n2->count()) return true;
    if (n1->count() < n2->count()) return false;

    return CAdjacencyNodeNameCmp::nameCmp(n1, n2);
  }
};

struct CAdjacencyNodeGroupCmp {
  bool operator()(const CAdjacencyNode *n1, const CAdjacencyNode *n2) {
    if (n1->group() > n2->group()) return true;
    if (n1->group() < n2->group()) return false;

    return CAdjacencyNodeNameCmp::nameCmp(n1, n2);
  }
};

class CAdjacency {
 public:
  enum class SortType {
    NAME,
    GROUP,
    COUNT
  };

  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CAdjacency();

  void addNode(int id, const std::string &name, int group);

  void connectNodes(int source, int target, int value);

  void sortNodes();

  void draw(CAdjacencyRenderer *renderer);

  void nodeAtPoint(double x, double y);

  CRGBA nodeColor(CAdjacencyRenderer *renderer, CAdjacencyNode *node1, CAdjacencyNode *node2) const;

  CRGBA blendColors(const CRGBA &c1, const CRGBA &c2, int value) const;

 private:
  typedef std::map<int,CAdjacencyNode *> NodeMap;
  typedef std::vector<CAdjacencyNode *>  NodeArray;

  NodeMap   nodes_;
  NodeArray sortedNodes_;
  SortType  sort_ { SortType::NAME };
  int       ts_ { 0 };
  double    cs_ { 0 };
  int       ix_ { -1 };
  int       iy_ { -1 };
  int       maxValue_ { 0 };
  double    grey_ { 0 };
};

class CAdjacencyRenderer {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CAdjacencyRenderer() { }

  virtual ~CAdjacencyRenderer() { }

  virtual int pixelWidth () const = 0;
  virtual int pixelHeight() const = 0;

  virtual void setFontSize(int h) = 0;

  virtual int textWidth(const std::string &str) = 0;

  virtual int charDescent() = 0;

  virtual void fillRect(const CBBox2D &r, const CRGBA &c) = 0;

  virtual void drawNodeRect(const CBBox2D &r, const CRGBA &c, int value,
                            CAdjacencyNode *node1, CAdjacencyNode *node2) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) = 0;

  virtual void drawRotatedText(const CPoint2D &p, const std::string &text, double a,
                               const HAlignPos &halignPos, const VAlignPos &valignPos,
                               const CRGBA &c) = 0;

  virtual CRGBA indexColor(int i) const = 0;
};

#endif
