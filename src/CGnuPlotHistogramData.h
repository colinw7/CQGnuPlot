#ifndef CgnuPlotHistogramData_H
#define CgnuPlotHistogramData_H

#include <CGnuPlotTypes.h>

class CGnuPlotHistogramData {
 public:
  typedef CGnuPlotTypes::HistogramStyle HistogramStyle;
  typedef std::pair<int,std::string>    NewTitle;

 public:
  CGnuPlotHistogramData() { }

  HistogramStyle style() const { return style_; }
  void setStyle(HistogramStyle s) { style_ = s; }

  double gap() const { return gap_; }
  void setGap(double g) { gap_ = g; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double w) { lineWidth_ = w; }

  bool isBoxed() const { return boxed_; }
  void setBoxed(bool b) { boxed_ = b; }

  const CPoint2D &titleOffset() const { return titleOffset_; }
  void setTitleOffset(const CPoint2D &o) { titleOffset_ = o; }

  const CFontPtr &titleFont() const { return titleFont_; }
  void setTitleFont(const CFontPtr &f) { titleFont_ = f; }

  void addNewTitle(int i, const std::string &title) {
    newTitles_.push_back(NewTitle(i, title));
  }

 private:
  typedef std::vector<NewTitle> NewTitles;

  HistogramStyle style_       { HistogramStyle::CLUSTERED };
  double         gap_         { 2 };
  double         lineWidth_   { 0 };
  bool           boxed_       { false };
  CPoint2D       titleOffset_ { 0, 0 };
  CFontPtr       titleFont_;
  NewTitles      newTitles_;
};

#endif
