#ifndef CGnuPlotAxisData_H
#define CGnuPlotAxisData_H

#include <CGnuPlotColorSpec.h>
#include <CFont.h>

class CGnuPlotAxisData {
 public:
  typedef std::map<int,std::string>    ITicLabels;
  typedef std::map<double,std::string> RTicLabels;
  typedef std::map<int,RTicLabels>     LevelRTicLabels;

 public:
  CGnuPlotAxisData(int ind=1);

  int ind() const { return ind_; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  bool hasGrid() const { return grid_; }
  void setGrid(bool b) { grid_ = b; }

  bool hasGridTics() const { return gridTics_; }
  void setGridTics(bool b) { gridTics_ = b; }

  bool hasGridMinorTics() const { return gridMinorTics_; }
  void setGridMinorTics(bool b) { gridMinorTics_ = b; }

  bool isMirror() const { return mirror_; }
  void setMirror(bool b) { mirror_ = b; }

  bool isOutside() const { return outside_; }
  void setOutside(bool b) { outside_ = b; }

  bool isReverse() const { return reverse_; }
  void setReverse(bool b) { reverse_ = b; }

  bool isWriteBack() const { return writeback_; }
  void setWriteBack(bool b) { writeback_ = b; }

  bool isExtend() const { return extend_; }
  void setExtend(bool b) { extend_ = b; }

  //---

  bool isTime() const { return isTime_; }
  void setIsTime(bool b) { isTime_ = b; }

  bool isDays() const { return isDay_; }
  void setIsDays(bool b) { isDay_ = b; }

  bool isMonths() const { return isMonth_; }
  void setIsMonths(bool b) { isMonth_ = b; }

  //---

  void resetZeroAxis() {
    setZeroAxisDisplayed(false);

    resetZeroAxisLineStyle();
    resetZeroAxisLineType();
    resetZeroAxisLineWidth();
    resetZeroAxisLineDash();
  }

  bool isZeroAxisDisplayed() const { return zeroAxis_.displayed; }
  void setZeroAxisDisplayed(bool b) { zeroAxis_.displayed = b; }

  const COptInt &zeroAxisLineStyle() const { return zeroAxis_.lineStyle; }
  void setZeroAxisLineStyle(int lt) { zeroAxis_.lineStyle = lt; }
  void resetZeroAxisLineStyle() { zeroAxis_.lineStyle.setInvalid(); }

  const COptInt &zeroAxisLineType() const { return zeroAxis_.lineType; }
  void setZeroAxisLineType(int lt) { zeroAxis_.lineType = lt; }
  void resetZeroAxisLineType() { zeroAxis_.lineType.setInvalid(); }

  const COptReal &zeroAxisLineWidth() const { return zeroAxis_.lineWidth; }
  void setZeroAxisLineWidth(double lw) { zeroAxis_.lineWidth = lw; }
  void resetZeroAxisLineWidth() { zeroAxis_.lineWidth.setInvalid(); }

  const COptLineDash &zeroAxisLineDash() const { return zeroAxis_.lineDash; }
  void setZeroAxisLineDash(const CLineDash &dash) { zeroAxis_.lineDash = dash; }
  void resetZeroAxisLineDash() { zeroAxis_.lineDash.setInvalid(); }

  CRGBA     getZeroAxisColor(CGnuPlotGroup *group) const;
  double    getZeroAxisWidth() const;
  CLineDash getZeroAxisDash() const;

  //---

  bool isAutoScaleMin() const { return autoScaleMin_; }
  void setAutoScaleMin(bool b) { autoScaleMin_ = b; }

  bool isAutoScaleMax() const { return autoScaleMax_; }
  void setAutoScaleMax(bool b) { autoScaleMax_ = b; }

  void setAutoScale(bool b) { setAutoScaleMin(b); setAutoScaleMax(b); }

  bool isAutoScaleFixMin() const { return autoScaleFixMin_; }
  void setAutoScaleFixMin(bool b) { autoScaleFixMin_ = b; }

  bool isAutoScaleFixMax() const { return autoScaleFixMax_; }
  void setAutoScaleFixMax(bool b) { autoScaleFixMax_ = b; }

  void setAutoScaleFix(bool b) { setAutoScaleFixMin(b); setAutoScaleFixMax(b); }

  const COptReal &min() const { return min_; }
  void setMin(double r) { min_ = r; }
  void updateMin(double r) { min_.updateMin(r); }
  void resetMin() { min_.setInvalid(); }

  const COptReal &max() const { return max_; }
  void setMax(double r) { max_ = r; }
  void updateMax(double r) { max_.updateMax(r); }
  void resetMax() { max_.setInvalid(); }

  bool isBorderTics() const { return borderTics_; }
  void setBorderTics(bool b) { borderTics_ = b; }

  bool isMinorTicsDisplayed() const { return minorTics_; }
  void setMinorTicsDisplayed(bool b) { minorTics_ = b; }

  const COptReal &getMinorTicsFreq() const { return minorTicsFreq_; }
  void setMinorTicsFreq(double r) { minorTicsFreq_ = r; }
  void resetMinorTicsFreq() { minorTicsFreq_.setInvalid(); }

  //------

  void setTicScale(double majorScale, double minorScale);

  double getTicMajorScale() const { return majorScale_; }
  double getTicMinorScale() const { return minorScale_; }

  //------

  double getTextRotate() const { return textRotate_; }
  void setTextRotate(double a) { textRotate_ = a; }

  //------

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  //------

  void setTicLabel(double r, const std::string &s, int level);

  bool hasITicLabels() const { return ! iticLabels_.empty(); }
  bool hasRTicLabels(int level) const;

  void clearTicLabels() { clearITicLabels(); clearRTicLabels(); }

  //------

  bool hasITicLabel(int i) const { return (iticLabels_.find(i) != iticLabels_.end()); }

  const std::string &iticLabel(int i) const;

  //------

  RTicLabels rticLabels(int level) const;

  //------

  const std::string &getDummyVar() { return dummyVar_; }
  void setDummyVar(const std::string &name) { dummyVar_ = name; }

 private:
  void setITicLabel(int    i, const std::string &s) { iticLabels_[i] = s; }
  void setRTicLabel(double r, const std::string &s, int level) { rticLabels_[level][r] = s; }

  void clearITicLabels() { iticLabels_.clear(); }
  void clearRTicLabels() { rticLabels_.clear(); }

  //------

 public:
  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  double rotate() const { return rotate_; }
  void setRotate(double r) { rotate_ = r; }

  const std::string &format() const { return format_; }
  void setFormat(const std::string &s) { format_ = s; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  bool showTics() const { return showTics_; }
  void setShowTics(bool b) { showTics_ = b; }

  bool isFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  int lineType() const { return lineType_; }
  void setLineType(int i) { lineType_ = i; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &v) { textColor_ = v; }

  const COptInt &logScale() const { return logScale_; }
  void setLogScale(int s) { logScale_ = s; }
  void resetLogScale() { logScale_ = COptInt(); }

  void unset();
  void unsetRange();
  void unsetZeroAxis();

  void reset();

  void show(std::ostream &os, const std::string &prefix, int n) const;

  void showRange    (std::ostream &os, const std::string &prefix) const;
  void showTics     (std::ostream &os, const std::string &prefix) const;
  void showFormat   (std::ostream &os, const std::string &prefix) const;
  void showMinorTics(std::ostream &os, const std::string &str, const std::string &str1) const;
  void showZeroAxis (std::ostream &os, const std::string &str);

  void printLabel(std::ostream &os, const std::string &prefix) const;

 private:
  struct ZeroAxis {
    bool         displayed { false };
    COptInt      lineStyle;
    COptInt      lineType;
    COptReal     lineWidth;
    COptLineDash lineDash;
  };

  int               ind_;
  bool              displayed_       { true  };
  bool              grid_            { false };
  bool              gridTics_        { true };
  bool              gridMinorTics_   { false };
  bool              mirror_          { true  };
  bool              outside_         { true  };
  bool              reverse_         { false };
  bool              writeback_       { false };
  bool              extend_          { false };
  bool              isTime_          { false };
  bool              isDay_           { false };
  bool              isMonth_         { false };
  bool              autoScaleMin_    { true };
  bool              autoScaleMax_    { true };
  bool              autoScaleFixMin_ { false };
  bool              autoScaleFixMax_ { false };
  COptReal          min_;
  COptReal          max_;
  bool              borderTics_      { true };
  bool              minorTics_       { true };
  COptReal          minorTicsFreq_;
  double            majorScale_      { 1.0 };
  double            minorScale_      { 0.5 };
  double            textRotate_      { 0.0 };
  std::string       text_;
  ITicLabels        iticLabels_;
  LevelRTicLabels   rticLabels_;
  CPoint2D          offset_          { 0, 0 };
  double            rotate_          { 0 };
  std::string       format_          { "%g" };
  CFontPtr          font_;
  bool              showTics_        { true };
  bool              front_           { true };
  bool              enhanced_        { true };
  int               lineType_        { -1 };
  COptInt           logScale_;
  CGnuPlotColorSpec textColor_;
  ZeroAxis          zeroAxis_;
  std::string       dummyVar_;
};

#endif
