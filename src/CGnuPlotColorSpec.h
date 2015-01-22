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

  void setRGB(const CRGBA &c) {
    type_ = Type::RGB;
    c_    = c;
  }

  void setVariable() {
    type_ = Type::VARIABLE;
    c_    =  CRGBA(0,0,0);
  }

  void setPalette() {
    type_ = Type::PALETTE;
    c_    =  CRGBA(0,0,0);
  }

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
