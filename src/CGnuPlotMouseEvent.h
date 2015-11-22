#ifndef CGnuPlotMouseEvent_H
#define CGnuPlotMouseEvent_H

class CGnuPlotMouseEvent {
 public:
  CGnuPlotMouseEvent() { }

  const CPoint2D &pixel() const { return pixel_; }
  void setPixel(const CPoint2D &v) { pixel_ = v; }

  const CPoint2D &window() const { return window_; }
  void setWindow(const CPoint2D &v) { window_ = v; }

  int button() const { return button_; }
  void setButton(int i) { button_ = i; }

  bool isShift() const { return shift_; }
  void setShift(bool b) { shift_ = b; }

  bool isControl() const { return control_; }
  void setControl(bool b) { control_ = b; }

  bool isAlt() const { return alt_; }
  void setAlt(bool b) { alt_ = b; }

 private:
  CPoint2D pixel_;
  CPoint2D window_;
  int      button_  { 1 };
  bool     shift_   { false };
  bool     control_ { false };
  bool     alt_     { false };
};

#endif
