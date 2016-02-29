#ifndef CGnuPlotBoxPlot_H
#define CGnuPlotBoxPlot_H

#include <COptVal.h>
#include <CGnuPlotTypes.h>

class CGnuPlotPlot;

class CGnuPlotBoxPlot {
 public:
  typedef CGnuPlotTypes::BoxType   BoxType;
  typedef CGnuPlotTypes::BoxLabels BoxLabels;

 public:
  CGnuPlotBoxPlot(CGnuPlotPlot *plot=0);

  virtual ~CGnuPlotBoxPlot() { }

  void init(const CGnuPlotBoxPlot &boxPlot);

  CGnuPlotPlot *plot() const { return plot_; }

  const BoxType &type() const { return type_; }
  void setType(const BoxType &t) { type_ = t; }

  const COptReal &range() const { return range_; }
  void setRange(double r) { range_ = r; }

  const COptReal &fraction() const { return fraction_; }
  void setFraction(double r) { fraction_ = r; }

  bool outliers() const { return outliers_; }
  void setOutliers(bool b) { outliers_ = b; }

  const COptReal &separation() const { return separation_; }
  void setSeparation(double s) { separation_ = s; }

  const COptInt &pointType() const { return pointType_; }
  void setPointType(int t) { pointType_ = t; }

  const BoxLabels &labels() const { return labels_; }
  void setLabels(const BoxLabels &l) { labels_ = l; }

  bool sorted() const { return sorted_; }
  void setSorted(bool b) { sorted_ = b; }

  void show(std::ostream &os) const;

  void unset();

 protected:
  CGnuPlotPlot* plot_       { 0 };
  BoxType       type_       { BoxType::CandleSticks };
  COptReal      range_;
  COptReal      fraction_;
  bool          outliers_   { true };
  COptReal      separation_;
  COptInt       pointType_;
  BoxLabels     labels_     { BoxLabels::Auto };
  bool          sorted_     { false };
};

#endif
