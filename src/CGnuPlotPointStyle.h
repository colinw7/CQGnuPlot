#ifndef CGnuPlotPointStyle_H
#define CGnuPlotPointStyle_H

class CGnuPlotPointStyle {
 public:
  CGnuPlotPointStyle() { }

  bool varSize() const { return varSize_; }
  void setVarSize(bool b) { varSize_ = b; }

 private:
  bool varSize_ { false };
};

#endif
