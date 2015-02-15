#ifndef CGnuPlotPie_H
#define CGnuPlotPie_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotPie : CGnuPlotPlotObject {
 public:
  CGnuPlotPie(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPie() { }

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

  bool hasColor() const { return color_.isValid(); }

  const CRGBA &color() const { return color_.getValue(); }
  void setColor(const CRGBA &c) { color_ = c; }

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  bool inside(const CPoint2D &p) const;

  void draw() const override;

 private:
  bool            used_   { false };
  CPoint2D        c_      { 0, 0 };
  double          r_      { 1 };
  double          angle1_ { 0 };
  double          angle2_ { 360 };
  COptValT<CRGBA> color_;
  std::string     name_   { "" };
};

#endif
