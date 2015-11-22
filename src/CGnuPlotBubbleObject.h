#ifndef CGnuPlotBubbleObject_H
#define CGnuPlotBubbleObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotBubbleObject : public CGnuPlotPlotObject {
 public:
  CGnuPlotBubbleObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBubbleObject() { }

  const CPoint2D &center() const { return c_; }
  void setCenter(const CPoint2D &c) { c_ = c; }

  double xRadius() const { return xr_; }
  void setXRadius(double r) { xr_ = r; }

  double yRadius() const { return yr_; }
  void setYRadius(double r) { yr_ = r; }

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CPoint2D        c_      { 0, 0 };
  double          xr_     { 1 };
  double          yr_     { 1 };
  std::string     name_   { "" };
  double          value_  { 0 };
  CGnuPlotFillP   fill_;
  CGnuPlotStrokeP stroke_;
  mutable CBBox2D bbox_;
};

#endif
