#ifndef CGnuPlotRotate_H
#define CGnuPlotRotate_H

#include <CAngle.h>

class CGnuPlotRotate {
 public:
  CGnuPlotRotate() { }

  CGnuPlotRotate(const CPoint2D &o, double a) :
   set_(true), origin_(o), angle_(a) {
     ra_ = CAngle::Deg2Rad(angle_);
  }

  bool isSet() const { return set_; }

  const CPoint2D &origin() const { return origin_; }
  void setOrigin(const CPoint2D &v) { origin_ = v; }

  double angle() const { return angle_; }
  void setAngle(double r) { angle_ = r; ra_ = CAngle::Deg2Rad(angle_); }

  CPoint2D apply(const CPoint2D &p) const {
    if (isSet())
      return CMathGeom2D::RotatePoint(p, ra_, origin_);
    else
      return p;
  }

 private:
  bool           set_    { false };
  CPoint2D       origin_ { 0 , 0 };
  double         angle_  { 0 };
  mutable double ra_     { 0 };
};

#endif
