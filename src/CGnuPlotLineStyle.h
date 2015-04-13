#ifndef CGnuPlotLineStyle_H
#define CGnuPlotLineStyle_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotColorSpec.h>

#include <CRefPtr.h>
#include <CRGBA.h>
#include <COptVal.h>
#include <CLineDash.h>
#include <CStrUtil.h>

#include <sstream>

class CGnuPlotLineStyle {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotLineStyle() { }

  virtual ~CGnuPlotLineStyle() { }

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  const COptInt &type() const { return type_; }
  void setType(int type) { type_ = type; }
  void unsetType() { type_.setInvalid(); }

  double width() const { return width_; }
  void setWidth(double width) { width_ = width; }

  const CLineDash &dash() const { return dash_; }
  void setDash(const CLineDash &d) { dash_ = d; }

  const CGnuPlotColorSpec &color() const { return color_; }
  void setColor(const CGnuPlotColorSpec &c) { color_ = c; }

  const CRGBA &calcColor(const CRGBA &c) const {
    if      (color_.type() == CGnuPlotColorSpec::Type::RGB ||
             color_.type() == CGnuPlotColorSpec::Type::INDEX)
      return color_.color();
    else if (type_.isValid())
      return CGnuPlotStyleInst->indexColor(type_.getValue());
    else
      return c;
  }
  //void setColor(const CRGBA &c) { color_.setRGB(c); }

  SymbolType pointType() const { return pointType_; }
  void setPointType(SymbolType type) { pointType_ = type; }

  double pointSize() const { return pointSize_.getValue(1); }
  void setPointSize(double s) { pointSize_ = s; }
  void resetPointSize() { pointSize_ = COptReal(); }

  int pointInterval() const { return pointInterval_; }
  void setPointInterval(int pi) { pointInterval_ = pi; }

  bool palette() const { return palette_; }
  void setPalette(bool b) { palette_ = b; }

  void init(int ind) {
    unset();

    ind_ = ind;

    //color_.setRGB(CGnuPlotStyleInst->indexColor(ind_));
  }

  void unset();

  void print(std::ostream &os) const;

  void show(std::ostream &os) const;

 private:
  std::string pointSizeStr() const {
    if (pointSize_.isValid()) {
      std::stringstream os;

      os << pointSize_.getValue();

      return os.str();
    }
    else
      return "default";
  }

 private:
  int               ind_       { -1 };
  COptInt           type_;
  double            width_     { 1 };
  CLineDash         dash_;
  CGnuPlotColorSpec color_;
  SymbolType        pointType_ { SymbolType::PLUS };
  COptReal          pointSize_;
  double            pointInterval_ { 0 };
  bool              palette_ { false };
};

typedef CRefPtr<CGnuPlotLineStyle> CGnuPlotLineStyleP;

#endif
