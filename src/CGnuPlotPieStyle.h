#ifndef CGnuPlotPieStyle_H
#define CGnuPlotPieStyle_H

class CGnuPlotPieStyle {
 public:
  CGnuPlotPieStyle() { }

  const COptReal &startAngle() const { return startAngle_; }
  void setStartAngle(double a) { startAngle_ = a; }

  const COptReal &innerRadius() const { return innerRadius_; }
  void setInnerRadius(double a) { innerRadius_ = a; }

  const COptReal &labelRadius() const { return labelRadius_; }
  void setLabelRadius(double a) { labelRadius_ = a; }

 protected:
  COptReal startAngle_;
  COptReal innerRadius_;
  COptReal labelRadius_;
};

#endif
