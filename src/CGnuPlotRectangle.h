#ifndef CGnuPlotRectangle_H
#define CGnuPlotRectangle_H

#include <CGnuPlotObject.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotRectangle : public CGnuPlotGroupAnnotation {
 public:
  typedef COptValT<CGnuPlotPosition> Position;
  typedef COptValT<CGnuPlotSize>     Size;

 public:
  static const char *getName() { return "rectangle"; }

  CGnuPlotRectangle(CGnuPlotGroup *group);

  virtual ~CGnuPlotRectangle() { }

  CGnuPlotRectangle *setData(const CGnuPlotRectangle *rect) {
    (void) CGnuPlotGroupAnnotation::setData(rect);

    from_   = rect->from_;
    to_     = rect->to_;
    rto_    = rect->rto_;
    center_ = rect->center_;
    size_   = rect->size_;
    fs_     = rect->fs_;
    lw_     = rect->lw_;
    bbox_   = rect->bbox_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::RECTANGLE; }

  const Position &getFrom() const { return from_; }
  void setFrom(const CGnuPlotPosition &p) { from_ = p; }

  const Position &getTo() const { return to_; }
  void setTo(const CGnuPlotPosition &p) { to_ = p; }

  const Position &getRTo() const { return rto_; }
  void setRTo(const CGnuPlotPosition &p) { rto_ = p; }

  const Position &getCenter() const { return center_; }
  void setCenter(const CGnuPlotPosition &p) { center_ = p; }

  const Size &getSize() const { return size_; }
  void setSize(const CGnuPlotSize &p) { size_ = p; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void setLineType(int lt) { lt_ = lt; }

  const COptReal &getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

  CBBox2D calcBBox() const;

  void draw() const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &os) const;

 protected:
  Position          from_;
  Position          to_;
  Position          rto_;
  Position          center_;
  Size              size_;
  CGnuPlotFillStyle fs_;
  COptInt           lt_;
  COptReal          lw_;
  mutable CBBox2D   bbox_;
};

typedef std::shared_ptr<CGnuPlotRectangle> CGnuPlotRectangleP;

#endif
