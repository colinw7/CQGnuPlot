#include <CGnuPlotStyle.h>

CGnuPlotStyle *
CGnuPlotStyle::
instance()
{
  static CGnuPlotStyle *inst;

  if (! inst) inst = new CGnuPlotStyle;

  return inst;
}

const std::string &
CGnuPlotStyle::
defName() const
{
  return defName_;
}

void
CGnuPlotStyle::
setDefName(const std::string &name)
{
  if (! isNamedColor(name)) return;

  defName_ = name;
}

bool
CGnuPlotStyle::
isNamedColor(const std::string &name) const
{
  auto p = namedColors_.find(name);

  return (p != namedColors_.end());
}

uint
CGnuPlotStyle::
getNumColors(const std::string &name)
{
  initColors();

  auto p = namedColors_.find(name);

  if (p == namedColors_.end())
    return 0;

  return (*p).second.size();
}

const CRGBA &
CGnuPlotStyle::
indexColor(int i) const
{
  return indexColor(defName_, i);
}

const CRGBA &
CGnuPlotStyle::
indexColor(const std::string &name, int i) const
{
  static CRGBA noColor   (0,0,0,0);
  static CRGBA blackColor(0,0,0);
  static CRGBA whiteColor(1,1,1);
  static CRGBA grayColor (0.6,0.6,0.6);

  initColors();

  // 0: gray, -1: black, -2: transparent, -3: background (TODO ?)
  if      (i == 0)
    return grayColor;
  else if (i == -1)
    return blackColor;
  else if (i == -2)
    return whiteColor;
  else if (i < 0)
    return blackColor;

  auto p = namedColors_.find(name);

  if (p != namedColors_.end()) {
    const Colors &colors = (*p).second;

    return colors[(i - 1) % colors.size()].second;
  }
  else
    return blackColor;
}

std::string
CGnuPlotStyle::
indexColorName(int i)
{
  return indexColorName(defName_, i);
}

std::string
CGnuPlotStyle::
indexColorName(const std::string &name, int i)
{
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

CRGBA
CGnuPlotStyle::
getRangeColor(const std::string &name, double x) const
{
  initColors();

  auto p = namedPalette_.find(name);

  if (p == namedPalette_.end())
    return CRGBA(0, 0, 0);

  CColor c = (*p).second.getColor(x);

  return c.rgba();
}

double
CGnuPlotStyle::
indexWidth(int i) const
{
  return i;
}

CLineDash
CGnuPlotStyle::
indexDash(int i)
{
  static CLineDash dotDash("1, 4");

  if (i == 0) return dotDash;

  return CLineDash();
}

CGnuPlotTypes::SymbolType
CGnuPlotStyle::
indexSymbol(int i)
{
  if (i <  0) return SymbolType::NONE;
  if (i == 0) return SymbolType::POINT;

  // map i (1->inf) -> symbol type (PLUS(1), FILLED_PENTAGON(LAST))
  int i1 = i - 1;
  int ii = int(SymbolType::LAST);

  return static_cast<SymbolType>((i1 % ii) + 1);
}

CGnuPlotTypes::FillPattern
CGnuPlotStyle::
indexPattern(int i)
{
  if (i <= 0) return CGnuPlotTypes::FillPattern::NONE;

  int i1 = i - 1;
  int ii = int(CGnuPlotTypes::FillPattern::LAST);

  return static_cast<CGnuPlotTypes::FillPattern>(
          (i1 % ii) + static_cast<int>(CGnuPlotTypes::FillPattern::HATCH));
}

double
CGnuPlotStyle::
indexPointSize(int)
{
  return -1;
}

double
CGnuPlotStyle::
indexPointInterval(int)
{
  return 0.0;
}

void
CGnuPlotStyle::
initColors() const
{
  CGnuPlotStyle *th = const_cast<CGnuPlotStyle *>(this);

  if (th->namedColors_.empty()) {
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

void
CGnuPlotStyle::
initPalette(const std::string &name)
{
  auto p = namedColors_.find(name);

  CGnuPlotPalette palette;

  palette.setColorType(CGnuPlotPalette::ColorType::DEFINED);

  for (uint i = 0; i < (*p).second.size(); ++i) {
    CColor c((*p).second[i].second);

    palette.addDefinedColor(i, c);
  }

  namedPalette_[name] = palette;
}
