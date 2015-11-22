#include <CGnuPlotImageObject.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CImageLib.h>
#include <CRotBBox2D.h>

CGnuPlotImageObject::
CGnuPlotImageObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

void
CGnuPlotImageObject::
setSize(const CISize2D &s)
{
  if (s != size_) {
    size_ = s;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setOrigin(const CPoint3D &o)
{
  if (o != origin_) {
    origin_ = o;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setCenter(const CPoint3D &c)
{
  if (c != center_) {
    center_ = c;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setDelta(const CSize2D &d)
{
  if (d != delta_) {
    delta_ = d;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setAngle(double a)
{
  if (a != angle_) {
    angle_ = a;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setFlipX(bool b)
{
  if (b != flipX_) {
    flipX_ = b;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setFlipY(bool b)
{
  if (b != flipY_) {
    flipY_ = b;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
setNearest(bool b)
{
  if (b != nearest_) {
    nearest_ = b;

    image2_ = CImagePtr();
  }
}

void
CGnuPlotImageObject::
init() const
{
  if (! image1_.isValid()) {
    image1_ = CImageMgrInst->createImage();

    image1_->setDataSize(size_);
  }

  if (! image2_.isValid()) {
    double ra = CAngle::Deg2Rad(angle_);

    double w = size_.getWidth ()*delta_.getWidth ();
    double h = size_.getHeight()*delta_.getHeight();

    CPoint2D p1(center_.x - w/2.0, center_.y - h/2.0);
    CPoint2D p2(center_.x + w/2.0, center_.y + h/2.0);

    if (flipX_) {
      p1.x = center_.x - (p1.x - center_.x);
      p2.x = center_.x - (p2.x - center_.x);
    }

    if (flipY_) {
      p1.y = center_.y - (p1.y - center_.y);
      p2.y = center_.y - (p2.y - center_.y);
    }

    bbox_ = CBBox2D(p1, p2);

    CRotBBox2D rbbox(bbox_, ra, CPoint2D(origin_.x, origin_.y));

    rbbox_ = rbbox.boundingBox();

    //---

    size1_ = CISize2D(rbbox_.getWidth(), rbbox_.getHeight());
    size2_ = CISize2D(0, 0);

    image2_ = CImageMgrInst->createImage();
  }
}

void
CGnuPlotImageObject::
drawPoint(const CPoint2D &p, const CRGBA &c)
{
  init();

  image1_->drawRGBAPoint(p.x, p.y, c);
}

void
CGnuPlotImageObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (renderer->is3D())
    draw3D(renderer);
  else
    draw2D(renderer);
}

void
CGnuPlotImageObject::
draw2D(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  init();

  //---

  bool highlighted = (isHighlighted() || isSelected());

  CPoint2D p1, p2;

  renderer->windowToPixel(rbbox_.getLL(), p1);
  renderer->windowToPixel(rbbox_.getUR(), p2);

  CPoint2D p;

  renderer->pixelToWindow(CPoint2D(std::min(p1.x, p2.x), std::min(p1.y, p2.y)), p);
  //CPoint2D p(rbbox_.getXMin(), rbbox_.getYMax());

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  int ipw = int(size1_.width /pw + 0.5);
  int iph = int(size1_.height/ph + 0.5);

  if (size2_ != CISize2D(ipw, iph)) {
    size2_ = CISize2D(ipw, iph);

    image2_->setDataSize(size2_);

    updateImage2();
  }

  if (! isOriginal())
    renderer->drawImage(p, image2_);
  else
    renderer->drawImage(p, image1_);

  if (highlighted)
    renderer->drawRect(rbbox_, CRGBA(255,0,0), 2);
}

void
CGnuPlotImageObject::
draw3D(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  CPoint2D o(origin_.x, origin_.y);

  double ra = CAngle::Deg2Rad(angle_);

  int nx = size_.width;
  int ny = size_.height;

  double w = nx*delta_.width;
  double h = ny*delta_.height;

  CPoint2D p1(center_.x - w/2.0, center_.y - h/2.0);
  CPoint2D p2(center_.x + w/2.0, center_.y + h/2.0);

  double z = 0;

  //renderer->setAntiAlias(false);

  int i = 0;

  for (int iy = 0; iy < ny; ++iy) {
    int iy1 = (flipY_ ? ny - 1 - iy : iy);

    for (int ix = 0; ix < nx; ++ix, ++i) {
      int ix1 = (flipX_ ? nx - 1 - ix : ix);

      int i = iy1*nx + ix1;

      CRGBA rgba = colors_[i];

      double x1 = CGnuPlotUtil::map(ix, 0, nx - 1, p1.x, p2.x);
      double y1 = CGnuPlotUtil::map(iy, 0, ny - 1, p1.y, p2.y);
      double x2 = x1 + delta_.width;
      double y2 = y1 + delta_.height;

      CPoint2D pr1 = CMathGeom2D::RotatePoint(CPoint2D(x1, y1), ra, o);
      CPoint2D pr2 = CMathGeom2D::RotatePoint(CPoint2D(x2, y1), ra, o);
      CPoint2D pr3 = CMathGeom2D::RotatePoint(CPoint2D(x2, y2), ra, o);
      CPoint2D pr4 = CMathGeom2D::RotatePoint(CPoint2D(x1, y2), ra, o);

      CGnuPlotRenderer::Points2D poly({
        renderer->transform(CPoint3D(pr1.x, pr1.y, z)),
        renderer->transform(CPoint3D(pr2.x, pr2.y, z)),
        renderer->transform(CPoint3D(pr3.x, pr3.y, z)),
        renderer->transform(CPoint3D(pr4.x, pr4.y, z))
      });

      renderer->fillPolygon(poly, rgba);
    }
  }

  //renderer->setAntiAlias(true);

  bbox_  = CBBox2D(0, 0, 1, 1);
  rbbox_ = CBBox2D(0, 0, 1, 1);
}

void
CGnuPlotImageObject::
updateImage2() const
{
  CGnuPlotAxis *plotXAxis = plot_->group()->getPlotAxis(CGnuPlotTypes::AxisType::X, 1, true);
  CGnuPlotAxis *plotYAxis = plot_->group()->getPlotAxis(CGnuPlotTypes::AxisType::Y, 1, true);

  bool flipX = plotXAxis->isReverse();
  bool flipY = plotYAxis->isReverse();

  if (flipX_) flipX = ! flipX;
  if (flipY_) flipY = ! flipY;

  //---

  double ra = CAngle::Deg2Rad(angle_);

  //if (flipY) ra = -ra;

  //---

  double w = size_.getWidth ();
  double h = size_.getHeight();

  CPoint2D o(w/2, h/2);

  CRotBBox2D rbbox(CBBox2D(0, 0, w - 1, h - 1), ra, o);

  CBBox2D bbox = rbbox.boundingBox();

  double xo = bbox.getXMin();
  double yo = bbox.getYMin();

  double xscale = (size2_.width  - 1)/bbox.getWidth ();
  double yscale = (size2_.height - 1)/bbox.getHeight();

  //---

  for (int y = 0; y < size2_.height; ++y) {
    double y1 = y/yscale + yo;

    for (int x = 0; x < size2_.width; ++x) {
      double x1 = x/xscale + xo;

      CPoint2D p1 = CMathGeom2D::RotatePoint(CPoint2D(x1, y1), ra, o);

      if (flipX)
        p1.x = o.x - (p1.x - o.x);

      if (flipY)
        p1.y = o.y - (p1.y - o.y);

      CRGBA c;

      if (nearest_) {
        if (p1.x >= 0 && p1.x < size_.width && p1.y >= 0 && p1.y < size_.height)
          c = image1_->getBilinearRGBAPixel(p1.x, p1.y);
        else
          c = CRGBA(0,0,0,0);
      }
      else {
        int px = CMathGen::Round(p1.x);
        int py = CMathGen::Round(p1.y);

        if (px >= 0 && px < size_.width && py >= 0 && py < size_.height)
          image1_->getRGBAPixel(px, py, c);
        else
          c = CRGBA(0,0,0,0);
      }

      image2_->setRGBAPixel(x, y, c);
    }
  }
}

CGnuPlotTipData
CGnuPlotImageObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(tipText());
  tip.setYStr("");

  tip.setBorderColor(CRGBA(0,0,0));
  tip.setXColor     (CRGBA(0,0,0));

  tip.setBBox(rbbox_);

  return tip;
}
