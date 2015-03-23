#ifndef CGnuPlotBoxWidth_H
#define CGnuPlotBoxWidth_H

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

  bool isAutoWidth() const { return (width_ == -2); }

  double getSpacing(double s) const {
    if      (type_ == BoxWidthType::ABSOLUTE) return width_;
    else if (type_ == BoxWidthType::RELATIVE) return width_*s;
    else                                      return s;
  }

  void unset() {
    type_ = BoxWidthType::AUTO;
  }

  void show(std::ostream &os) {
    if (type_ == BoxWidthType::AUTO)
      os << "boxwidth is auto" << std::endl;
    else
      os << "boxwidth is " << width_ << " " <<
            (type_ == BoxWidthType::ABSOLUTE ? "absolute" : "relative") << std::endl;
  }

  void save(std::ostream &os) {
    if (type_ == BoxWidthType::AUTO)
      os << "set boxwidth" << std::endl;
    else
      os << "set boxwidth " << width_ << " " <<
            (type_ == BoxWidthType::ABSOLUTE ? "absolute" : "relative") << std::endl;
  }

 private:
  double       width_ { 1 };
  BoxWidthType type_  { BoxWidthType::AUTO };
  bool         calc_  { true };
};

#endif
