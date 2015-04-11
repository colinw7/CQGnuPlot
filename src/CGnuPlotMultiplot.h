#ifndef CGnuPlotMultiplot_H
#define CGnuPlotMultiplot_H

class CGnuPlotMultiplot {
 public:
  CGnuPlotMultiplot() { }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  bool isAutoFit() const { return autoFit_; }
  void setAutoFit(bool b) { autoFit_ = b; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  int rows() const { return rows_; }
  void setRows(int i) { rows_ = i; }

  int cols() const { return cols_; }
  void setCols(int i) { cols_ = i; }

  double dx() const { return (cols_ > 0 ? 1.0/cols_ : 1.0); }
  double dy() const { return (rows_ > 0 ? 1.0/rows_ : 1.0); }

  CIPoint2D pos(int n) {
    int x = 0, y = 0;

    if (cols_ > 0 && rows_ > 0) {
      x = n % cols_;
      y = n / cols_;
    }

    return CIPoint2D(x, y);
  }

  const std::string &title() const { return title_; }
  void setTitle(const std::string &title) { title_ = title; }

 private:
  bool        enabled_  { false };
  bool        autoFit_  { true };
  bool        enhanced_ { false };
  int         rows_     { 0 };
  int         cols_     { 0 };
  std::string title_;
};

#endif
