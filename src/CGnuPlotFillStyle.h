#ifndef CGnuPlotFillStyle_H
#define CGnuPlotFillStyle_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotColorSpec.h>
#include <COptVal.h>

class CGnuPlotFillStyle {
 public:
  typedef CGnuPlotTypes::FillType     FillType;
  typedef CGnuPlotTypes::FillPattern  FillPattern;
  typedef COptValT<CGnuPlotColorSpec> OptColor;

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

  const COptInt &borderLineType() const { return borderLineType_; }
  void setBorderLineType(int i) { borderLineType_ = i; }
  void unsetBorderLineType() { borderLineType_ = COptInt(); }

  const OptColor &borderColor() const { return borderColor_; }
  void setBorderColor(const CGnuPlotColorSpec &c) { borderColor_ = c; }
  void unsetBorderColor() { borderColor_ = OptColor(); }

  bool calcColor(CGnuPlotPlot *plot, CRGBA &c) const;

  void unset() {
    style_          = FillType::EMPTY;
    density_        = 1.0;
    pattern_        = FillPattern::NONE;
    transparent_    = false;
    border_         = true;
    borderLineType_ = COptInt();
    borderColor_    = OptColor();
  }

  void show(std::ostream &os) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotFillStyle &fs) {
    fs.print(os);

    return os;
  }

 private:
  FillType    style_       { FillType::EMPTY };
  double      density_     { 1.0 };               // density 0.0 empty, 1.0 is line color
  FillPattern pattern_     { FillPattern::NONE };
  bool        transparent_ { false };
  bool        border_      { true };
  COptInt     borderLineType_;
  OptColor    borderColor_;
};

#endif
