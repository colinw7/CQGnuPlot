#ifndef CGnuPlotSurfaceData_H
#define CGnuPlotSurfaceData_H

class CGnuPlotSurfaceData {
 public:
  CGnuPlotSurfaceData() { }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  const CRGBA &color() const { return color_; }
  void setColor(const CRGBA &c) { color_ = c; }

 private:
  bool  enabled_   { true };
  CRGBA color_     { 1, 1, 1};
};

#endif
