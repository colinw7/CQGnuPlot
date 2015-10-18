#ifndef CGnuPlotEllipseObject_H
#define CGnuPlotEllipseObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotEllipseObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  CGnuPlotEllipseObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotEllipseObject() { }

  const CPoint2D &center() const { return center_; }
  void setCenter(const CPoint2D &center);

  const CSize2D &size() const { return size_; }
  void setSize(const CSize2D &size);

  double angle() const { return angle_; }
  void setAngle(double r) { angle_ = r; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  void update();

 protected:
  CPoint2D        center_;
  CSize2D         size_;
  double          angle_ { 0 };
  std::string     text_  { "" };
  CGnuPlotFillP   fill_;
  CGnuPlotStrokeP stroke_;
  mutable CBBox2D rect_;
};

#endif
