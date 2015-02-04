#ifndef CGnuPlotColorSpec_H
#define CGnuPlotColorSpec_H

#include <CGnuPlotStyle.h>

class CGnuPlotColorSpec {
 public:
  enum class Type {
    NONE,
    RGB,
    VARIABLE,
    PALETTE,
    INDEX
  };

  CGnuPlotColorSpec() { }

  Type type() const { return type_; }

  void setRGB(const CRGBA &c) {
    type_ = Type::RGB;
    c_    = c;
  }

  bool isRGB     () const { return type_ == Type::RGB     ; }
  bool isVariable() const { return type_ == Type::VARIABLE; }
  bool isPalette () const { return type_ == Type::PALETTE ; }
  bool isIndex   () const { return type_ == Type::INDEX   ; }

  void setVariable() {
    type_ = Type::VARIABLE;
    c_    = CRGBA(0,0,0);
  }

  void setPalette() {
    type_ = Type::PALETTE;
    c_    = CRGBA(0,0,0);
  }

  int index() const { return i_; }

  void setIndex(int i) {
    type_ = Type::INDEX;
    i_    = i;
    c_    = CGnuPlotStyle::instance()->indexColor(i_);
  }

  const CRGBA &color() const { return c_; }

 private:
  Type  type_ { Type::NONE };
  CRGBA c_    { CRGBA(0,0,0) };
  int   i_    { 0 };
};

#endif
