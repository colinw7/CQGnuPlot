#ifndef CGnuPlotBarObject_H
#define CGnuPlotBarObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotFill.h>

class CGnuPlotPlot;
class CGnuPlotBarObject;

class CGnuPlotEndBar {
 public:
  CGnuPlotEndBar(CGnuPlotPlot *plot);

  virtual ~CGnuPlotEndBar() { }

  CGnuPlotEndBar(const CGnuPlotEndBar &) = delete;
  const CGnuPlotEndBar &operator=(const CGnuPlotEndBar &) = delete;

  void setBarObject(CGnuPlotBarObject *bar) { bar_ = bar; }

  const CPoint2D &start() const { return start_; }
  void setStart(const CPoint2D &v) { start_ = v; }

  const CPoint2D &end() const { return end_; }
  void setEnd(const CPoint2D &v) { end_ = v; }

  double endWidth() const { return endWidth_; }
  void setEndWidth(double r) { endWidth_ = r; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  virtual void draw(CGnuPlotRenderer *renderer) const;

 protected:
  CGnuPlotPlot*      plot_      { 0 };
  CGnuPlotBarObject* bar_       { 0 };
  CPoint2D           start_;
  CPoint2D           end_;
  double             endWidth_  { 0 };
  CGnuPlotStrokeP    stroke_;
};

typedef CRefPtr<CGnuPlotEndBar> CGnuPlotEndBarP;

//---

class CGnuPlotBarObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CGnuPlotEndBarP> EndBars;

 public:
  CGnuPlotBarObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBarObject() { }

  CGnuPlotBarObject(const CGnuPlotBarObject &) = delete;
  const CGnuPlotBarObject &operator=(const CGnuPlotBarObject &) = delete;

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; }

  double xvalue() const { return x_; }
  double yvalue() const { return y_; }
  void setValues(double x, double y) { x_ = x; y_ = y; }

  void setXValueStr(const std::string &s) { x_str_ = s; }
  void setYValueStr(const std::string &s) { y_str_ = s; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  void clearEndBars();

  CGnuPlotEndBarP addEndBar(const CPoint2D &start, const CPoint2D &end);

  const EndBars &endBars() const { return endBars_; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CBBox2D         bbox_  { 0, 0, 1, 1 };
  double          x_     { 0.0 }; // associated x value
  double          y_     { 0.0 }; // associated y value
  std::string     x_str_;
  std::string     y_str_;
  CGnuPlotFillP   fill_;
  CGnuPlotStrokeP stroke_;
  EndBars         endBars_;
};

#endif
