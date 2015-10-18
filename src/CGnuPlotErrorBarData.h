#ifndef CGnuPlotErrorBarData_H
#define CGnuPlotErrorBarData_H

#include <CGnuPlotTypes.h>

class CGnuPlotRenderer;

class CGnuPlotErrorBarData {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotErrorBarData() { }

  double x() const { return x_; }
  void setX(double r) { x_ = r; }

  double y() const { return y_; }
  void setY(double r) { y_ = r; }

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

  double pointSize() const { return pointSize_; }
  void setPointSize(double r) { pointSize_ = r; }

  const SymbolType &pointType() const { return pointType_; }
  void setPointType(const SymbolType &v) { pointType_ = v; }

  const CRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const CRGBA &v) { lineColor_ = v; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  CPoint2D point() const { return CPoint2D(x_, y_ ); }

  CPoint2D xLowPoint () const { return CPoint2D(xl_, y_); }
  CPoint2D xHighPoint() const { return CPoint2D(xh_, y_); }
  CPoint2D yLowPoint () const { return CPoint2D(x_, yl_); }
  CPoint2D yHighPoint() const { return CPoint2D(x_, yh_); }

  void draw(CGnuPlotRenderer *renderer) const;

 private:
  double     x_         { 0 };
  double     y_         { 0 };
  bool       xdir_      { false };
  bool       ydir_      { false };
  double     xl_        { 0 };
  double     xh_        { 0 };
  double     yl_        { 0 };
  double     yh_        { 0 };
  double     barWidth_  { 0 };
  double     barHeight_ { 0 };
  double     pointSize_ { 0 };
  SymbolType pointType_;
  CRGBA      lineColor_;
  double     lineWidth_ { 0 };
};

#endif
