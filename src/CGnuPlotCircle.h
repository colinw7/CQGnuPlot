#ifndef CGnuPlotCircle_H
#define CGnuPlotCircle_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotCircle : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "ellipse"; }

  CGnuPlotCircle(CGnuPlotGroup *group) :
   CGnuPlotGroupAnnotation(group) {
  }

  virtual ~CGnuPlotCircle() { }

  CGnuPlotCircle *setData(const CGnuPlotCircle *ellipse) {
    (void) CGnuPlotGroupAnnotation::setData(ellipse);

    p_ = ellipse->p_;
    r_ = ellipse->r_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::CIRCLE; }

  const CGnuPlotPosition &getCenter() const { return p_; }
  void setCenter(const CGnuPlotPosition &p) { p_ = p; }

  double getRadius() const { return r_; }
  void setRadius(double r) { r_ = r; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &os) const;

 protected:
  CGnuPlotPosition  p_;
  double            r_ { 1 };
  CGnuPlotFillStyle fs_;
};

typedef std::shared_ptr<CGnuPlotCircle> CGnuPlotCircleP;

#endif
