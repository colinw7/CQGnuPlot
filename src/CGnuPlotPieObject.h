#ifndef CGnuPlotPie_H
#define CGnuPlotPie_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotPieObject : public CGnuPlotPlotObject {
 public:
  CGnuPlotPieObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPieObject() { }

  bool isUsed() const { return used_; }
  void setUsed(bool b) { used_ = b; }

  const CPoint2D &center() const { return c_; }
  void setCenter(const CPoint2D &c) { c_ = c; }

  double radius() const { return r_; }
  void setRadius(double r) { r_ = r; }

  double angle1() const { return angle1_; }
  void setAngle1(double a) { angle1_ = a; }

  double angle2() const { return angle2_; }
  void setAngle2(double a) { angle2_ = a; }

  bool hasColor() const { return lineColor_.isValid(); }

  const CRGBA &lineColor() const { return lineColor_.getValue(); }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  const CRGBA &fillColor() const { return fillColor_.getValue(); }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  bool isExploded() const { return exploded_; }
  void setExploded(bool b) { exploded_ = b; }

  bool inside(const CPoint2D &p) const override;

  std::string tip() const override { return name_; }

  CBBox2D tipRect() const { CPoint2D d(r_, r_); return CBBox2D(c_ - d, c_ + d); }

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  bool        used_     { false };
  CPoint2D    c_        { 0, 0 };
  double      r_        { 1 };
  double      angle1_   { 0 };
  double      angle2_   { 360 };
  COptRGBA    lineColor_;
  COptRGBA    fillColor_;
  std::string name_     { "" };
  bool        exploded_ { false };
};

#endif
