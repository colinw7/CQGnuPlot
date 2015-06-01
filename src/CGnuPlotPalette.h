#ifndef CGnuPlotPalette_H
#define CGnuPlotPalette_H

#include <CGnuPlotTypes.h>
#include <CColor.h>
#include <map>
#include <memory>

class CGnuPlotGroup;

class CGnuPlotPalette {
 public:
  typedef CGnuPlotTypes::ColorModel ColorModel;

 public:
  enum class ColorType {
    MODEL,
    DEFINED,
    FUNCTIONS,
    CUBEHELIX
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

  int maxColors() const { return maxColors_; }
  void setMaxColors(int n) { maxColors_ = n; }

  bool isPSAllCF() const { return psAllcF_; }
  void setPSAllCF(bool b) { psAllcF_ = b; }

  void setRgbModel(int r, int g, int b) {
    colorType_ = ColorType::MODEL;

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

  void addDefinedColor(double v, const CColor &c) {
    colors_[v] = c;
  }

  CColor getColor(double x) const;

  double interp(int ind, double x) const;

  void setFunctions(const std::string &rf, const std::string &gf, const std::string &bf) {
    colorType_ = ColorType::FUNCTIONS;

    rf_ = rf; gf_ = gf; bf_ = bf;
  }

  void setCubeHelix(int start, int cycles, double saturation) {
    colorType_ = ColorType::CUBEHELIX;

    cbStart_      = start;
    cbCycles_     = cycles;
    cbSaturation_ = saturation;
  }

  bool readFile(const std::string &filename);

  void unset();

  void show(std::ostream &os) const;

  void showGradient(std::ostream &os) const;
  void showRGBFormulae(std::ostream &os) const;
  void showPaletteValues(std::ostream &os, int n, bool is_float, bool is_int);

 private:
  typedef std::map<double,CColor> ColorMap;

  CGnuPlotGroup *group_        { 0 };
  ColorType      colorType_    { ColorType::MODEL };
  bool           gray_         { false };
  ColorModel     colorModel_   { ColorModel::RGB };
  int            rModel_       { 7 };
  int            gModel_       { 5 };
  int            bModel_       { 15 };
  std::string    rf_, gf_, bf_;
  bool           negative_     { false };
  bool           psAllcF_      { false };
  double         gamma_        { 1.5 };
  int            maxColors_    { -1 };
  int            cbStart_      { 0 };
  int            cbCycles_     { 1 };
  double         cbSaturation_ { 1 };
  ColorMap       colors_;
};

typedef std::unique_ptr<CGnuPlotPalette> CGnuPlotPaletteP;

#endif
