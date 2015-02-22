#ifndef CGnuPlotPolygon_H
#define CGnuPlotPolygon_H

#include <CGnuPlotObject.h>

class CGnuPlotPolygon : public CGnuPlotGroupAnnotation {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  static const char *getName() { return "polygon"; }

  CGnuPlotPolygon(CGnuPlotGroup *group) :
   CGnuPlotGroupAnnotation(group) {
  }

  virtual ~CGnuPlotPolygon() { }

  CGnuPlotPolygon *setData(const CGnuPlotPolygon *poly) {
    (void) CGnuPlotGroupAnnotation::setData(poly);

    points_ = poly->points_;
    lw_     = poly->lw_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::POLYGON; }

  void addPoint(const CPoint2D &p) {
    points_.push_back(p);
  }

  const Points &getPoints() const { return points_; }

  const COptReal &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  void draw() const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &) const;

 protected:
  Points   points_;
  COptReal lw_;
};

typedef std::shared_ptr<CGnuPlotPolygon> CGnuPlotPolygonP;

#endif
