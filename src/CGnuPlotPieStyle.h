#ifndef CGnuPlotPieStyle_H
#define CGnuPlotPieStyle_H

class CGnuPlotPieStyle {
 public:
  CGnuPlotPieStyle() { }

  const std::optional<double> &startAngle() const { return startAngle_; }
  void setStartAngle(double a) { startAngle_ = a; }

  const std::optional<double> &innerRadius() const { return innerRadius_; }
  void setInnerRadius(double a) { innerRadius_ = a; }

  const std::optional<double> &labelRadius() const { return labelRadius_; }
  void setLabelRadius(double a) { labelRadius_ = a; }

 protected:
  std::optional<double> startAngle_;
  std::optional<double> innerRadius_;
  std::optional<double> labelRadius_;
};

#endif
