#ifndef CBubblePack_H
#define CBubblePack_H

#include <CirclePack.h>

class CBubbleNode : public CCircleNode {
 public:
  CBubbleNode(const std::string &name, double value, int id) :
   CCircleNode(sqrt(value/M_PI), 0.0, 0.0), name_(name), value_(value), id_(id) {
  }

  const std::string &name() const { return name_; }

  int id() const { return id_; }

  double value() const { return value_; }

 private:
  std::string name_;
  double      value_ { 0.0 };
  int         id_    { 0 };
};

class CBubblePack {
 public:
  typedef CCirclePack<CBubbleNode> Pack;

 public:
  CBubblePack() { }

  void addNode(const std::string &name, double value, int id) {
    double r = sqrt(value/M_PI);

    CBubbleNode *node = new CBubbleNode(name, r, id);

    pack_.addNode(node);
  }

  void boundingBox(double &xmin, double &ymin, double &xmax, double &ymax) {
    pack_.boundingBox(xmin, ymin, xmax, ymax);
  }

  const Pack::Nodes &nodes() const { return pack_.nodes(); }

 private:
  Pack pack_;
};

#endif
