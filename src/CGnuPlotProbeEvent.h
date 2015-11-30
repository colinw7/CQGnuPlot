#ifndef CGnuPlotProbeEvent_H
#define CGnuPlotProbeEvent_H

class CGnuPlotProbeEvent {
 public:
  CGnuPlotProbeEvent() { }

  double x() const { return x_; }
  void setX(double r) { x_ = r; }

  double y() const { return y_; }
  void setY(double r) { y_ = r; }

  bool isNearest() const { return nearest_; }
  void setNearest(bool b) { nearest_ = b; }

  const std::string &tip() const { return tip_; }
  void setTip(const std::string &v) { tip_ = v; }

 private:
  double      x_       { 0 };
  double      y_       { 0 };
  bool        nearest_ { true };
  std::string tip_     { };
};

#endif
