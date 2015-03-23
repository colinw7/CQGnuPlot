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

  double range() const { return range_; }
  void setRange(double r) { range_ = r; }

  double fraction() const { return fraction_; }
  void setFraction(double r) { fraction_ = r; }

  bool outliers() const { return outliers_; }
  void setOutliers(bool b) { outliers_ = b; }

  int pointType() const { return pointtype_; }
  void setPointType(int t) { pointtype_ = t; }

  const Type &type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

  double separation() const { return separation_; }
  void setSeparation(double s) { separation_ = s; }

  const Labels &labels() const { return labels_; }
  void setLabels(const Labels &l) { labels_ = l; }

  bool sorted() const { return sorted_; }
  void setSorted(bool b) { sorted_ = b; }

 private:
  double range_      { 0.0 };
  double fraction_   { 0.0 };
  bool   outliers_   { true };
  int    pointtype_  { -1 };
  Type   type_       { Type::CandleSticks };
  double separation_ { 0.0 };
  Labels labels_     { Labels::Off };
  bool   sorted_     { false };
};

#endif
