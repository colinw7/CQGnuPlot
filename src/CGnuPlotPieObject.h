#ifndef CGnuPlotPieObject_H
#define CGnuPlotPieObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotPieObject : public CGnuPlotPlotObject {
 public:
  CGnuPlotPieObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPieObject() { }

  const CPoint2D &center() const { return center_; }
  void setCenter(const CPoint2D &c) { center_ = c; }

  double radius() const { return r_; }
  void setRadius(double r) { r_ = r; }

  double angle1() const { return angle1_; }
  void setAngle1(double a) { angle1_ = a; }

  double angle2() const { return angle2_; }
  void setAngle2(double a) { angle2_ = a; }

  double innerRadius() const { return innerRadius_; }
  void setInnerRadius(double r) { innerRadius_ = r; }

  double labelRadius() const { return labelRadius_; }
  void setLabelRadius(double r) { labelRadius_ = r; }

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  bool isExploded() const { return exploded_; }
  void setExploded(bool b) { exploded_ = b; }

  const CBBox2D &keyRect() const { return keyRect_; }
  void setKeyRect(const CBBox2D &r) { keyRect_ = r; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  bool keyInside(const CPoint2D &p) const;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CPoint2D        center_      { 0, 0 };
  double          r_           { 1 };
  double          angle1_      { 0 };
  double          angle2_      { 360 };
  double          innerRadius_ { 0.0 };
  double          labelRadius_ { 0.5 };
  std::string     name_        { "" };
  double          value_       { 0 };
  bool            exploded_    { false };
  CBBox2D         keyRect_;
  CGnuPlotFillP   fill_;
  CGnuPlotStrokeP stroke_;
  mutable CBBox2D bbox_;
};

#endif
