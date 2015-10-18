#ifndef CGnuPlotErrorBarObject_H
#define CGnuPlotErrorBarObject_H

#include <CGnuPlotObject.h>
#include <CGnuPlotEndBar.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotMark.h>

class CGnuPlotPlot;

class CGnuPlotErrorBarObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotErrorBarObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotErrorBarObject() { }

  CGnuPlotErrorBarObject(const CGnuPlotErrorBarObject &) = delete;
  const CGnuPlotErrorBarObject &operator=(const CGnuPlotErrorBarObject &) = delete;

  double x() const { return x_; }
  void setX(double x) { x_ = x; }

  double y() const { return y_; }
  void setY(double y) { y_ = y; }

  bool isXDirection() const { return xdir_; }
  void setXDirection(bool b) { xdir_ = b; }

  bool isYDirection() const { return ydir_; }
  void setYDirection(bool b) { ydir_ = b; }

  double xLow() const { return xl_; }
  void setXLow(double r) { xl_ = r; }

  double xHigh() const { return xh_; }
  void setXHigh(double r) { xh_ = r; }

  double yLow() const { return yl_; }
  void setYLow(double r) { yl_ = r; }

  double yHigh() const { return yh_; }
  void setYHigh(double r) { yh_ = r; }

  double barWidth() const { return barWidth_; }
  void setBarWidth(double r) { barWidth_ = r; }

  double barHeight() const { return barHeight_; }
  void setBarHeight(double r) { barHeight_ = r; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  const CGnuPlotMarkP &mark() const { return mark_; }
  void setMark(const CGnuPlotMarkP &s) { mark_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  double          x_         { 0.0 };
  double          y_         { 0.0 };
  bool            xdir_      { false };
  bool            ydir_      { false };
  double          xl_        { 0 };
  double          xh_        { 0 };
  double          yl_        { 0 };
  double          yh_        { 0 };
  double          barWidth_  { 0 };
  double          barHeight_ { 0 };
  CGnuPlotStrokeP stroke_;
  CGnuPlotMarkP   mark_;
  mutable CBBox2D bbox_;
};

#endif
