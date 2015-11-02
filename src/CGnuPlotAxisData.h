#ifndef CGnuPlotAxisData_H
#define CGnuPlotAxisData_H

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotGridData.h>
#include <CFont.h>

class CGnuPlotAxisData {
 public:
  typedef std::map<int,std::string>    ITicLabels;
  typedef std::map<double,std::string> RTicLabels;
  typedef std::map<int,RTicLabels>     LevelRTicLabels;
  typedef CGnuPlotTypes::DrawLayer     DrawLayer;
  typedef CGnuPlotTypes::AxisType      AxisType;
  typedef CGnuPlotTypes::AxisDirection AxisDirection;

  enum Justify {
    LEFT,
    RIGHT,
    CENTER,
    AUTOJUSTIFY
  };

 public:
  CGnuPlotAxisData(AxisType type=AxisType::X, int ind=1);

  AxisType type() const { return type_; }

  int ind() const { return ind_; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  AxisDirection direction() const;

  //---

  bool hasGrid() const { return grid_.isEnabled(); }

  bool hasGridMajor() const { return grid_.hasMajorTics(); }
  void setGridMajor(bool b) { grid_.setMajorTics(b); }

  bool hasGridMinor() const { return grid_.hasMinorTics(); }
  void setGridMinor(bool b) { grid_.setMinorTics(b); }

  const DrawLayer &getGridLayer() const { return grid_.layer(); }
  void setGridLayer(const DrawLayer &l) { grid_.setLayer(l); }

  double gridPolarAngle() const { return grid_.polarAngle(); }
  void setGridPolarAngle(double r) { grid_.setPolarAngle(r); }

  int gridMajorLineStyle() const { return grid_.majorLineStyle(); }
  void setGridMajorLineStyle(int i) { grid_.setMajorLineStyle(i); }

  int gridMinorLineStyle() const { return grid_.minorLineStyle(); }
  void setGridMinorLineStyle(int i) { grid_.setMinorLineStyle(i); }

  int gridMajorLineType() const { return grid_.majorLineType(); }
  void setGridMajorLineType(int i) { grid_.setMajorLineType(i); }

  int gridMinorLineType() const { return grid_.minorLineType(); }
  void setGridMinorLineType(int i) { grid_.setMinorLineType(i); }

  double gridMajorLineWidth() const { return grid_.majorLineWidth(); }
  void setGridMajorLineWidth(double r) { grid_.setMajorLineWidth(r); }

  double gridMinorLineWidth() const { return grid_.minorLineWidth(); }
  void setGridMinorLineWidth(double r) { grid_.setMinorLineWidth(r); }

  //---

  bool isMirror() const { return mirror_; }
  void setMirror(bool b) { mirror_ = b; }

  bool isInside() const { return inside_; }
  void setInside(bool b) { inside_ = b; }

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
  double getZeroAxisWidth() const;

  const COptLineDash &zeroAxisLineDash() const { return zeroAxis_.lineDash; }
  void setZeroAxisLineDash(const CLineDash &dash) { zeroAxis_.lineDash = dash; }
  void resetZeroAxisLineDash() { zeroAxis_.lineDash.setInvalid(); }
  CLineDash getZeroAxisDash() const;

  CRGBA getZeroAxisColor(CGnuPlotGroup *group) const;

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

  //---

  const COptReal &min() const { return min_; }
  void setMin(double r) { min_ = r; }
  void updateMin(double r) { min_.updateMin(r); }
  void resetMin() { min_.setInvalid(); }

  const COptReal &max() const { return max_; }
  void setMax(double r) { max_ = r; }
  void updateMax(double r) { max_.updateMax(r); }
  void resetMax() { max_.setInvalid(); }

  bool inside(double x) const;

  bool mappedInside(double x) const;

  //-----

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
  void setTicMajorScale(double s) { majorScale_ = s; }

  double getTicMinorScale() const { return minorScale_; }
  void setTicMinorScale(double s) { minorScale_ = s; }

  //------

  double ticsRotate() const { return ticsRotate_; }
  void setTicsRotate(double a) { ticsRotate_ = a; }

  COptReal labelRotate() const { return labelRotate_; }
  void setLabelRotate(double a) { labelRotate_ = a; }
  void unsetLabelRotate() { labelRotate_.setInvalid(); }

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

  //-----

  std::string geographicFormat(const std::string &fmt, double r) const;

 private:
  void setITicLabel(int    i, const std::string &s) { iticLabels_[i] = s; }
  void setRTicLabel(double r, const std::string &s, int level) { rticLabels_[level][r] = s; }

  void clearITicLabels() { iticLabels_.clear(); }
  void clearRTicLabels() { rticLabels_.clear(); }

  //------

 public:
  const CPoint2D &ticOffset() const { return ticOffset_; }
  void setTicOffset(const CPoint2D &o) { ticOffset_ = o; }

  const CPoint3D &labelOffset() const { return labelOffset_; }
  void setLabelOffset(const CPoint3D &o) { labelOffset_ = o; }

  void setLabelOffset(const CPoint2D &o) { setLabelOffset(CPoint3D(o.x, o.y, 0)); }

  //---

  Justify ticJustify() const { return ticJustify_; }
  void setTicJustify(Justify justify) { ticJustify_ = justify; }

  //---

  const std::string &format() const { return format_; }
  void setFormat(const std::string &s) { format_ = s; }

  //--

  const CFontPtr &ticFont() const { return ticFont_; }
  void setTicFont(const CFontPtr &f) { ticFont_ = f; }

  const CFontPtr &labelFont() const { return labelFont_; }
  void setLabelFont(const CFontPtr &f) { labelFont_ = f; }

  //--

  bool isShowTics() const { return showTics_; }
  void setShowTics(bool b) { showTics_ = b; }

  bool isFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  bool isNumeric() const { return numeric_; }
  void setNumeric(bool b) { numeric_ = b; }

  bool isTimeDate() const { return timedate_; }
  void setTimeDate(bool b) { timedate_ = b; }

  bool isGeographic() const { return geographic_; }
  void setGeographic(bool b) { geographic_ = b; }

  bool isRangeLimited() const { return rangelimited_; }
  void setRangeLimited(bool b) { rangelimited_ = b; }

  //---

  const CGnuPlotColorSpec &labelColor() const { return labelColor_; }
  void setLabelColor(const CGnuPlotColorSpec &c) { labelColor_ = c; }

  const CGnuPlotColorSpec &ticColor() const { return ticColor_; }
  void setTicColor(const CGnuPlotColorSpec &c) { ticColor_ = c; }

  //---

  int lineType() const { return lineType_; }
  void setLineType(int i) { lineType_ = i; }

  //---

  const COptInt &logBase() const { return logBase_; }
  void setLogBase(int s) { logBase_ = s; }
  void resetLogBase() { logBase_ = COptInt(); }

  double logTol() const { return 0.1; }

  double logValue(double x, int base) const {
    return log(x + logTol())/log(base);
  }

  double expValue(double x, int base) const {
    return exp(x*log(base)) - logTol();
  }

  double mapLogValue(double x) const {
    if (isLogValue())
      return logValue(x, logBase_.getValue());
    else
      return x;
  }

  double unmapLogValue(double x) const {
    if (isLogValue())
      return expValue(x, logBase_.getValue());
    else
      return x;
  }

  bool isLogValue() const { return (logBase_.isValid() && logBase_.getValue() > 1); }

  //---

  const COptReal &ticStart() const { return ticStart_; }
  void setTicStart(double r) { ticStart_ = r; }

  const COptReal &ticEnd() const { return ticEnd_; }
  void setTicEnd(double r) { ticEnd_ = r; }

  const COptReal &ticIncr() const { return ticIncr_; }
  void setTicIncr(double r) { ticIncr_ = r; }

  //---

  void unset();
  void unsetRange();
  void unsetZeroAxis();

  void reset();

  std::string getAxisValueStr(int i, double r) const;
  std::string formatAxisValue(double r) const;

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

  AxisType          type_            { AxisType::X };
  int               ind_             { 1     };
  bool              displayed_       { true  };
  CGnuPlotGridData  grid_;
  bool              mirror_          { false };
  bool              inside_          { true  };
  bool              reverse_         { false };
  bool              writeback_       { false };
  bool              extend_          { false };
  bool              isTime_          { false };
  bool              isDay_           { false };
  bool              isMonth_         { false };
  bool              autoScaleMin_    { true  };
  bool              autoScaleMax_    { true  };
  bool              autoScaleFixMin_ { false };
  bool              autoScaleFixMax_ { false };
  COptReal          min_;
  COptReal          max_;
  bool              borderTics_      { true  };
  bool              minorTics_       { false };
  COptReal          minorTicsFreq_;
  double            majorScale_      { 1.0   };
  double            minorScale_      { 0.5   };
  std::string       text_;
  ITicLabels        iticLabels_;
  LevelRTicLabels   rticLabels_;
  CPoint2D          ticOffset_       { 0,0   };
  CPoint3D          labelOffset_     { 0,0,0 };
  double            ticsRotate_      { 0.0   };
  COptReal          labelRotate_;
  std::string       format_          { "%g"  };
  Justify           ticJustify_      { Justify::CENTER };
  CFontPtr          ticFont_;
  CFontPtr          labelFont_;
  bool              showTics_        { true  };
  bool              front_           { true  };
  bool              enhanced_        { true  };
  bool              numeric_         { false };
  bool              timedate_        { false };
  bool              geographic_      { false };
  bool              rangelimited_    { false };
  int               lineType_        { -1    };
  COptInt           logBase_;
  COptReal          ticStart_;
  COptReal          ticEnd_;
  COptReal          ticIncr_;
  CGnuPlotColorSpec ticColor_;
  CGnuPlotColorSpec labelColor_;
  ZeroAxis          zeroAxis_;
  std::string       dummyVar_;
};

#endif
