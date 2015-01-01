#ifndef CGnuPlotLineStyle_H
#define CGnuPlotLineStyle_H

#include <CRGBA.h>
#include <COptVal.h>
#include <CRefPtr.h>

namespace CGnuPlotTypes {
  enum class SymbolType {
    NONE,
    POINT,
    PLUS,
    CROSS,
    STAR,
    BOX,
    FILLED_BOX,
    CIRCLE,
    FILLED_CIRCLE,
    TRIANGLE,
    FILLED_TRIANGLE,
    INV_TRIANGLE,
    FILLED_INV_TRIANGLE,
    DIAMOND,
    FILLED_DIAMOND,
    LAST=FILLED_DIAMOND
  };
}

class CGnuPlotLineStyle {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotLineStyle() { }

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  int type() const { return type_; }
  void setType(int type) { type_ = type; }

  double width() const { return width_; }
  void setWidth(double width) { width_ = width; }

  CRGBA color(const CRGBA &c=CRGBA(0,0,0)) const { return color_.getValue(c); }
  void setColor(const CRGBA &c) { color_ = c; }

  SymbolType pointType() const { return pointType_; }
  void setPointType(SymbolType type) { pointType_ = type; }

  double pointSize() const { return pointSize_; }
  void setPointSize(double s) { pointSize_ = s; }

 private:
  int             ind_       { -1 };
  int             type_      { 1 };
  double          width_     { 1 };
  COptValT<CRGBA> color_;
  SymbolType      pointType_ { SymbolType::PLUS };
  double          pointSize_ { 1 };
};

typedef CRefPtr<CGnuPlotLineStyle> CGnuPlotLineStyleP;

#endif
