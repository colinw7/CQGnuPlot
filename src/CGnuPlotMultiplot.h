#ifndef CGnuPlotMultiplot_H
#define CGnuPlotMultiplot_H

#include <CFont.h>
#include <CIPoint2D.h>

class CGnuPlot;

class CGnuPlotMultiplot {
 public:
  CGnuPlotMultiplot(CGnuPlot *plot);

  virtual ~CGnuPlotMultiplot();

  void reset() {
    enabled_   = false;
    autoFit_   = false;
    enhanced_  = false;
    rows_      = 0;
    cols_      = 0;
    rowsFirst_ = true;
    downward_  = true;
    xscale_    = 1.0;
    yscale_    = 1.0;
    xoffset_   = 0.0;
    yoffset_   = 0.0;
    lmargin_   = 0.0;
    rmargin_   = 0.0;
    tmargin_   = 0.0;
    bmargin_   = 0.0;
    xspacing_  = 0.0;
    yspacing_  = 0.0;
    title_     = "";
    titleFont_ = CFontPtr();
  }

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

  double xSpacing() const { return xspacing_; }
  void setXSpacing(double r) { xspacing_ = r; }

  double ySpacing() const { return yspacing_; }
  void setYSpacing(double r) { yspacing_ = r; }

  void getSpacing(double &x, double &y) { x = xSpacing(); y = ySpacing(); }
  void setSpacing(double x, double y) { setXSpacing(x); setYSpacing(y); }

  double lmargin() const { return lmargin_; }
  void setLMargin(double r) { lmargin_ = r; }
  double rmargin() const { return rmargin_; }
  void setRMargin(double r) { rmargin_ = r; }
  double tmargin() const { return tmargin_; }
  void setTMargin(double r) { tmargin_ = r; }
  double bmargin() const { return bmargin_; }
  void setBMargin(double r) { bmargin_ = r; }

  void getMargins(double &l, double &r, double &t, double &b) {
    l = lmargin(); r = rmargin(); t = tmargin(); b = bmargin();
  }
  void setMargins(double l, double r, double t, double b) {
    setLMargin(l); setRMargin(r); setTMargin(t); setBMargin(b);
  }

  const std::string &title() const { return title_; }
  void setTitle(const std::string &title) { title_ = title; }

  const CFontPtr &titleFont() const { return titleFont_; }
  void setTitleFont(const CFontPtr &f) { titleFont_ = f; }

  double dx() const { return (cols_ > 0 ? 1.0/cols_ : 1.0); }
  double dy() const { return (rows_ > 0 ? 1.0/rows_ : 1.0); }

  CIPoint2D pos(int n) const;

 private:
  CGnuPlot*   plot_      { 0 };
  bool        enabled_   { false };
  bool        autoFit_   { false };
  bool        enhanced_  { false };
  int         rows_      { 0 };
  int         cols_      { 0 };
  bool        rowsFirst_ { true };
  bool        downward_  { true };
  double      xscale_    { 1.0 };
  double      yscale_    { 1.0 };
  double      xoffset_   { 0.0 };
  double      yoffset_   { 0.0 };
  double      lmargin_   { 0.0 };
  double      rmargin_   { 0.0 };
  double      tmargin_   { 0.0 };
  double      bmargin_   { 0.0 };
  double      xspacing_  { 0.0 };
  double      yspacing_  { 0.0 };
  std::string title_;
  CFontPtr    titleFont_;
};

typedef CRefPtr<CGnuPlotMultiplot> CGnuPlotMultiplotP;

#endif
