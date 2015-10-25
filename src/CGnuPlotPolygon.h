#ifndef CGnuPlotPolygon_H
#define CGnuPlotPolygon_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotPlot;

class CGnuPlotPolygon : public CGnuPlotGroupAnnotation {
 public:
  typedef std::vector<CPoint3D> Points;

 public:
  static const char *getName() { return "polygon"; }

  CGnuPlotPolygon(CGnuPlotGroup *group);

  virtual ~CGnuPlotPolygon() { }

  CGnuPlotPolygon *setData(const CGnuPlotPolygon *poly) {
    (void) CGnuPlotGroupAnnotation::setData(poly);

    points_ = poly->points_;
    fs_     = poly->fs_;
    lw_     = poly->lw_;
    bbox_   = poly->bbox_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::POLYGON; }

  void addPoint(const CPoint3D &p) { points_.push_back(p); }

  const Points &getPoints() const { return points_; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void setLineType(int lt) { lt_ = lt; }

  const COptReal &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  CBBox2D calcBBox() const;

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  const CBBox2D &bbox() const { return bbox_; }

  CGnuPlotTipData tip() const override;

  void print(std::ostream &) const;

 protected:
  Points            points_;
  CGnuPlotFillStyle fs_;
  COptInt           lt_;
  COptReal          lw_;
  mutable CBBox2D   bbox_;
  mutable CRGBA     fc_;
  mutable CRGBA     lc_;
};

typedef std::shared_ptr<CGnuPlotPolygon> CGnuPlotPolygonP;

#endif
