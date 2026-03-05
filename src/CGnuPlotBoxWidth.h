#ifndef CGnuPlotBoxWidth_H
#define CGnuPlotBoxWidth_H

#include <CGnuPlotTypes.h>
#include <iostream>

class CGnuPlotBoxWidth {
 public:
  typedef CGnuPlotTypes::BoxWidthType BoxWidthType;

 public:
  CGnuPlotBoxWidth(double w=1, BoxWidthType t=BoxWidthType::AUTO) :
   width_(w), type_(t) {
  }

  double width() const { return width_; }
  void setWidth(double w) { width_ = w; }

  BoxWidthType type() const { return type_; }
  void setType(BoxWidthType type) { type_ = type; }

  bool isCalc() const { return calc_; }
  void setCalc(bool b) { calc_ = b; }

  bool isAutoWidth() const { return (width() == -2); }

  double getSpacing(double s) const {
    if      (type() == BoxWidthType::ABSOLUTE) return width();
    else if (type() == BoxWidthType::RELATIVE) return width()*s;
    else                                       return s;
  }

  void unset() {
    type_ = BoxWidthType::AUTO;
  }

  void show(std::ostream &os) const {
    if (type() == BoxWidthType::AUTO)
      os << "boxwidth is auto\n";
    else
      os << "boxwidth is " << width() << " " <<
            (type() == BoxWidthType::ABSOLUTE ? "absolute" : "relative") << "\n";
  }

  void save(std::ostream &os) const {
    if (type() == BoxWidthType::AUTO)
      os << "set boxwidth\n";
    else
      os << "set boxwidth " << width() << " " <<
            (type() == BoxWidthType::ABSOLUTE ? "absolute" : "relative") << "\n";
  }

 private:
  double       width_ { 1 };
  BoxWidthType type_  { BoxWidthType::AUTO };
  bool         calc_  { true };
};

#endif
