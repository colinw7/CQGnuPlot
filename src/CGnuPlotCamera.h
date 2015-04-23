#ifndef CGnuPlotCamera_H
#define CGnuPlotCamera_H

#include <CPoint3D.h>
#include <CVector3D.h>
#include <CCoordFrame3D.h>
#include <CMatrix3DH.h>

class CGnuPlotGroup;

class CGnuPlotCamera {
 public:
  enum class AxesScale {
    NONE,
    XY,
    XYZ
  };

 public:
  CGnuPlotCamera(CGnuPlotGroup *group=0);

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  void init();

  void init(const CGnuPlotCamera &camera) {
    CGnuPlotGroup *group;

    std::swap(group, group_);

    *this = camera;

    std::swap(group, group_);
  }

  void setMap();

  double xmin() const { return xmin_; }
  void setXMin(double x) { xmin_ = x; init(); }

  double xmax() const { return xmax_; }
  void setXMax(double x) { xmax_ = x; init(); }

  double ymin() const { return ymin_; }
  void setYMin(double y) { ymin_ = y; init(); }

  double ymax() const { return ymax_; }
  void setYMax(double y) { ymax_ = y; init(); }

  double near() const { return near_; }
  void setNear(double z) { near_ = z; init(); }

  double far() const { return far_ ; }
  void setFar (double z) { far_  = z; init(); }

  double rotateX() const { return rotateX_; }
  void setRotateX(double a) { rotateX_ = a; init(); }

  double rotateY() const { return rotateY_; }
  void setRotateY(double a) { rotateY_ = a; init(); }

  double rotateZ() const { return rotateZ_; }
  void setRotateZ(double a) { rotateZ_ = a; init(); }

  double scaleX() const { return scaleX_; }
  void setScaleX(double r) { scaleX_ = r; }

  double scaleY() const { return scaleY_; }
  void setScaleY(double r) { scaleY_ = r; }

  double scaleZ() const { return scaleZ_; }
  void setScaleZ(double r) { scaleZ_ = r; }

  const AxesScale &axesScale() const { return axesScale_; }
  void setAxesScale(const AxesScale &v) { axesScale_ = v; }

  void setPosition(const CPoint3D &position) {
    coordFrame_.setOrigin(position);
  }

  void setDirection(const CVector3D &dir);

  void moveDX(double dx) { coordFrame_.moveX(dx); }
  void moveDY(double dy) { coordFrame_.moveY(dy); }
  void moveDZ(double dz) { coordFrame_.moveZ(dz); }

  void rotateDX(double dx) { coordFrame_.rotateAboutX(dx); }
  void rotateDY(double dy) { coordFrame_.rotateAboutY(dy); }
  void rotateDZ(double dz) { coordFrame_.rotateAboutZ(dz); }

  CPoint3D transform(const CPoint3D &p) const;

  void showView(std::ostream &os) const;

  void unsetView();

 private:
  CGnuPlotGroup* group_      { 0 };
  bool           enabled_    { true };
  double         xmin_       { -1 };
  double         xmax_       {  1 };
  double         ymin_       { -1 };
  double         ymax_       {  1 };
  double         near_       { 0.1 };
  double         far_        { 100 };
  double         rotateX_    { 60.0 };
  double         rotateY_    {  0.0 };
  double         rotateZ_    { 30.0 };
  double         scaleX_     { 1.0 };
  double         scaleY_     { 1.0 };
  double         scaleZ_     { 1.0 };
  AxesScale      axesScale_  { AxesScale::NONE };
  CCoordFrame3D  coordFrame_;
  CVector3D      direction_  { 0, 0, 1 };
  double         fov_        { 90 };
  CMatrix3DH     projMatrix_;
};

#endif