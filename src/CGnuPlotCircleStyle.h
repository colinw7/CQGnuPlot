#ifndef CGnuPlotCircleStyle_H
#define CGnuPlotCircleStyle_H

class CGnuPlotCircleStyle {
 public:
  CGnuPlotCircleStyle() {
    radius_[0] = CGnuPlotCoordValue(0.02);
  }

  const CGnuPlotCoordValue &radius(int i) const { return radius_[i]; }
  void setRadius(int i, const CGnuPlotCoordValue &r) { radius_[i] = r; }

  bool wedge() const { return wedge_; }
  void setWedge(bool b) { wedge_ = b; }

  bool clip() const { return clip_; }
  void setClip (bool b) { clip_ = b; }

  void unset() {
    radius_[0] = CGnuPlotCoordValue(0.02);
    wedge_     = true;
    clip_      = false;
  }

  void show(std::ostream &os) const {
    os << "Circle style has default radius (";

    for (int i = 0; i < 3; ++i) {
      if (i > 0) os << ", ";

      os << radius_[i];
    }

    os << ")";

    os << (wedge_ ? " wedge" : " nowedge");
    os << (clip_  ? " clip"  : " noclip" );

    os << std::endl;
  }

 private:
  CGnuPlotCoordValue radius_[3];
  bool               wedge_    { true };
  bool               clip_     { false };
};

#endif
