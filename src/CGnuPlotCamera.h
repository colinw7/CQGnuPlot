#ifndef CGnuPlotCamera_H
#define CGnuPlotCamera_H

class CGnuPlotCamera {
 public:
  enum class AxesScale {
    NONE,
    XY,
    XYZ
  };

 public:
  CGnuPlotCamera() { init(); }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  void init() {
    coordFrame_.init();

    setDirection(CVector3D(0,0,1));

  //projMatrix.buildPerspective(fov, 1.0, near, far);
    projMatrix_.buildOrtho(xmin_, xmax_, ymin_, ymax_, near_, far_);
  //projMatrix.buildFrustrum(-2, 2, -2, 2, near, far);

    rotateDX(M_PI*rotateX_/180.0);
    rotateDY(M_PI*rotateY_/180.0);
    rotateDZ(M_PI*rotateZ_/180.0);
  }

  void setMap() {
    enabled_ = false;

    rotateX_ = 0.0;
    rotateY_ = 0.0;
    rotateZ_ = 0.0;

    init();
  }

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

  void setDirection(const CVector3D &dir) {
    CVector3D right, up, dir1;

    coordFrame_.getBasis(right, up, dir1);

    dir1 = dir.unit();

    right = dir1 .crossProduct(up );
    up    = right.crossProduct(dir1);

    if (COrthonormalBasis3DT<double>::validate(right, up, dir1)) {
      coordFrame_.setBasis(right, up, dir1);

      direction_ = dir;
    }
  }

  void moveDX(double dx) { coordFrame_.moveX(dx); }
  void moveDY(double dy) { coordFrame_.moveY(dy); }
  void moveDZ(double dz) { coordFrame_.moveZ(dz); }

  void rotateDX(double dx) { coordFrame_.rotateAboutX(dx); }
  void rotateDY(double dy) { coordFrame_.rotateAboutY(dy); }
  void rotateDZ(double dz) { coordFrame_.rotateAboutZ(dz); }

  CPoint3D transform(const CPoint3D &p) const {
    if (! enabled_) return p;

    CPoint3D p1 = coordFrame_.transformTo(p);

    CPoint3D p2;

    projMatrix_.multiplyPoint(p1, p2);

    return p2;
  }

  void showView(std::ostream &os) const {
    os << "view is " << rotateX_ << ", " << rotateZ_ << " rot_z";
    os << ", " << scaleX_ << ", " << scaleZ_ << " scale_z" << std::endl;

    if      (axesScale_ == AxesScale::NONE)
      os << " axes are independently scaled" << std::endl;
    else if (axesScale_ == AxesScale::XY)
      os << " x/y axes are on the same scale" << std::endl;
    else if (axesScale_ == AxesScale::XYZ)
      os << " x/y/z axes are on the same scale" << std::endl;
  }

  void unsetView() {
    rotateX_   = 60.0;
    rotateZ_   = 30.0;
    scaleX_    = 1.0;
    scaleZ_    = 1.0;
    axesScale_ = AxesScale::NONE;
  }

 private:
  bool          enabled_    { true };
  double        xmin_       { -1 };
  double        xmax_       {  1 };
  double        ymin_       { -1 };
  double        ymax_       {  1 };
  double        near_       { 0.1 };
  double        far_        { 100 };
  double        rotateX_    { 60.0 };
  double        rotateY_    {  0.0 };
  double        rotateZ_    { 30.0 };
  double        scaleX_     { 1.0 };
  double        scaleY_     { 1.0 };
  double        scaleZ_     { 1.0 };
  AxesScale     axesScale_  { AxesScale::NONE };
  CCoordFrame3D coordFrame_;
  CVector3D     direction_  { 0, 0, 1 };
  double        fov_        { 90 };
  CMatrix3DH    projMatrix_;
};

#endif
