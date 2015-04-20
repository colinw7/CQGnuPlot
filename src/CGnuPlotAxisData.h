#ifndef CGnuPlotAxisData_H
#define CGnuPlotAxisData_H

class CGnuPlotAxisData {
 public:
  CGnuPlotAxisData(int i=1) :
   ind_(i) {
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

  bool isAutoScaleFixMin() const { return autoScaleFixMin_; }
  void setAutoScaleFixMin(bool b) { autoScaleFixMin_ = b; }
  bool isAutoScaleFixMax() const { return autoScaleFixMax_; }
  void setAutoScaleFixMax(bool b) { autoScaleFixMax_ = b; }

  const COptReal &min() const { return min_; }
  void setMin(double r) { min_ = r; }
  void setMin(const COptReal &r) { min_ = r; }
  void updateMin(double r) { min_.updateMin(r); }
  void resetMin() { min_.setInvalid(); }

  const COptReal &max() const { return max_; }
  void setMax(double r) { max_ = r; }
  void setMax(const COptReal &r) { max_ = r; }
  void updateMax(double r) { max_.updateMax(r); }
  void resetMax() { max_.setInvalid(); }

  bool isMinorTicsDisplayed() const { return minorTics_; }
  void setMinorTicsDisplayed(bool b) { minorTics_ = b; }

  const COptReal &getMinorTicsFreq() const { return minorTicsFreq_; }
  void setMinorTicsFreq(double r) { minorTicsFreq_ = r; }
  void resetMinorTicsFreq() { minorTicsFreq_.setInvalid(); }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  bool hasTicLabel(int i) const { return (ticlabel_.find(i) != ticlabel_.end()); }

  const std::string &ticLabel(int i) const {
    auto p = ticlabel_.find(i);

    return (*p).second;
  }

  void setTicLabel(int i, const std::string &s) {
    ticlabel_[i] = s;
  }

  double offset() const { return offset_; }
  void setOffset(double r) { offset_ = r; }

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

  void unset() {
    displayed_ = false;

    unsetRange();

    text_ = "";
    offset_ = 0;
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
      os << "  offset (character " << offset_ << ", 0, 0)" << std::endl;
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
          "offset at ((character units) " << offset_ << ", 0, 0)" << std::endl;
  }

 private:
  typedef std::map<int,std::string> TicLabelMap;

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
  std::string       text_;
  TicLabelMap       ticlabel_;
  double            offset_          { 0 };
  double            rotate_          { 0 };
  std::string       format_          { "%g" };
  CFontPtr          font_;
  bool              showTics_        { true };
  bool              front_           { true };
  bool              enhanced_        { true };
  int               lineType_        { -1 };
  CGnuPlotColorSpec textColor_;
  ZeroAxis          zeroAxis_;
};

#endif
