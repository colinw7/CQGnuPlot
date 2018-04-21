#ifndef CGnuPlotPalette_H
#define CGnuPlotPalette_H

#include <CGradientPalette.h>

class CGnuPlot;
class CGnuPlotGroup;

class CGnuPlotPalette {
 public:
  typedef CGradientPalette::ColorType  ColorType;
  typedef CGradientPalette::ColorModel ColorModel;

 public:
  CGnuPlotPalette(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotPalette();

  void init(const CGnuPlotPalette &palette) {
    CGnuPlotGroup *group;

    std::swap(group, group_);

    *pal_ = *palette.pal_;

    std::swap(group, group_);
  }

  CGradientPalette *gradientPalette() const { return pal_; }
  void setGradientPalette(const CGradientPalette &pal);

  void setExpr(CExpr *expr) { pal_->setExpr(expr); }

  ColorType colorType() const { return pal_->colorType(); }
  void setColorType(ColorType t) { pal_->setColorType(t); }

  ColorModel colorModel() const { return pal_->colorModel(); }
  void setColorModel(ColorModel m) { pal_->setColorModel(m); }

  double gamma() const { return pal_->gamma(); }
  void setGamma(double g) { pal_->setGamma(g); }

  bool isGray() const { return pal_->isGray(); }
  void setGray(bool b) { pal_->setGray(b); }

  bool isRedNegative() const { return pal_->isRedNegative(); }
  void setRedNegative(bool b) { pal_->setRedNegative(b); }

  bool isGreenNegative() const { return pal_->isGreenNegative(); }
  void setGreenNegative(bool b) { pal_->setGreenNegative(b); }

  bool isBlueNegative() const { return pal_->isBlueNegative(); }
  void setBlueNegative(bool b) { pal_->setBlueNegative(b); }

  int maxColors() const { return pal_->maxColors(); }
  void setMaxColors(int n) { pal_->setMaxColors(n); }

  //bool isPSAllCF() const { return pal_->isPSAllCF(); }
  //void setPSAllCF(bool b) { pal_->setPSAllCF(b); }

  void setRgbModel(int r, int g, int b) { pal_->setRgbModel(r, g, b); }

  int redModel() const { return pal_->redModel(); }
  void setRedModel(int r) { pal_->setRedModel(r); }

  int greenModel() const { return pal_->greenModel(); }
  void setGreenModel(int r) { pal_->setGreenModel(r); }

  int blueModel() const { return pal_->blueModel(); }
  void setBlueModel(int r) { pal_->setBlueModel(r); }

  void addDefinedColor(double v, const CColor &c) { pal_->addDefinedColor(v, c); }

  void resetDefinedColors() { pal_->resetDefinedColors(); }

  void setFunctions(const std::string &rf, const std::string &gf, const std::string &bf) {
    pal_->setFunctions(rf, gf, bf);
  }

  void setCubeHelix(double start, double cycles, double saturation) {
    pal_->setCubeHelix(start, cycles, saturation);
  }

  CColor getColor(double x) const;

  bool readFile(CGnuPlot *plot, const std::string &filename);

  void unset() { pal_->unset(); }

  void show(std::ostream &os) const { pal_->show(os); }

  void showGradient(std::ostream &os) const { pal_->showGradient(os); }
  void showRGBFormulae(std::ostream &os) const { pal_->showRGBFormulae(os); }

  void showPaletteValues(std::ostream &os, int n, bool is_float, bool is_int) {
    pal_->showPaletteValues(os, n, is_float, is_int);
  }

 private:
  CGnuPlotGroup*    group_ { 0 };
  CGradientPalette* pal_   { 0 };
};

typedef std::unique_ptr<CGnuPlotPalette> CGnuPlotPaletteP;

#endif
