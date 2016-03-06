#ifndef CForceDirected_H
#define CForceDirected_H

#include <Springy.h>

class CForceDirected {
 public:
  CForceDirected() {
    graph_  = new Springy::Graph;
    layout_ = new Springy::Layout(graph_, stiffness_, repulsion_, damping_);
  }

  Springy::Node *newNode(const std::string &name) {
    return graph_->newNode(name);
  }

  Springy::Node *getNode(int id) {
    return graph_->getNode(id);
  }

  Springy::Edge *newEdge(Springy::Node *node1, Springy::Node *node2) {
    return graph_->newEdge(node1, node2);
  }

  Springy::Edges edges() const { return graph_->edges(); }
  Springy::Nodes nodes() const { return graph_->nodes(); }

  void step(double dt) {
    layout_->step(dt);
  }

  Springy::Spring *spring(Springy::Edge *edge) {
    return layout_->spring(edge);
  }

  Springy::Point *point(Springy::Node *node) {
    return layout_->point(node);
  }

  Springy::NodePoint nearest(const Springy::Vector &v) {
    return layout_->nearest(v);
  }

  Springy::Node *currentNode() const { return currentNode_; }
  void setCurrentNode(Springy::Node *node) { currentNode_ = node; }

  Springy::Point *currentPoint() const { return currentPoint_; }
  void setCurrentPoint(Springy::Point *point) { currentPoint_ = point; }

 private:
  double           stiffness_    { 400.0 };
  double           repulsion_    { 400.0 };
  double           damping_      { 0.5 };
  Springy::Graph  *graph_        { 0 };
  Springy::Layout *layout_       { 0 };
  Springy::Node   *currentNode_  { 0 };
  Springy::Point  *currentPoint_ { 0 };
};

#endif
