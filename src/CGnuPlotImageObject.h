#ifndef CGnuPlotImageObject_H
#define CGnuPlotImageObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <CImageLib.h>

#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotImageObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D> Points;
  typedef std::vector<CRGBA>    Colors;

 public:
  CGnuPlotImageObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotImageObject() { }

  CGnuPlotImageObject(const CGnuPlotImageObject &) = delete;
  CGnuPlotImageObject &operator=(const CGnuPlotImageObject &) = delete;

  const CISize2D &size() const { return size_; }
  void setSize(const CISize2D &v);

  const CPoint3D &origin() const { return origin_; }
  void setOrigin(const CPoint3D &o);

  const CPoint3D &center() const { return center_; }
  void setCenter(const CPoint3D &c);

  const CSize2D &delta() const { return delta_; }
  void setDelta(const CSize2D &v);

  double angle() const { return angle_; }
  void setAngle(double r);

  bool isFlipX() const { return flipX_; }
  void setFlipX(bool b);

  bool isFlipY() const { return flipY_; }
  void setFlipY(bool b);

  bool isNearest() const { return nearest_; }
  void setNearest(bool b);

  bool isOriginal() const { return original_; }
  void setOriginal(bool b) { original_ = b; }

  const Colors &colors() const { return colors_; }
  void setColors(const Colors &v) { colors_ = v; }

  const CBBox2D &bbox() const override { return rbbox_; }
  void setBBox(const CBBox2D &v) override { rbbox_ = v; }

  const CImagePtr &image1() const { return image1_; }
  void setImage1(const CImagePtr &v) { image1_ = v; }

  void drawPoint(const CPoint2D &p, const CRGBA &c);

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

  void draw2D(CGnuPlotRenderer *renderer) const;
  void draw3D(CGnuPlotRenderer *renderer) const;

 protected:
  void init() const;

  void updateImage2() const;

 protected:
  mutable CISize2D  size_;            // pixel size
  CPoint3D          origin_;          // origin
  CPoint3D          center_;          // center
  CSize2D           delta_;           // pixel delta
  double            angle_ { 0     }; // angle
  bool              flipX_ { false }; // flip X
  bool              flipY_ { false }; // flip Y
  bool              nearest_ { false };
  bool              original_ { false };
  Colors            colors_;
  mutable CBBox2D   bbox_;            // unrotated bbox
  mutable CBBox2D   rbbox_;           // rotated bbox
  mutable CImagePtr image1_;          // pixel image
  mutable CImagePtr image2_;          // device image
  mutable CISize2D  size1_;           // expanded pixel size (delta applied)
  mutable CISize2D  size2_;           // device pixel size (pixel size on device)
};

#endif
