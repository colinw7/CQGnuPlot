#ifndef CGnuPlotAxisData_H
#define CGnuPlotAxisData_H

#include <CGnuPlotColorSpec.h>
#include <CFont.h>

class CGnuPlotAxisData {
 public:
  typedef std::map<int,std::string>    ITicLabels;
  typedef std::map<double,std::string> RTicLabels;

 public:
  CGnuPlotAxisData(int ind=1) :
   ind_(ind) {
    displayed_ = (ind_ == 1);
    gridTics_  = (ind_ == 1);
    minorTics_ = (ind_ == 1);
    showTics_  = (ind_ == 1);
  }

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

  bool isZeroAxisDisplayed() const { return zeroAxis_.displayed; }
  void setZeroAxisDisplayed(bool b) { zeroAxis_.displayed = b; }

  int  zeroAxisLineStyle() const { return zeroAxis_.lineStyle; }
  void setZeroAxisLineStyle(int lt) { zeroAxis_.lineStyle = lt; }

  int  zeroAxisLineType() const { return zeroAxis_.lineType; }
  void setZeroAxisLineType(int lt) { zeroAxis_.lineType = lt; }

  double zeroAxisLineWidth() const { return zeroAxis_.lineWidth; }
  void setZeroAxisLineWidth(double lw) { zeroAxis_.lineWidth = lw; }

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

  bool isMinorTicsDisplayed() const { return minorTics_; }
  void setMinorTicsDisplayed(bool b) { minorTics_ = b; }

  const COptReal &getMinorTicsFreq() const { return minorTicsFreq_; }
  void setMinorTicsFreq(double r) { minorTicsFreq_ = r; }
  void resetMinorTicsFreq() { minorTicsFreq_.setInvalid(); }

  //------

  void setTicScale(double majorScale, double minorScale) {
    majorScale_ = majorScale;
    minorScale_ = minorScale;
  }

  double getTicMajorScale() const { return majorScale_; }
  double getTicMinorScale() const { return minorScale_; }

  //------

  double getTextRotate() const { return textRotate_; }
  void setTextRotate(double a) { textRotate_ = a; }

  //------

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  //------

  void setTicLabel(double r, const std::string &s) {
    setITicLabel(int(r + 0.5), s);
    setRTicLabel(r, s);
  }

  bool hasTicLabels() const { return hasITicLabels(); }

  void clearTicLabels() { clearITicLabels(); clearRTicLabels(); }

  //------

  bool hasITicLabel(int i) const { return (iticLabels_.find(i) != iticLabels_.end()); }

  const std::string &iticLabel(int i) const {
    auto p = iticLabels_.find(i);

    return (*p).second;
  }

  //------

  const RTicLabels &rticLabels() const { return rticLabels_; }

  //------

  const std::string &getDummyVar() { return dummyVar_; }
  void setDummyVar(const std::string &name) { dummyVar_ = name; }

 private:
  bool hasITicLabels() const { return ! iticLabels_.empty(); }
  bool hasRTicLabels() const { return ! rticLabels_.empty(); }

  void setITicLabel(int    i, const std::string &s) { iticLabels_[i] = s; }
  void setRTicLabel(double r, const std::string &s) { rticLabels_[r] = s; }

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

  void unset() {
    displayed_ = false;

    unsetRange();

    text_ = "";

    offset_ = CPoint2D(0,0);
  }

  void unsetRange() {
    min_ = COptReal();
    max_ = COptReal();

    reverse_   = false;
    writeback_ = false;
  }

  void unsetZeroAxis() {
    zeroAxis_.displayed = false;
    zeroAxis_.lineStyle = -1;
    zeroAxis_.lineType  = -1;
    zeroAxis_.lineWidth = 0;
  }

  void reset() {
    unsetRange();

    text_ = "";

    dummyVar_ = "";
  }

  void show(std::ostream &os, const std::string &prefix, int n) const {
    os << "set " << prefix << "axis " << n;
    os << " range [" << min_.getValue(0) << " : " << max_.getValue(10) << " ] ";
    os << " " << (reverse_   ? "reverse"   : "noreverse" );
    os << " " << (writeback_ ? "writeback" : "nowriteback");

    // paxis 1 -axis tics are IN, major ticscale is 1 and minor ticscale is 0.5
    // paxis 1 -axis tics: on axis tics are limited to data range

    os << " ";
    printLabel(os, prefix);

    // intervals computed automatically
  }

  void showRange(std::ostream &os, const std::string &prefix) const {
    os << "set " << prefix;
    os << " [ "; min_.print(os, "*"); os << " : "; max_.print(os, "*"); os << " ]";

    os << " " << (reverse_   ? "reverse"   : "noreverse" );
    os << " " << (writeback_ ? "writeback" : "nowriteback");

    if (! min_.isValid() || ! max_.isValid()) {
      os << " # (currently [";
      if (! min_.isValid()) os << min_.getValue(-10);
      os << ":";
      if (! max_.isValid()) os << max_.getValue( 10);
      os << "] )";
    }

    os << std::endl;
  }

  void showTics(std::ostream &os, const std::string &prefix) const {
    os << prefix << " tics are IN, major ticscale is 1 and minor ticscale is 0.5" << std::endl;

    if (showTics_) {
      os << prefix << " tics: on axis" << std::endl;
      os << "  labels are justified automatically, format \"" <<
            format_ << "\" and are not rotated," << std::endl;
      os << "  offset (character " << offset_.x << ", " << offset_.y << ", 0)" << std::endl;
      os << "  intervals computed automatically" << std::endl;
    }
    else
      os << prefix << " tics: OFF" << std::endl;
  }

  void showFormat(std::ostream &os, const std::string &prefix) const {
    os << "set format " << prefix << " \"" << format_ << "\"" << std::endl;
  }

  void showMinorTics(std::ostream &os, const std::string &str, const std::string &str1) const {
    if (isMinorTicsDisplayed()) {
      if (getMinorTicsFreq().isValid())
        os << "minor " << str << " are drawn with " << getMinorTicsFreq().getValue() <<
              " subintervals between major " << str1 << " marks" << std::endl;
      else
        os << "minor " << str << " are computed automatically" << std::endl;
    }
    else
      os << "minor " << str << " are off" << std::endl;
  }

  void showZeroAxis(std::ostream &os, const std::string &str) {
    os << str << "zeroaxis is ";

    if (zeroAxis_.displayed) {
      os << "drawn with";
      os << " lt " << zeroAxis_.lineType;
      os << " linewidth " << zeroAxis_.lineWidth;
    }
    else
      os << "OFF";

    os << std::endl;
  }

  void printLabel(std::ostream &os, const std::string &prefix) const {
    os << prefix << "label is \"" << text_ << "\", " <<
          "offset at ((character units) " << offset_.x << ", " << offset_.y << ", 0)" << std::endl;
  }

 private:
  struct ZeroAxis {
    bool   displayed { false };
    int    lineStyle { -1 };
    int    lineType  { -1 };
    double lineWidth { 0 };
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
  bool              minorTics_       { true };
  COptReal          minorTicsFreq_;
  double            majorScale_      { 1.0 };
  double            minorScale_      { 0.5 };
  double            textRotate_      { 0.0 };
  std::string       text_;
  ITicLabels        iticLabels_;
  RTicLabels        rticLabels_;
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
