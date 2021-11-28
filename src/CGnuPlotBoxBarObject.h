#ifndef CGnuPlotBoxBarObject_H
#define CGnuPlotBoxBarObject_H

#include <CPoint2D.h>
#include <string>

#include <CGnuPlotObject.h>
#include <CGnuPlotEndBar.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotFill.h>

class CGnuPlotPlot;

class CGnuPlotBoxBarObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CGnuPlotEndBarP> EndBars;

 public:
  CGnuPlotBoxBarObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBoxBarObject() { }

  CGnuPlotBoxBarObject(const CGnuPlotBoxBarObject &) = delete;
  CGnuPlotBoxBarObject &operator=(const CGnuPlotBoxBarObject &) = delete;

  const CBBox2D &bbox() const override { return bbox_; }
  void setBBox(const CBBox2D &b) override { bbox_ = b; }

  double xvalue() const { return x_; }
  double yvalue() const { return y_; }
  void setValues(double x, double y) { x_ = x; y_ = y; }

  void setXValueStr(const std::string &s) { x_str_ = s; }
  void setYValueStr(const std::string &s) { y_str_ = s; }

  bool isVertical() const { return vertical_; }
  void setVertical(bool b) { vertical_ = b; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  void clearEndBars();

  CGnuPlotEndBarP addEndBar(const CPoint2D &start, const CPoint2D &end);

  const EndBars &endBars() const { return endBars_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CBBox2D         bbox_     { 0, 0, 1, 1 };
  double          x_        { 0.0 }; // associated x value
  double          y_        { 0.0 }; // associated y value
  bool            vertical_ { true };
  std::string     x_str_;
  std::string     y_str_;
  CGnuPlotFillP   fill_;
  CGnuPlotStrokeP stroke_;
  EndBars         endBars_;
};

#endif
