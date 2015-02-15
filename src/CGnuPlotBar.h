#ifndef CGnuPlotBar_H
#define CGnuPlotBar_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotBar : CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::FillType    FillType;
  typedef CGnuPlotTypes::FillPattern FillPattern;

 public:
  CGnuPlotBar(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBar() { }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  FillType fillType() const { return fillType_; }
  void setFillType(FillType t) { fillType_ = t; }

  FillPattern fillPattern() const { return fillPattern_; }
  void setFillPattern(FillPattern t) { fillPattern_ = t; }

  bool hasFillColor() const { return fillColor_.isValid(); }
  void resetFillColor() { fillColor_.setInvalid(); }

  CRGBA fillColor() const { return fillColor_.getValue(CRGBA(1,0,0)); }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

  bool hasLineColor() const { return lineColor_.isValid(); }
  void resetLineColor() { lineColor_.setInvalid(); }

  CRGBA lineColor() const { return lineColor_.getValue(CRGBA(0,0,0)); }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  bool inside(const CPoint2D &p) const;

  void draw() const override;

 private:
  CBBox2D         bbox_ { 0, 0, 1, 1 };
  FillType        fillType_ { FillType::EMPTY };
  FillPattern     fillPattern_ { FillPattern::NONE };
  COptValT<CRGBA> fillColor_;
  COptValT<CRGBA> lineColor_;
  double          value_ { 0.0} ;
};

#endif
