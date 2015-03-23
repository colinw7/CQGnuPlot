#ifndef CGnuPlotBar_H
#define CGnuPlotBar_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotBar : public CGnuPlotPlotObject {
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

  bool isInitialized() const { return initialized_; }
  void setInitialized(bool b) { initialized_ = b; }

  FillType fillType() const { return fillType_; }
  void setFillType(FillType t) { fillType_ = t; }

  FillPattern fillPattern() const { return fillPattern_; }
  void setFillPattern(FillPattern t) { fillPattern_ = t; }

  const COptValT<CRGBA> &fillColor() const { return fillColor_; }
  void setFillColor(const COptValT<CRGBA> &c) { fillColor_ = c; }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

  bool hasBorder() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  const COptValT<CRGBA> &lineColor() const { return lineColor_; }
  void setLineColor(const COptValT<CRGBA> &c) { lineColor_ = c; }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  double width() const { return width_; }
  void setWidth(double r) { width_ = r; }

  bool inside(const CPoint2D &p) const;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CBBox2D         bbox_        { 0, 0, 1, 1 };
  double          value_       { 0.0 }; // associated value
  bool            initialized_ { false };
  FillType        fillType_    { FillType::EMPTY };
  FillPattern     fillPattern_ { FillPattern::NONE };
  COptValT<CRGBA> fillColor_;
  bool            border_      { true }; // border displayed
  COptValT<CRGBA> lineColor_;
  double          width_       { 0.0 }; // associated value
};

#endif
