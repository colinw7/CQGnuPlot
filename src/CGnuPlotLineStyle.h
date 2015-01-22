#ifndef CGnuPlotLineStyle_H
#define CGnuPlotLineStyle_H

#include <CRGBA.h>
#include <COptVal.h>
#include <CGnuPlotTypes.h>
#include <sstream>

class CGnuPlotLineStyle {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotLineStyle() { }

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  int type() const { return type_; }
  void setType(int type) { type_ = type; }

  double width() const { return width_; }
  void setWidth(double width) { width_ = width; }

  CRGBA color(const CRGBA &c=CRGBA(0,0,0)) const { return color_.getValue(c); }
  void setColor(const CRGBA &c) { color_ = c; }

  SymbolType pointType() const { return pointType_; }
  void setPointType(SymbolType type) { pointType_ = type; }

  double pointSize() const { return pointSize_.getValue(1); }
  void setPointSize(double s) { pointSize_ = s; }

  void print(std::ostream &os) const {
    os << "linetype " << type_ << " linewidth " << width_ << " pointtype " << int(pointType_) <<
          " pointsize " << pointSizeStr() << " pointinterval " << pointInterval_;
  }

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
  int             ind_       { -1 };
  int             type_      { 1 };
  double          width_     { 1 };
  COptValT<CRGBA> color_;
  SymbolType      pointType_ { SymbolType::PLUS };
  COptReal        pointSize_;
  double          pointInterval_ { 0 };
};

typedef std::shared_ptr<CGnuPlotLineStyle> CGnuPlotLineStyleP;

#endif
