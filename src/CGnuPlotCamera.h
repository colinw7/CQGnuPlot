#ifndef CGnuPlotCamera_H
#define CGnuPlotCamera_H

class CGnuPlotCamera {
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

  double rotateX() const { return rotateX_; }
  double rotateY() const { return rotateY_; }
  double rotateZ() const { return rotateZ_; }

  void setRotateX(double a) { rotateX_ = a; init(); }
  void setRotateY(double a) { rotateY_ = a; init(); }
  void setRotateZ(double a) { rotateZ_ = a; init(); }

  double xmin() const { return xmin_; }
  double xmax() const { return xmax_; }
  double ymin() const { return ymin_; }
  double ymax() const { return ymax_; }
  double near() const { return near_; }
  double far () const { return far_ ; }

  void setXMin(double x) { xmin_ = x; init(); }
  void setXMax(double x) { xmax_ = x; init(); }
  void setYMin(double y) { ymin_ = y; init(); }
  void setYMax(double y) { ymax_ = y; init(); }
  void setNear(double z) { near_ = z; init(); }
  void setFar (double z) { far_  = z; init(); }

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

 private:
  bool          enabled_    { true };
  CCoordFrame3D coordFrame_;
  CVector3D     direction_  { 0, 0, 1 };
  double        fov_        { 90 };
  double        xmin_       { -1 };
  double        xmax_       {  1 };
  double        ymin_       { -1 };
  double        ymax_       {  1 };
  double        near_       { 0.1 };
  double        far_        { 100 };
  CMatrix3DH    projMatrix_;
  double        rotateX_    { 60.0 };
  double        rotateY_    {  0.0 };
  double        rotateZ_    { 45.0 };
};

#endif
