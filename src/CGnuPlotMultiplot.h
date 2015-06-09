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

  bool isRowsFirst() const { return rowsFirst_; }
  void setRowsFirst(bool b) { rowsFirst_ = b; }

  bool isDownward() const { return downward_; }
  void setDownward(bool b) { downward_ = b; }

  double xScale() const { return xscale_; }
  void setXScale(double r) { xscale_ = r; }

  double yScale() const { return yscale_; }
  void setYScale(double r) { yscale_ = r; }

  double xOffset() const { return xoffset_; }
  void setXOffset(double r) { xoffset_ = r; }

  double yOffset() const { return yoffset_; }
  void setYOffset(double r) { yoffset_ = r; }

  const std::string &title() const { return title_; }
  void setTitle(const std::string &title) { title_ = title; }

  const CFontPtr &titleFont() const { return titleFont_; }
  void setTitleFont(const CFontPtr &f) { titleFont_ = f; }

  double dx() const { return (cols_ > 0 ? 1.0/cols_ : 1.0); }
  double dy() const { return (rows_ > 0 ? 1.0/rows_ : 1.0); }

  CIPoint2D pos(int n) {
    int r = 0, c = 0;

    if (cols_ > 0 && rows_ > 0) {
      if (rowsFirst_) {
        r = n % rows_;
        c = n / rows_;
      }
      else {
        c = n % cols_;
        r = n / cols_;
      }
    }

    if (downward_)
      r = rows_ - 1 - r;

    return CIPoint2D(c, r);
  }

 private:
  bool        enabled_   { false };
  bool        autoFit_   { true };
  bool        enhanced_  { false };
  int         rows_      { 0 };
  int         cols_      { 0 };
  bool        rowsFirst_ { true };
  bool        downward_  { true };
  double      xscale_    { 1.0 };
  double      yscale_    { 1.0 };
  double      xoffset_   { 0.0 };
  double      yoffset_   { 0.0 };
  std::string title_;
  CFontPtr    titleFont_;
};

#endif
