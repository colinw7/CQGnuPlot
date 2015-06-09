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

  right = up  .crossProduct(dir1);
  up    = dir1.crossProduct(right);

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

  // map to unit radius cube centered at 0,0
  CGnuPlotAxisData &xaxis = group_->xaxis(1);
  CGnuPlotAxisData &yaxis = group_->yaxis(1);
  CGnuPlotAxisData &zaxis = group_->zaxis(1);

  double x1 = CGnuPlotUtil::map(p.x,
    xaxis.min().getValue(0.0), xaxis.max().getValue(1.0), -scaleX_, scaleX_);
  double y1 = CGnuPlotUtil::map(p.y,
    yaxis.min().getValue(0.0), yaxis.max().getValue(1.0), -scaleY_, scaleY_);
  double z1 = CGnuPlotUtil::map(p.z,
    zaxis.min().getValue(0.0), zaxis.max().getValue(1.0), -scaleZ_, scaleZ_);

  // transform to 2D
  CPoint3D p1(x1, y1, z1);

  CPoint3D p2 = coordFrame_.transformTo(p1);

  CPoint3D p3;

  projMatrix_.multiplyPoint(p2, p3);

  // remap back to x/y axis
  double x2 = CGnuPlotUtil::map(p3.x, -1, 1, xaxis.min().getValue(), xaxis.max().getValue());
  double y2 = CGnuPlotUtil::map(p3.y, -1, 1, yaxis.min().getValue(), yaxis.max().getValue());

  double z2 = 0.0;

  if (zaxis.min().isValid() && zaxis.max().isValid())
    z2 = CGnuPlotUtil::map(p3.z, -1, 1, zaxis.min().getValue(), zaxis.max().getValue());

  return CPoint3D(x2, y2, z2);
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
