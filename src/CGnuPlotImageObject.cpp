#include <CGnuPlotImageObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CImageLib.h>

CGnuPlotImageObject::
CGnuPlotImageObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

void
CGnuPlotImageObject::
setSize(const CISize2D &s)
{
  size1_ = s;
  size2_ = CISize2D(0, 0);

  image1_ = CImageMgrInst->createImage();
  image2_ = CImageMgrInst->createImage();

  image1_->setDataSize(size1_);
}

void
CGnuPlotImageObject::
drawPoint(const CPoint2D &p, const CRGBA &c)
{
  image1_->drawRGBAPoint(p.x, p.y, c);
}

void
CGnuPlotImageObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CPoint2D p(bbox_.getXMin(), bbox_.getYMax());

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  int ipw = int(size1_.width /pw + 0.5);
  int iph = int(size1_.height/ph + 0.5);

  if (flipY1_ != flipY2_ || ipw != size2_.width || iph != size2_.height ||
      fabs(angle1_ - angle2_) > 1E-6) {
    size2_  = CISize2D(ipw, iph);
    angle2_ = angle1_;
    flipY2_ = flipY1_;

    image2_->setDataSize(size2_);

    updateImage2();
  }

  renderer->drawImage(p, image2_);

  if (highlighted)
    renderer->drawRect(bbox_, CRGBA(255,0,0), 2);
}

void
CGnuPlotImageObject::
updateImage2() const
{
  double ra = CAngle::Deg2Rad(angle1_);

  CPoint2D o(size1_.width/2, size1_.height/2);

  CPoint2D p1 = CMathGeom2D::RotatePoint(CPoint2D(0               , 0                ), ra, o);
  CPoint2D p2 = CMathGeom2D::RotatePoint(CPoint2D(size1_.width - 1, 0                ), ra, o);
  CPoint2D p3 = CMathGeom2D::RotatePoint(CPoint2D(size1_.width - 1, size1_.height - 1), ra, o);
  CPoint2D p4 = CMathGeom2D::RotatePoint(CPoint2D(0               , size1_.height - 1), ra, o);

  CBBox2D bbox(p1, p2); bbox.add(p3); bbox.add(p4);

  double xo = bbox.getXMin();
  double yo = bbox.getXMin();

  double xscale = (size2_.width  - 1)/bbox.getWidth ();
  double yscale = (size2_.height - 1)/bbox.getHeight();

  // x = (x1 - xo)*xscale
  // y = (y1 - yo)*yscale

  for (int y = 0; y < size2_.height; ++y) {
    double y1 = y/yscale + yo;

    for (int x = 0; x < size2_.width; ++x) {
      double x1 = x/xscale + xo;

      CPoint2D p1 = CMathGeom2D::RotatePoint(CPoint2D(x1, y1), -ra, o);

      CRGBA c;

      if (p1.x >= 0 && p1.x < size1_.width && p1.y >= 0 && p1.y < size1_.height) {
        if (flipY2_)
          p1.y = size1_.height - 1 - p1.y;

        image1_->getRGBAPixel(p1.x, p1.y, c);
      }
      else
        c = CRGBA(0,0,0,0);

      image2_->setRGBAPixel(x, y, c);
    }
  }
}

CGnuPlotTipData
CGnuPlotImageObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr("");
  tip.setYStr("");

  tip.setBorderColor(CRGBA(0,0,0));
  tip.setXColor     (CRGBA(0,0,0));

  tip.setBBox(bbox_);

  return tip;
}
