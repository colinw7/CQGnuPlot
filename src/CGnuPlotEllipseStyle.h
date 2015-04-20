#ifndef CGnuPlotEllipseStyle_H
#define CGnuPlotEllipseStyle_H

class CGnuPlotEllipseStyle {
 public:
  enum class Units {
    XX,
    XY,
    YY
  };

  CGnuPlotEllipseStyle() {
    size_[0] = CGnuPlotCoordValue(0.05);
    size_[1] = CGnuPlotCoordValue(0.03);
    size_[2] = CGnuPlotCoordValue();
  }

  const Units &units() const { return units_; }
  void setUnits(const Units &v) { units_ = v; }

  const CGnuPlotCoordValue &size(int i) const { return size_[i]; }
  void setSize(int i, const CGnuPlotCoordValue &r) { size_[i] = r; }

  double angle() const { return angle_; }
  void setAngle(double r) { angle_ = r; }

  bool clip() const { return clip_; }
  void setClip (bool b) { clip_ = b; }

  void unset() {
    units_   = Units::XX;
    size_[0] = CGnuPlotCoordValue(0.05);
    size_[1] = CGnuPlotCoordValue(0.03);
    size_[2] = CGnuPlotCoordValue();
    angle_   = 0;
    clip_    = false;
  }

  void show(std::ostream &os) const {
    os << "Ellipse style has default size (";

    for (int i = 0; i < 3; ++i) {
      if (i > 0) os << ", ";

      os << size_[i];
    }

    os << ")";

    os << ", default angle is " << angle_ << " degrees";

    if      (units_ == Units::XX)
      os << ", both diameters are in the same units as the x axis";
    else if (units_ == Units::XY)
      os << ", diameters are in different units (major: x axis, minor: y axis)";
    else if (units_ == Units::YY)
      os << ", both diameters are in the same units as the y axis";

    os << (clip_ ? " clip" : " noclip" );

    os << std::endl;
  }

 private:
  Units              units_ { Units::XY };
  CGnuPlotCoordValue size_[3];
  double             angle_ { 0 };
  bool               clip_ { false };
};

#endif
