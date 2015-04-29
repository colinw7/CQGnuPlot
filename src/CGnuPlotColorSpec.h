#ifndef CGnuPlotColorSpec_H
#define CGnuPlotColorSpec_H

#include <CGnuPlotStyle.h>

class CGnuPlotPlot;

class CGnuPlotColorSpec {
 public:
  enum class Type {
    NONE,
    BACKGROUND,
    RGB,
    VARIABLE,
    RGB_VARIABLE,
    PALETTE_FRAC,
    PALETTE_CB,
    PALETTE_Z,
    PALETTE_VARIABLE,
    INDEX
  };

  CGnuPlotColorSpec() { }

  bool isValid() const { return (type_ != Type::NONE); }

  Type type() const { return type_; }

  bool isBackground     () const { return type_ == Type::BACKGROUND      ; }
  bool isRGB            () const { return type_ == Type::RGB             ; }
  bool isVariable       () const { return type_ == Type::VARIABLE        ; }
  bool isRGBVariable    () const { return type_ == Type::RGB_VARIABLE    ; }
  bool isPaletteFrac    () const { return type_ == Type::PALETTE_FRAC    ; }
  bool isPaletteCB      () const { return type_ == Type::PALETTE_CB      ; }
  bool isPaletteZ       () const { return type_ == Type::PALETTE_Z       ; }
  bool isPaletteVariable() const { return type_ == Type::PALETTE_VARIABLE; }
  bool isIndex          () const { return type_ == Type::INDEX           ; }

  void setBackground() {
    type_ = Type::BACKGROUND;
    c_    = CRGBA(1,1,1);
  }

  void setRGB(const CRGBA &c) {
    type_ = Type::RGB;
    c_    = c;
  }

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

  void setPaletteVariable() {
    type_ = Type::PALETTE_VARIABLE;
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
    return isVariable() || isRGBVariable() || isPaletteZ() || isPaletteVariable();
  }

  CRGBA calcColor(CGnuPlotPlot *plot, double x=0.0) const;

  void reset();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotColorSpec &c) {
    c.print(os);

    return os;
  }

 private:
  Type   type_ { Type::NONE };
  CRGBA  c_    { CRGBA(0,0,0) };
  int    i_    { 0 };
  double r_    { 0.0 };
};

#endif
