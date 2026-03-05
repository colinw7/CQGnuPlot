#ifndef CGnuPlotBoxPlotStyleValue_H
#define CGnuPlotBoxPlotStyleValue_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotStyleValue.h>

class CGnuPlotBoxPlotStyleValue : public CGnuPlotStyleValue {
 public:
  typedef CGnuPlotTypes::BoxType   BoxType;
  typedef CGnuPlotTypes::BoxLabels BoxLabels;

 public:
  CGnuPlotBoxPlotStyleValue(CGnuPlotPlot *plot=nullptr);

  virtual ~CGnuPlotBoxPlotStyleValue() { }

  void init(const CGnuPlotBoxPlotStyleValue &boxPlot);

  const BoxType &type() const { return type_; }
  void setType(const BoxType &t) { type_ = t; }

  const std::optional<double> &range() const { return range_; }
  void setRange(double r) { range_ = r; }

  const std::optional<double> &fraction() const { return fraction_; }
  void setFraction(double r) { fraction_ = r; }

  bool outliers() const { return outliers_; }
  void setOutliers(bool b) { outliers_ = b; }

  const std::optional<double> &separation() const { return separation_; }
  void setSeparation(double s) { separation_ = s; }

  const std::optional<int> &pointType() const { return pointType_; }
  void setPointType(int t) { pointType_ = t; }

  const BoxLabels &labels() const { return labels_; }
  void setLabels(const BoxLabels &l) { labels_ = l; }

  bool sorted() const { return sorted_; }
  void setSorted(bool b) { sorted_ = b; }

  void show(std::ostream &os) const;

  void unset();

 protected:
  BoxType               type_       { BoxType::CandleSticks };
  std::optional<double> range_;
  std::optional<double> fraction_;
  bool                  outliers_   { true };
  std::optional<double> separation_;
  std::optional<int>    pointType_;
  BoxLabels             labels_     { BoxLabels::Auto };
  bool                  sorted_     { false };
};

#endif
