#ifndef CGnuPlotEllipse_H
#define CGnuPlotEllipse_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotFillStyle.h>

class CGnuPlotEllipse : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "ellipse"; }

  CGnuPlotEllipse(CGnuPlotGroup *group) :
   CGnuPlotGroupAnnotation(group) {
  }

  virtual ~CGnuPlotEllipse() { }

  CGnuPlotEllipse *setData(const CGnuPlotEllipse *ellipse) {
    (void) CGnuPlotGroupAnnotation::setData(ellipse);

    p_  = ellipse->p_;
    rx_ = ellipse->rx_;
    ry_ = ellipse->ry_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::ELLIPSE; }

  const CGnuPlotPosition &getCenter() const { return p_; }
  void setCenter(const CGnuPlotPosition &p) { p_ = p; }

  double getRX() const { return rx_; }
  void setRX(double x) { rx_ = x; }

  double getRY() const { return ry_; }
  void setRY(double y) { ry_ = y; }

  double getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  const CGnuPlotFillStyle &getFillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &fs) { fs_ = fs; }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &os) const;

 protected:
  CGnuPlotPosition  p_;
  double            rx_    { 1 };
  double            ry_    { 1 };
  double            angle_ { 0 };
  CGnuPlotFillStyle fs_;
};

typedef std::shared_ptr<CGnuPlotEllipse> CGnuPlotEllipseP;

#endif
