#ifndef CGnuPlotArrowStyle_H
#define CGnuPlotArrowStyle_H

#include <CGnuPlotCoordValue.h>
#include <CGnuPlotDash.h>
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
  void setLength(double l) { length_ = l; }

  double angle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  double backAngle() const { return backAngle_; }
  void setBackAngle(double a) { backAngle_ = a; }

  bool filled() const { return filled_; }
  void setFilled(bool b) { filled_ = b; }

  bool fixed() const { return fixed_; }
  void setFixed(bool b) { fixed_ = b; }

  bool empty() const { return empty_; }
  void setEmpty(bool b) { empty_ = b; }

  bool border() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  bool front() const { return front_; }
  void setFront(bool b) { front_ = b; }

  double lineWidth(CGnuPlot *plot=0) const;
  void setLineWidth(double w) { lineWidth_ = w; }

  const COptRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  const COptInt &lineStyle() const { return lineStyle_; }
  void setLineStyle(int t) { lineStyle_ = t; }

  const CGnuPlotDash &dash() const { return dash_; }
  void setDash(const CGnuPlotDash &d) { dash_ = d; }

  bool isVariable() const { return variable_; }
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
  bool               border_    { true  };
  bool               front_     { false };
  bool               fixed_     { false }; // no auto size reduce for show arrows
  COptReal           lineWidth_;
  COptInt            lineStyle_;
  COptRGBA           lineColor_;
  CGnuPlotDash       dash_;
  bool               variable_  { false };
  COptReal           varValue_;
};

typedef std::shared_ptr<CGnuPlotArrowStyle> CGnuPlotArrowStyleP;

#endif
