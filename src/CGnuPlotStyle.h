#ifndef CGnuPlotStyle_H
#define CGnuPlotStyle_H

#include <CRGBA.h>
#include <CRGBName.h>
#include <CGnuPlotTypes.h>

#include <vector>

#define CGnuPlotStyleInst CGnuPlotStyle::instance()

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
    return indexColor("basic", i);
  }

  CRGBA indexColor(const std::string &name, int i) {
    initColors();

    if (i < 1) return CRGBA(0,0,0);

    NamedColors::const_iterator p = namedColors_.find(name);

    if (p != namedColors_.end()) {
      const Colors &colors = (*p).second;

      return colors[(i - 1) % colors.size()];
    }
    else
      return CRGBA(0,0,0);
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
    if (namedColors_.empty()) {
      Colors basicColors = {{
        CRGBName::toRGBA("#FF0000"), // red
        CRGBName::toRGBA("#00FF00"), // green
        CRGBName::toRGBA("#0000FF"), // blue
        CRGBName::toRGBA("#FF00FF"), // magenta
        CRGBName::toRGBA("#00FFFF"), // cyan
        CRGBName::toRGBA("#FFFF00"), // yellow
        CRGBName::toRGBA("#FF8000"), // orange
        CRGBName::toRGBA("#888888"), // gray
    //  CRGBName::toRGBA("#000000") // black
      }};

      namedColors_["basic"] = basicColors;

      Colors subtleColors = {{
        // blue
        CRGBName::toRGBA("#3182BD"),
        CRGBName::toRGBA("#6BAED6"),
        CRGBName::toRGBA("#9ECAE1"),
        CRGBName::toRGBA("#C6DBEF"),

        // orange
        CRGBName::toRGBA("#E6550D"),
        CRGBName::toRGBA("#FD8D3C"),
        CRGBName::toRGBA("#FDAE6B"),
        CRGBName::toRGBA("#FDD0A2"),

        // green
        CRGBName::toRGBA("#31A354"),
        CRGBName::toRGBA("#74C476"),
        CRGBName::toRGBA("#A1D99B"),
        CRGBName::toRGBA("#C7E9C0"),

        // purple
        CRGBName::toRGBA("#756BB1"),
        CRGBName::toRGBA("#9E9AC8"),
        CRGBName::toRGBA("#BCBDDC"),
        CRGBName::toRGBA("#DADAEB"),

        // gray
        CRGBName::toRGBA("#636363"),
        CRGBName::toRGBA("#969696"),
        CRGBName::toRGBA("#BDBDBD"),
        CRGBName::toRGBA("#D9D9D9")
      }};

      namedColors_["subtle"] = subtleColors;
    };
  }

 private:
  typedef std::vector<CRGBA>           Colors;
  typedef std::map<std::string,Colors> NamedColors;

  NamedColors namedColors_;
};

#endif
