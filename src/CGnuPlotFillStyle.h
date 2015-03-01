#ifndef CGnuPlotFillStyle_H
#define CGnuPlotFillStyle_H

#include <CRGBA.h>
#include <CGnuPlotTypes.h>

class CGnuPlotFillStyle {
 public:
  typedef CGnuPlotTypes::FillType    FillType;
  typedef CGnuPlotTypes::FillPattern FillPattern;

 public:
  CGnuPlotFillStyle() { }

  const FillType &style() const { return style_; }
  void setStyle(FillType style) { style_ = style; }

  double density() const { return density_; }
  void setDensity(double d) { density_ = d; }

  const FillPattern &pattern() const { return pattern_; }
  void setPattern(FillPattern p) { pattern_ = p; }

  bool isTransparent() const { return transparent_; }
  void setTransparent(bool b) { transparent_ = b; }

  bool hasBorder() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  int borderLineType() const { return borderLineType_; }
  void setBorderLineType(int i) { borderLineType_ = i; }

  const CRGBA &borderColor() const { return borderColor_; }
  void setBorderColor(const CRGBA &c) { borderColor_ = c; }

 private:
  FillType    style_          { FillType::EMPTY };
  double      density_        { 1.0 };               // density 0.0 empty, 1.0 is line color
  FillPattern pattern_        { FillPattern::NONE };
  bool        transparent_    { false };
  bool        border_         { true };
  int         borderLineType_ { -1 };
  CRGBA       borderColor_;
};

#endif
