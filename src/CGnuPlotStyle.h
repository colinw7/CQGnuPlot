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
  static CGnuPlotStyle *instance();

  const std::string &defName() const;

  void setDefName(const std::string &name);

  bool isNamedColor(const std::string &name) const;

  uint getNumColors(const std::string &name);

  const CRGBA &indexColor(int i) const;

  const CRGBA &indexColor(const std::string &name, int i) const;

  std::string indexColorName(int i);

  std::string indexColorName(const std::string &name, int i);

  CRGBA getRangeColor(const std::string &name, double x) const;

  double indexWidth(int i) const;

  CLineDash indexDash(int);

  SymbolType indexSymbol(int i);

  CGnuPlotTypes::FillPattern indexPattern(int i);

  double indexPointSize(int);

  double indexPointInterval(int);

 private:
  void initColors() const;

  void initPalette(const std::string &name);

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
