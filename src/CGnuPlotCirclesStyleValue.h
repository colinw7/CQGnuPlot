#ifndef CGnuPlotCirclesStyleValue_H
#define CGnuPlotCirclesStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CGnuPlotCoordValue.h>

class CGnuPlotCirclesStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotCirclesStyleValue(CGnuPlotPlot *plot=0) :
   CGnuPlotStyleValue(plot) {
    radius_[0] = CGnuPlotCoordValue(0.02);
  }

  virtual ~CGnuPlotCirclesStyleValue() { }

  void init(const CGnuPlotCirclesStyleValue &v) {
    for (int i = 0; i <  3; ++i)
      radius_[i] = v.radius_[i];

    wedge_ = v.wedge_;
    clip_  = v.clip_;
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

    os << (wedge() ? " wedge" : " nowedge");
    os << (clip()  ? " clip"  : " noclip" );

    os << std::endl;
  }

 private:
  CGnuPlotCoordValue radius_[3];
  bool               wedge_    { true };
  bool               clip_     { false };
};

#endif
