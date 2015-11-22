#ifndef CGnuPlotTextBoxStyle_H
#define CGnuPlotTextBoxStyle_H

class CGnuPlotTextBoxStyle {
 public:
  CGnuPlotTextBoxStyle() {
    unset();
  }

  bool isTransparent() const { return transparent_; }
  void setTransparent(bool b) { transparent_ = b; }

  bool isBorder() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  const CSize2D &margins() const { return margins_; }
  void setMargins(const CSize2D &v) { margins_ = v; }

  void unset() {
    transparent_ = true;
    border_      = true;
    margins_     = CSize2D(0, 0);
  }

  void show(std::ostream &os) const {
    os << "textboxes are ";
    os << (transparent_ ? "transparent" : "opaque");
    // TODO:
    //os << "with margins ";
    os << " and " << (border_  ? "border": "no border");
    os << std::endl;
  }

 private:
  bool    transparent_ { true };
  bool    border_      { true };
  CSize2D margins_     { 0, 0 };
};

#endif
