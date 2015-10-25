#ifndef CGnuPlotImageObject_H
#define CGnuPlotImageObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <CImagePtr.h>

#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotImageObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  CGnuPlotImageObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotImageObject() { }

  CGnuPlotImageObject(const CGnuPlotImageObject &) = delete;
  CGnuPlotImageObject &operator=(const CGnuPlotImageObject &) = delete;

  const CImagePtr &image1() const { return image1_; }
  void setImage1(const CImagePtr &v) { image1_ = v; }

  const CISize2D &size() const { return size1_; }
  void setSize(const CISize2D &v);

  const CBBox2D &bbox() const override { return bbox_; }
  void setBBox(const CBBox2D &v) override { bbox_ = v; }

  double angle() const { return angle1_; }
  void setAngle(double r) { angle1_ = r; }

  bool isFlipY() const { return flipY1_; }
  void setFlipY(bool b) { flipY1_ = b; }

  void drawPoint(const CPoint2D &p, const CRGBA &c);

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  void updateImage2() const;

 protected:
  CImagePtr         image1_;           // image
  CISize2D          size1_;            // pixel size
  double            angle1_ { 0     }; // angle
  bool              flipY1_ { false }; // flip Y
  CBBox2D           bbox_;             // data size
  mutable CImagePtr image2_;
  mutable CISize2D  size2_;
  mutable double    angle2_ { 0 };
  mutable bool      flipY2_ { false };
};

#endif
