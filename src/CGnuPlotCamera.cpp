#include <CGnuPlotCamera.h>
#include <CGnuPlotGroup.h>

CGnuPlotCamera::
CGnuPlotCamera(CGnuPlotGroup *group) :
 group_(group)
{
  init();
}

void
CGnuPlotCamera::
init()
{
  coordFrame_.init();

  setDirection(CVector3D(0,0,1));

//projMatrix_.buildPerspective(fov, 1.0, near, far);
  projMatrix_.buildOrtho(xmin_, xmax_, ymin_, ymax_, near_, far_);
//projMatrix_.buildFrustrum(-2, 2, -2, 2, near, far);

  rotateDX(M_PI*rotateX_/180.0);
  rotateDY(M_PI*rotateY_/180.0);
  rotateDZ(M_PI*rotateZ_/180.0);

  if (group_)
    group_->cameraChanged();
}

void
CGnuPlotCamera::
setMap()
{
  enabled_ = false;

  rotateX_ = 0.0;
  rotateY_ = 0.0;
  rotateZ_ = 0.0;

  init();
}

void
CGnuPlotCamera::
setDirection(const CVector3D &dir)
{
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

CPoint3D
CGnuPlotCamera::
transform(const CPoint3D &p) const
{
  if (! enabled_) return p;

  CPoint3D p1 = coordFrame_.transformTo(p);

  CPoint3D p2;

  projMatrix_.multiplyPoint(p1, p2);

  return p2;
}

void
CGnuPlotCamera::
showView(std::ostream &os) const
{
  os << "view is " << rotateX_ << ", " << rotateZ_ << " rot_z";
  os << ", " << scaleX_ << ", " << scaleZ_ << " scale_z" << std::endl;

  if      (axesScale_ == AxesScale::NONE)
    os << " axes are independently scaled" << std::endl;
  else if (axesScale_ == AxesScale::XY)
    os << " x/y axes are on the same scale" << std::endl;
  else if (axesScale_ == AxesScale::XYZ)
    os << " x/y/z axes are on the same scale" << std::endl;
}

void
CGnuPlotCamera::
unsetView()
{
  rotateX_   = 60.0;
  rotateZ_   = 30.0;
  scaleX_    = 1.0;
  scaleZ_    = 1.0;
  axesScale_ = AxesScale::NONE;
}
