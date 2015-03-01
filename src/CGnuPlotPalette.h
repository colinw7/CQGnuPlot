#ifndef CGnuPlotPalette_H
#define CGnuPlotPalette_H

#include <CGnuPlotTypes.h>
#include <CRGBA.h>
#include <map>
#include <memory>

class CGnuPlotGroup;

class CGnuPlotPalette {
 public:
  typedef CGnuPlotTypes::ColorModel ColorModel;

 public:
  enum class ColorType {
    MODEL,
    DEFINED
  };

 public:
  CGnuPlotPalette(CGnuPlotGroup *group=0) :
   group_(group) {
  }

  virtual ~CGnuPlotPalette() { }

  void init(const CGnuPlotPalette &palette) {
    CGnuPlotGroup *group;

    std::swap(group, group_);

    *this = palette;

    std::swap(group, group_);
  }

  ColorType colorType() const { return colorType_; }
  void setColorType(ColorType t) { colorType_ = t; }

  CGnuPlotTypes::ColorModel colorModel() const { return colorModel_; }
  void setColorModel(CGnuPlotTypes::ColorModel m) { colorModel_ = m; }

  double gamma() const { return gamma_; }
  void setGamma(double g) { gamma_ = g; }

  bool isGray() const { return gray_; }
  void setGray(bool b) { gray_ = b; }

  bool isNegative() const { return negative_; }
  void setNegative(bool b) { negative_ = b; }

  void setRgbModel(int r, int g, int b) {
    rModel_ = r;
    gModel_ = g;
    bModel_ = b;
  }

  int redModel() const { return rModel_; }
  void setRedModel(int r) { rModel_ = r; }

  int greenModel() const { return gModel_; }
  void setGreenModel(int r) { gModel_ = r; }

  int blueModel() const { return bModel_; }
  void setBlueModel(int r) { bModel_ = r; }

  void resetDefinedColors() {
    colors_.clear();
  }

  void addDefinedColor(double v, const CRGBA &c) {
    colors_[v] = c;
  }

  CRGBA getColor(double x) const;

  double interp(int ind, double x) const;

 private:
  typedef std::map<double,CRGBA> ColorMap;

  CGnuPlotGroup *group_      { 0 };
  ColorType      colorType_  { ColorType::MODEL };
  bool           gray_       { false };
  ColorModel     colorModel_ { ColorModel::RGB };
  int            rModel_     { 7 };
  int            gModel_     { 5 };
  int            bModel_     { 15 };
  bool           negative_   { false };
  double         gamma_      { 1.5 };
  ColorMap       colors_;
};

typedef std::unique_ptr<CGnuPlotPalette> CGnuPlotPaletteP;

#endif
