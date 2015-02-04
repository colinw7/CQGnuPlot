#ifndef CGnuPlotArrowStyle_H
#define CGnuPlotArrowStyle_H

#include <CGnuPlotCoordValue.h>
#include <COptVal.h>
#include <CRGBA.h>

#include <memory>
#include <iostream>

class CGnuPlot;

class CGnuPlotArrowStyle {
 public:
  CGnuPlotArrowStyle() { }

  bool fhead() const { return fhead_; }
  void setFHead(bool b) { fhead_ = b; }

  bool thead() const { return thead_; }
  void setTHead(bool b) { thead_ = b; }

  void setHeads(bool fhead, bool thead) { setFHead(fhead); setTHead(thead); }

  const CGnuPlotCoordValue &length() const { return length_; }
  void setLength(const CGnuPlotCoordValue &l) { length_ = l; }

  double lengthValue() const { return length_.value(); }
  void setLengthValue(double l) { length_.setValue(l); }

  double angle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  double backAngle() const { return backAngle_; }
  void setBackAngle(double a) { backAngle_ = a; }

  bool filled() const { return filled_; }
  void setFilled(bool b) { filled_ = b; }

  bool empty() const { return empty_; }
  void setEmpty(bool b) { empty_ = b; }

  bool front() const { return front_; }
  void setFront(bool b) { front_ = b; }

  double lineWidth(CGnuPlot *plot) const;
  void setLineWidth(double w) { lineWidth_ = w; }

  CRGBA lineColor() const { return lineColor_.getValue(CRGBA(0,0,0)); }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  int lineStyle() const { return lineStyle_.getValue(-1); }
  void setLineStyle(int t) { lineStyle_ = t; }

  bool variable() const { return variable_; }
  void setVariable(bool b) { variable_ = b; }

  const COptReal &varValue() const { return varValue_; }
  void setVarValue(double r) { varValue_ = r; }

  void print(CGnuPlot *plot, std::ostream &os) const;

 private:
  std::string headStr() const {
    if      (! fhead_ && ! thead_) return "nohead";
    else if (! fhead_ &&   thead_) return "head";
    else if (  fhead_ && ! thead_) return "backhead";
    else                           return "heads";
  }

  std::string frontStr() const {
    return (front_ ? "front" : "back");
  }

  std::string filledStr() const {
    if      (filled_ && empty_) return "empty";
    else if (filled_          ) return "filled";
    else                        return "nofilled";
  }

 private:
  bool               fhead_     { false };
  bool               thead_     { true };
  CGnuPlotCoordValue length_    { -1 };
  double             angle_     { -1 };
  double             backAngle_ { -1 };
  bool               filled_    { false };
  bool               empty_     { false };
  bool               front_     { false };
  COptReal           lineWidth_;
  COptInt            lineStyle_;
  COptValT<CRGBA>    lineColor_;
  bool               variable_  { false };
  COptReal           varValue_;
};

typedef std::shared_ptr<CGnuPlotArrowStyle> CGnuPlotArrowStyleP;

#endif
