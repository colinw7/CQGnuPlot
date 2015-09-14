#ifndef CgnuPlotHistogramData_H
#define CgnuPlotHistogramData_H

#include <CGnuPlotTypes.h>

class CGnuPlotHistogramData {
 public:
  typedef CGnuPlotTypes::HistogramStyle HistogramStyle;

 public:
  CGnuPlotHistogramData() { }

  HistogramStyle style() const { return style_; }
  void setStyle(HistogramStyle s) { style_ = s; }

  const COptReal &gap() const { return gap_; }
  void setGap(double g) { gap_ = g; }
  void resetGap() { gap_.setInvalid(); }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double w) { lineWidth_ = w; }

  bool isBoxed() const { return boxed_; }
  void setBoxed(bool b) { boxed_ = b; }

  const CPoint2D &titleOffset() const { return titleOffset_; }
  void setTitleOffset(const CPoint2D &o) { titleOffset_ = o; }

  const CFontPtr &titleFont() const { return titleFont_; }
  void setTitleFont(const CFontPtr &f) { titleFont_ = f; }

  bool isHorizontal() const { return horizontal_; }
  void setHorizontal(bool b) { horizontal_ = b; }

 private:
  HistogramStyle style_       { HistogramStyle::CLUSTERED };
  COptReal       gap_;
  double         lineWidth_   { 0 };
  bool           boxed_       { false };
  CPoint2D       titleOffset_ { 0, 0 };
  CFontPtr       titleFont_;
  bool           horizontal_  { false };
};

class CGnuPlotNewHistogramData {
 public:
  CGnuPlotNewHistogramData() { }

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  const std::string &title() const { return title_; }
  void setTitle(const std::string &v) { title_ = v; }

  int lineType() const { return lineType_; }
  void setLineType(int i) { lineType_ = i; }

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &v) { fillStyle_ = v; }

  const COptReal &x() const { return x_; }
  void setX(double r) { x_ = r; }
  void resetX() { x_.setInvalid(); }

 private:
  int               ind_      { 0 };
  std::string       title_;
  int               lineType_ { -1 };
  CGnuPlotFillStyle fillStyle_;
  COptReal          x_;
};

typedef std::vector<CGnuPlotNewHistogramData> CGnuPlotNewHistogramDatas;

#endif
