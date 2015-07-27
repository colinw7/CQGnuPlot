#ifndef CGnuPlotTextStyle_H
#define CGnuPlotTextStyle_H

class CGnuPlotTextStyle {
 public:
  CGnuPlotTextStyle() { }

  bool isBoxed() const { return boxed_; }
  void setBoxed(bool b) { boxed_ = b; }

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  const CHAlignType &halign() const { return halign_; }
  void setHAlign(const CHAlignType &v) { halign_ = v; }

 protected:
  bool        boxed_  { false };
  CPoint2D    offset_ { 0, 0};
  CHAlignType halign_ { CHALIGN_TYPE_LEFT };
};

#endif
