#ifndef CGnuPlotSurfaceData_H
#define CGnuPlotSurfaceData_H

class CGnuPlotSurfaceData {
 public:
  CGnuPlotSurfaceData() { }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

 private:
  bool enabled_ { true };
};

#endif
