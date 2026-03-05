#ifndef CGnuPlotCircle_H
#define CGnuPlotCircle_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotCoordValue.h>

class CGnuPlotCircle : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "circle"; }

  CGnuPlotCircle(CGnuPlotGroup *group);

  virtual ~CGnuPlotCircle() { }

  CGnuPlotCircle *setData(const CGnuPlotCircle *circle);

  ObjectType type() const override { return CGnuPlotTypes::ObjectType::CIRCLE; }

  VariableName varName() const override { return CGnuPlotTypes::VariableName::CIRCLE; }

  const CGnuPlotPosition &getCenter() const { return p_; }
  void setCenter(const CGnuPlotPosition &p) { p_ = p; }

  const CGnuPlotCoordValue &getRadius() const { return r_; }
  void setRadius(const CGnuPlotCoordValue &r) { r_ = r; }

  const std::optional<double> &arcStart() const { return arcStart_; }
  void setArcStart(double v) { arcStart_ = v; }

  const std::optional<double> &arcEnd() const { return arcEnd_; }
  void setArcEnd(double v) { arcEnd_ = v; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  const std::optional<double> &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  const CLineDash &getLineDash() const { return dash_; }
  void setLineDash(const CLineDash &dash) { dash_ = dash; }

  void initClip() override;

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }
  void setBBox(const CBBox2D &bbox) override;

  CGnuPlotTipData tip() const override;

  void print(std::ostream &os) const override;

 protected:
  CGnuPlotPosition      p_;
  CGnuPlotCoordValue    r_ { 1 };
  std::optional<double> arcStart_;
  std::optional<double> arcEnd_;
  CGnuPlotFillStyle     fs_;
  std::optional<double> lw_;
  CLineDash             dash_;
  mutable CPoint2D      center_;
  mutable double        xr_, yr_;
  mutable CRGBA         c_;
  mutable CBBox2D       bbox_;
};

typedef std::shared_ptr<CGnuPlotCircle> CGnuPlotCircleP;

#endif
