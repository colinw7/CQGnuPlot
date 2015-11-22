#ifndef CGnuPlotKeyEvent_H
#define CGnuPlotKeyEvent_H

class CGnuPlotKeyEvent {
 public:
  CGnuPlotKeyEvent() { }

  const CPoint2D &pixel() const { return pixel_; }
  void setPixel(const CPoint2D &v) { pixel_ = v; }

  const CPoint2D &window() const { return window_; }
  void setWindow(const CPoint2D &v) { window_ = v; }

  bool isShift() const { return shift_; }
  void setShift(bool b) { shift_ = b; }

  bool isControl() const { return control_; }
  void setControl(bool b) { control_ = b; }

  bool isAlt() const { return alt_; }
  void setAlt(bool b) { alt_ = b; }

  int key() const { return key_; }
  void setKey(int i) { key_ = i; }

  const std::string &text() const { return text_; }
  void setText(const std::string &v) { text_ = v; }

 private:
  CPoint2D    pixel_;
  CPoint2D    window_;
  bool        shift_   { false };
  bool        control_ { false };
  bool        alt_     { false };
  int         key_     { 0 };
  std::string text_;
};

#endif
