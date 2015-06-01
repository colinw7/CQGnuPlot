#ifndef CGnuPlotStyle_H
#define CGnuPlotStyle_H

#include <CRGBA.h>
#include <CRGBName.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotPalette.h>
#include <CLineDash.h>

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

  const std::string &defName() const { return defName_; }

  void setDefName(const std::string &name) {
    if (! isNamedColor(name)) return;

    defName_ = name;
  }

  bool isNamedColor(const std::string &name) const {
    auto p = namedColors_.find(name);

    return (p != namedColors_.end());
  }

  uint getNumColors(const std::string &name) {
    initColors();

    auto p = namedColors_.find(name);

    if (p == namedColors_.end())
      return 0;

    return (*p).second.size();
  }

  const CRGBA &indexColor(int i) const{
    return indexColor(defName_, i);
  }

  const CRGBA &indexColor(const std::string &name, int i) const {
    static CRGBA defColor(0,0,0);

    initColors();

    if (i < 1) return defColor;

    auto p = namedColors_.find(name);

    if (p != namedColors_.end()) {
      const Colors &colors = (*p).second;

      return colors[(i - 1) % colors.size()].second;
    }
    else
      return defColor;
  }

  std::string indexColorName(int i) {
    return indexColorName(defName_, i);
  }

  std::string indexColorName(const std::string &name, int i) {
    initColors();

    if (i < 1) return "black";

    auto p = namedColors_.find(name);

    if (p != namedColors_.end()) {
      const Colors &colors = (*p).second;

      return colors[(i - 1) % colors.size()].first;
    }
    else
      return "black";
  }

  CRGBA getRangeColor(const std::string &name, double x) const {
    initColors();

    auto p = namedPalette_.find(name);

    if (p == namedPalette_.end())
      return CRGBA(0, 0, 0);

    CColor c = (*p).second.getColor(x);

    return c.rgba();
  }

  double indexWidth(int i) const {
    return i;
  }

  CLineDash indexDash(int) {
    return CLineDash();
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

  double indexPointSize(int) {
    return -1;
  }

  double indexPointInterval(int) {
    return 0.0;
  }

 private:
  void initColors() const {
    CGnuPlotStyle *th = const_cast<CGnuPlotStyle *>(this);

    if (namedColors_.empty()) {
#define NAME_COLOR_DEF(a) { a, CRGBName::toRGBA(a) }
      Colors basicColors = {{
        NAME_COLOR_DEF("#FF0000"), // red
        NAME_COLOR_DEF("#00FF00"), // green
        NAME_COLOR_DEF("#0000FF"), // blue
        NAME_COLOR_DEF("#FF00FF"), // magenta
        NAME_COLOR_DEF("#00FFFF"), // cyan
        NAME_COLOR_DEF("#FFFF00"), // yellow
        NAME_COLOR_DEF("#FF8000"), // orange
        NAME_COLOR_DEF("#888888"), // gray
      }};

      th->namedColors_["basic"] = basicColors;

      //---

      Colors classicColors = {{
        NAME_COLOR_DEF("#FF0000"), // red
        NAME_COLOR_DEF("#00FF00"), // green
        NAME_COLOR_DEF("#0000FF"), // blue
        NAME_COLOR_DEF("#FF00FF"), // magenta
        NAME_COLOR_DEF("#00FFFF"), // cyan
        NAME_COLOR_DEF("#FFFF00"), // yellow
        NAME_COLOR_DEF("#FF8000"), // orange
        NAME_COLOR_DEF("#888888"), // gray
      }};

      th->namedColors_["classic"] = classicColors;

      //---

      Colors defaultColors = {{
        NAME_COLOR_DEF("dark-violet"),
        NAME_COLOR_DEF("#009e73"),
        NAME_COLOR_DEF("#56b4e9"),
        NAME_COLOR_DEF("#e69f00"),
        NAME_COLOR_DEF("#f0e442"),
        NAME_COLOR_DEF("#0072b2"),
        NAME_COLOR_DEF("#e51e10"),
        NAME_COLOR_DEF("black"),
      }};

      th->namedColors_["default"] = defaultColors;

      //---

      Colors podoColors = {{
        NAME_COLOR_DEF("black"),
        NAME_COLOR_DEF("#e69f00"),
        NAME_COLOR_DEF("#56b4e9"),
        NAME_COLOR_DEF("#009e73"),
        NAME_COLOR_DEF("#f0e442"),
        NAME_COLOR_DEF("#0072b2"),
        NAME_COLOR_DEF("#d55e00"),
        NAME_COLOR_DEF("#cc79a7"),
      }};

      th->namedColors_["podo"] = podoColors;

      //---

      Colors subtleColors = {{
        // blue
        NAME_COLOR_DEF("#3182BD"),
        NAME_COLOR_DEF("#6BAED6"),
        NAME_COLOR_DEF("#9ECAE1"),
        NAME_COLOR_DEF("#C6DBEF"),

        // orange
        NAME_COLOR_DEF("#E6550D"),
        NAME_COLOR_DEF("#FD8D3C"),
        NAME_COLOR_DEF("#FDAE6B"),
        NAME_COLOR_DEF("#FDD0A2"),

        // green
        NAME_COLOR_DEF("#31A354"),
        NAME_COLOR_DEF("#74C476"),
        NAME_COLOR_DEF("#A1D99B"),
        NAME_COLOR_DEF("#C7E9C0"),

        // purple
        NAME_COLOR_DEF("#756BB1"),
        NAME_COLOR_DEF("#9E9AC8"),
        NAME_COLOR_DEF("#BCBDDC"),
        NAME_COLOR_DEF("#DADAEB"),

        // gray
        NAME_COLOR_DEF("#636363"),
        NAME_COLOR_DEF("#969696"),
        NAME_COLOR_DEF("#BDBDBD"),
        NAME_COLOR_DEF("#D9D9D9")
      }};

      th->namedColors_["subtle"] = subtleColors;

      //---

      th->initPalette("basic" );
      th->initPalette("classic" );
      th->initPalette("default");
      th->initPalette("podo");
      th->initPalette("subtle");
    };
#undef NAME_COLOR_DEF
  }

  void initPalette(const std::string &name) {
    auto p = namedColors_.find(name);

    CGnuPlotPalette palette;

    palette.setColorType(CGnuPlotPalette::ColorType::DEFINED);

    for (uint i = 0; i < (*p).second.size(); ++i) {
      CColor c((*p).second[i].second);

      palette.addDefinedColor(i, c);
    }

    namedPalette_[name] = palette;
  }

 private:
  typedef std::pair<std::string,CRGBA>          NameColor;
  typedef std::vector<NameColor>                Colors;
  typedef std::map<std::string,Colors>          NamedColors;
  typedef std::map<std::string,CGnuPlotPalette> NamedPalette;

  NamedColors  namedColors_;
  NamedPalette namedPalette_;
  std::string  defName_ { "default" };
};

#endif
