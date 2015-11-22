#ifndef CGnuPlotPolygon_H
#define CGnuPlotPolygon_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotPlot;

class CGnuPlotPolygon : public CGnuPlotGroupAnnotation {
 public:
  typedef std::vector<CPoint3D> Points3D;
  typedef std::vector<CPoint2D> Points2D;

 public:
  static const char *getName() { return "polygon"; }

  CGnuPlotPolygon(CGnuPlotGroup *group);

  virtual ~CGnuPlotPolygon() { }

  CGnuPlotPolygon *setData(const CGnuPlotPolygon *poly);

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::POLYGON; }

  void addPoint (const CGnuPlotPosition &p);
  void addRPoint(const CGnuPlotPosition &p);

  const Points3D &getPolyPoints3D() const { return ppoints3D_; }
  const Points2D &getPolyPoints2D() const { return ppoints2D_; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void setLineType(int lt) { lt_ = lt; }

  const COptReal &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  const CLineDash &dash() const { return dash_; }
  void setDash(const CLineDash &v) { dash_ = v; }

  CBBox2D calcBBox() const;

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void initClip() override;

  void print(std::ostream &) const override;

 protected:
  void calcPoints(CGnuPlotRenderer *renderer) const;

 protected:
  enum class PointType {
    ABSOLUTE,
    RELATIVE
  };

  typedef std::pair<PointType,CGnuPlotPosition> TypePosition;
  typedef std::vector<TypePosition>             TypePositions;

  TypePositions     typePositions_;
  CGnuPlotFillStyle fs_;
  COptInt           lt_;
  COptReal          lw_;
  CLineDash         dash_;
  mutable CBBox2D   bbox_;
  mutable CRGBA     fc_;
  mutable CRGBA     lc_;
  mutable Points3D  ppoints3D_;
  mutable Points2D  ppoints2D_;
};

typedef std::shared_ptr<CGnuPlotPolygon> CGnuPlotPolygonP;

#endif
