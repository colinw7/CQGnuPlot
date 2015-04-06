#ifndef CGnuPlotBoxPlot_H
#define CGnuPlotBoxPlot_H

class CGnuPlotBoxPlot {
 public:
  enum class Type {
    CandleSticks,
    FinanceBars
  };

  enum class Labels {
    Off,
    Auto,
    X,
    X2
  };

 public:
  CGnuPlotBoxPlot() { }

  const Type &type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

  const COptReal &range() const { return range_; }
  void setRange(double r) { range_ = r; }

  const COptReal &fraction() const { return fraction_; }
  void setFraction(double r) { fraction_ = r; }

  bool outliers() const { return outliers_; }
  void setOutliers(bool b) { outliers_ = b; }

  const COptReal &separation() const { return separation_; }
  void setSeparation(double s) { separation_ = s; }

  const COptInt &pointType() const { return pointtype_; }
  void setPointType(int t) { pointtype_ = t; }

  const Labels &labels() const { return labels_; }
  void setLabels(const Labels &l) { labels_ = l; }

  bool sorted() const { return sorted_; }
  void setSorted(bool b) { sorted_ = b; }

  void show(std::ostream &os) const {
    if (type_ == Type::CandleSticks)
      os << "boxplot representation is box and whisker" << std::endl;
    else
      os << "boxplot representation is finance bar" << std::endl;

    if (fraction_.isValid())
      os << "boxplot range extends from the median to include " <<
            fraction_.getValue() << " of the points" << std::endl;
    else
      os << "boxplot range extends from the box by " <<
            range_.getValue(1.5) << " of the interquartile distance" << std::endl;

    if (outliers_)
      os << "outliers will be drawn using point type " << pointtype_.getValue(7) << std::endl;
    else
      os << "outliers will not be drawn" << std::endl;

    os << "separation between boxplots is " << separation_.getValue(1) << std::endl;

    if      (labels_ == Labels::Off)
      os << "factor labels are off" << std::endl;
    else if (labels_ == Labels::Auto)
      os << "factor labels are automatic" << std::endl;
    else if (labels_ == Labels::X)
      os << "factor labels will be put on the x axis" << std::endl;
    else if (labels_ == Labels::X2)
      os << "factor labels will be put on the x2 axis" << std::endl;

    if (! sorted_)
      os << "factor labels will appear in the order they were found" << std::endl;
    else
      os << "factor labels will be sorted alphabetically" << std::endl;
  }

  void unset() {
    type_       = Type::CandleSticks;
    range_      = COptReal();
    fraction_   = COptReal();
    outliers_   = true;
    separation_ = COptReal();
    pointtype_  = COptInt();
    labels_     = Labels::Auto;
    sorted_     = false;
  }

 private:
  Type     type_       { Type::CandleSticks };
  COptReal range_;
  COptReal fraction_;
  bool     outliers_   { true };
  COptReal separation_;
  COptInt  pointtype_;
  Labels   labels_     { Labels::Auto };
  bool     sorted_     { false };
};

#endif
