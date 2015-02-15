#ifndef CGnuPlotBubble_H
#define CGnuPlotBubble_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotBubble : CGnuPlotPlotObject {
 public:
  CGnuPlotBubble(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBubble() { }

  const CPoint2D &center() const { return c_; }
  void setCenter(const CPoint2D &c) { c_ = c; }

  double xRadius() const { return xr_; }
  void setXRadius(double r) { xr_ = r; }

  double yRadius() const { return yr_; }
  void setYRadius(double r) { yr_ = r; }

  bool hasColor() const { return color_.isValid(); }

  const CRGBA &color() const { return color_.getValue(); }
  void setColor(const CRGBA &c) { color_ = c; }

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  bool inside(const CPoint2D &p) const;

  void draw() const override;

 private:
  CPoint2D        c_    { 0, 0 };
  double          xr_   { 1 };
  double          yr_   { 1 };
  COptValT<CRGBA> color_;
  std::string     name_ { "" };
};

#endif
