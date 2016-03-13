#ifndef CGnuPlotPieChartStyleValue_H
#define CGnuPlotPieChartStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <string>

class CGnuPlotPieChartStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotPieChartStyleValue(CGnuPlotPlot *plot=0) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotPieChartStyleValue() { }

  const std::string &palette() const { return data_.palette; }
  void setPalette(const std::string &v) { data_.palette = v; }

  double alpha() const { return data_.alpha; }
  void setAlpha(double r) { data_.alpha = r; }

  double startAngle() const { return data_.startAngle; }
  void setStartAngle(double r) { data_.startAngle = r; }

  double innerRadius() const { return data_.innerRadius; }
  void setInnerRadius(double r) { data_.innerRadius = r; }

  double labelRadius() const { return data_.labelRadius; }
  void setLabelRadius(double r) { data_.labelRadius = r; }

  void init(const CGnuPlotPieChartStyleValue &value) {
    data_ = value.data_;
  }

 private:
  struct Data {
    std::string palette     { "subtle" };
    double      alpha       { 0.5 };
    double      startAngle  { 90 };
    double      innerRadius { 0 };
    double      labelRadius { 0.5 };
  };

  Data data_;
};

#endif
