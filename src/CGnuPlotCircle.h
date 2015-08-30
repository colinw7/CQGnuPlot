#ifndef CGnuPlotCircle_H
#define CGnuPlotCircle_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotCoordValue.h>

class CGnuPlotCircle : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "circle"; }

  CGnuPlotCircle(CGnuPlotGroup *group) :
   CGnuPlotGroupAnnotation(group) {
  }

  virtual ~CGnuPlotCircle() { }

  CGnuPlotCircle *setData(const CGnuPlotCircle *ellipse) {
    (void) CGnuPlotGroupAnnotation::setData(ellipse);

    p_        = ellipse->p_;
    r_        = ellipse->r_;
    arcStart_ = ellipse->arcStart_;
    arcEnd_   = ellipse->arcEnd_;
    fs_       = ellipse->fs_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::CIRCLE; }

  const CGnuPlotPosition &getCenter() const { return p_; }
  void setCenter(const CGnuPlotPosition &p) { p_ = p; }

  const CGnuPlotCoordValue &getRadius() const { return r_; }
  void setRadius(const CGnuPlotCoordValue &r) { r_ = r; }

  const COptReal &arcStart() const { return arcStart_; }
  void setArcStart(double v) { arcStart_ = v; }

  const COptReal &arcEnd() const { return arcEnd_; }
  void setArcEnd(double v) { arcEnd_ = v; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CPoint2D &p) const override;

  CGnuPlotTipData tip() const override { return CGnuPlotTipData(); }

  void print(std::ostream &os) const;

 protected:
  CGnuPlotPosition   p_;
  CGnuPlotCoordValue r_;
  COptReal           arcStart_;
  COptReal           arcEnd_;
  CGnuPlotFillStyle  fs_;
  mutable CPoint2D   c_;
  mutable double     xr_, yr_;
};

typedef std::shared_ptr<CGnuPlotCircle> CGnuPlotCircleP;

#endif
