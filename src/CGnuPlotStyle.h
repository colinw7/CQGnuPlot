#ifndef CGnuPlotStyle_H
#define CGnuPlotStyle_H

#include <CRGBA.h>
#include <CRGBName.h>
#include <CGnuPlotTypes.h>

class CGnuPlotStyle {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  static CGnuPlotStyle *instance() {
    static CGnuPlotStyle *inst;

    if (! inst) inst = new CGnuPlotStyle;

    return inst;
  }

  CRGBA indexColor(int i) {
    initColors();

    if (i < 1) return CRGBA(0,0,0);

    return colors_[(i - 1) % colors_.size()];
  }

  SymbolType indexSymbol(int i) {
    if (i <  0) return SymbolType::NONE;
    if (i == 0) return SymbolType::POINT;

    int i1 = i - 1;
    int ii = int(SymbolType::LAST) - 1;

    return static_cast<SymbolType>((i1 % ii) + static_cast<int>(SymbolType::PLUS));
  }

  CGnuPlotTypes::FillPattern indexPattern(int i) {
    if (i <= 0) return CGnuPlotTypes::FillPattern::NONE;

    int i1 = i - 1;
    int ii = int(CGnuPlotTypes::FillPattern::LAST);

    return static_cast<CGnuPlotTypes::FillPattern>(
            (i1 % ii) + static_cast<int>(CGnuPlotTypes::FillPattern::HATCH));
  }

 private:
  void initColors() {
    if (colors_.empty()) {
      colors_.push_back(CRGBName::toRGBA("#FF0000")); // red
      colors_.push_back(CRGBName::toRGBA("#00FF00")); // green
      colors_.push_back(CRGBName::toRGBA("#0000FF")); // blue
      colors_.push_back(CRGBName::toRGBA("#FF00FF")); // magenta
      colors_.push_back(CRGBName::toRGBA("#00FFFF")); // cyan
      colors_.push_back(CRGBName::toRGBA("#FFFF00")); // yellow
      colors_.push_back(CRGBName::toRGBA("#000000")); // black
      colors_.push_back(CRGBName::toRGBA("#FF8000")); // orange
      colors_.push_back(CRGBName::toRGBA("#888888")); // gray
    }
  }

 private:
  std::vector<CRGBA> colors_;
};

#endif
