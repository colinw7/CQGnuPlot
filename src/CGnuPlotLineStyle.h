#ifndef CGnuPlotLineStyle_H
#define CGnuPlotLineStyle_H

#include <CGnuPlotTypes.h>

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

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  int type() const { return type_; }
  void setType(int type) { type_ = type; }

  double width() const { return width_; }
  void setWidth(double width) { width_ = width; }

  const CLineDash &dash() const { return dash_; }
  void setDash(const CLineDash &d) { dash_ = d; }

  const CGnuPlotColorSpec &colorSpec() const { return color_; }
  void setColorSpec(const CGnuPlotColorSpec &c) { color_ = c; }

  const CRGBA &color(const CRGBA &c) const {
    if (color_.type() == CGnuPlotColorSpec::Type::RGB ||
        color_.type() == CGnuPlotColorSpec::Type::INDEX)
      return color_.color();
    else
      return c;
  }

  void setColor(const CRGBA &c) { color_.setRGB(c); }

  SymbolType pointType() const { return pointType_; }
  void setPointType(SymbolType type) { pointType_ = type; }

  double pointSize() const { return pointSize_.getValue(1); }
  void setPointSize(double s) { pointSize_ = s; }
  void resetPointSize() { pointSize_ = COptReal(); }

  int pointInterval() const { return pointInterval_; }
  void setPointInterval(int pi) { pointInterval_ = pi; }

  void unset()  {
    width_         = 1;
    dash_          = CLineDash();
    pointType_     = static_cast<CGnuPlotTypes::SymbolType>(ind_);
    pointSize_     = COptReal();
    pointInterval_ = 0;

    color_.setRGB(CGnuPlotStyleInst->indexColor(ind_));
  }

  void print(std::ostream &os) const {
    os << "linetype " << type_ << " linewidth " << width_ << " pointtype " << int(pointType_) <<
          " pointsize " << pointSizeStr() << " pointinterval " << pointInterval_;
  }

  void show(std::ostream &os) const {
    os << "linetype " << ind_ << "," << " " << color_ <<
          " linewidth " << width_ << " dashtype " << dash_ <<
          " pointtype " << int(pointType_) <<
          " pointsize " << (! pointSize_.isValid() ? std::string("default") :
                            CStrUtil::toString(pointSize_.getValue(1))) <<
          " pointinterval " << pointInterval_ << std::endl;
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
  int               ind_       { -1 };
  int               type_      { 1 };
  double            width_     { 1 };
  CLineDash         dash_;
  CGnuPlotColorSpec color_;
  SymbolType        pointType_ { SymbolType::PLUS };
  COptReal          pointSize_;
  double            pointInterval_ { 0 };
};

typedef std::shared_ptr<CGnuPlotLineStyle> CGnuPlotLineStyleP;

#endif
