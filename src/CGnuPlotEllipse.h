#ifndef CGnuPlotEllipse_H
#define CGnuPlotEllipse_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotEllipse : public CGnuPlotGroupAnnotation {
 public:
  typedef CGnuPlotTypes::EllipseUnits EllipseUnits;

 public:
  static const char *getName() { return "ellipse"; }

  CGnuPlotEllipse(CGnuPlotGroup *group);

  virtual ~CGnuPlotEllipse() { }

  CGnuPlotEllipse *setData(const CGnuPlotEllipse *ellipse);

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::ELLIPSE; }

  const CGnuPlotPosition &getCenter() const { return p_; }
  void setCenter(const CGnuPlotPosition &p) { p_ = p; }

  double getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  const CGnuPlotSize &getSize() const { return size_; }
  void setSize(const CGnuPlotSize &s) { size_ = s; }

  const EllipseUnits &units() const { return units_; }
  void setUnits(const EllipseUnits &v) { units_ = v; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  const COptReal &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  void initClip() override;

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void print(std::ostream &os) const override;

 protected:
  CGnuPlotPosition  p_;
  double            angle_ { 0 };
  CGnuPlotSize      size_;
  EllipseUnits      units_ { EllipseUnits::XY };
  CGnuPlotFillStyle fs_;
  COptReal          lw_;
  mutable CPoint2D  center_;
  mutable double    xr_, yr_;
  mutable CRGBA     c_;
  mutable CBBox2D   bbox_;
};

typedef std::shared_ptr<CGnuPlotEllipse> CGnuPlotEllipseP;

#endif
