#ifndef CGnuPlotXYPlane_H
#define CGnuPlotXYPlane_H

class CGnuPlotXYPlane {
 public:
  CGnuPlotXYPlane(double z=0.5, bool relative=true) :
   z_(z), relative_(relative) {
  }

  void reset() { z_ = 0.5; relative_ = true; }

  double z() const { return z_; }
  void setZ(double z) { z_ = z; }

  bool isRelative() const { return relative_; }
  void setRelative(bool b) { relative_ = b; }

 private:
  double z_        { 0.5  };
  bool   relative_ { true };
};

#endif
