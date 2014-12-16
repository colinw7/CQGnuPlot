#ifndef CGnuPlotStyle_H
#define CGnuPlotStyle_H

#include <CRGBA.h>
#include <CRGBName.h>

class CGnuPlotStyle {
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

  CGnuPlot::SymbolType indexSymbol(int i) {
    if (i <  0) return CGnuPlot::SYMBOL_NONE;
    if (i == 0) return CGnuPlot::SYMBOL_POINT;

    int i1 = i - 1;
    int ii = int(CGnuPlot::SYMBOL_LAST) - 1;

    return (CGnuPlot::SymbolType)((i1 % ii) + CGnuPlot::SYMBOL_PLUS);
  }

  CGnuPlot::PatternType indexPattern(int i) {
    if (i <= 0) return CGnuPlot::PATTERN_NONE;

    int i1 = i - 1;
    int ii = int(CGnuPlot::PATTERN_LAST);

    return (CGnuPlot::PatternType)((i1 % ii) + CGnuPlot::PATTERN_HATCH);
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
