#ifndef CGnuPlotColorSpec_H
#define CGnuPlotColorSpec_H

#include <CGnuPlotStyle.h>

class CGnuPlotPlot;

class CGnuPlotColorSpec {
 public:
  enum class Type {
    NONE,
    RGB,
    VARIABLE,
    RGB_VARIABLE,
    PALETTE_FRAC,
    PALETTE_CB,
    PALETTE_Z,
    INDEX
  };

  CGnuPlotColorSpec() { }

  Type type() const { return type_; }

  void setRGB(const CRGBA &c) {
    type_ = Type::RGB;
    c_    = c;
  }

  bool isRGB        () const { return type_ == Type::RGB         ; }
  bool isVariable   () const { return type_ == Type::VARIABLE    ; }
  bool isRGBVariable() const { return type_ == Type::RGB_VARIABLE; }
  bool isPaletteFrac() const { return type_ == Type::PALETTE_FRAC; }
  bool isPaletteCB  () const { return type_ == Type::PALETTE_CB  ; }
  bool isPaletteZ   () const { return type_ == Type::PALETTE_Z   ; }
  bool isIndex      () const { return type_ == Type::INDEX       ; }

  void setVariable() {
    type_ = Type::VARIABLE;
    c_    = CRGBA(0,0,0);
  }

  void setRGBVariable() {
    type_ = Type::RGB_VARIABLE;
    c_    = CRGBA(0,0,0);
  }

  void setPaletteFrac(double r) {
    type_ = Type::PALETTE_FRAC;
    r_    = r;
    c_    = CRGBA(0,0,0);
  }

  void setPaletteCB(double r) {
    type_ = Type::PALETTE_CB;
    r_    = r;
    c_    = CRGBA(0,0,0);
  }

  void setPaletteZ() {
    type_ = Type::PALETTE_Z;
    r_    = 0.0;
    c_    = CRGBA(0,0,0);
  }

  int index() const { return i_; }

  void setIndex(int i) {
    type_ = Type::INDEX;
    i_    = i;
    c_    = CGnuPlotStyle::instance()->indexColor(i_);
  }

  const CRGBA &color() const { return c_; }

  bool isCalc() const {
    return isVariable() || isRGBVariable() || isPaletteZ();
  }

  CRGBA calcColor(CGnuPlotPlot *plot, double x) const;

 private:
  Type   type_ { Type::NONE };
  CRGBA  c_    { CRGBA(0,0,0) };
  int    i_    { 0 };
  double r_    { 0.0 };
};

#endif
