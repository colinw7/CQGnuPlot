#ifndef CGnuPlotAxis_H
#define CGnuPlotAxis_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotAxisData.h>
#include <CAlignType.h>
#include <CDirectionType.h>
#include <CLineDash.h>
#include <CPoint3D.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <string>
#include <vector>

#include <sys/types.h>

class CGnuPlot;
class CGnuPlotGroup;
class CGnuPlotRenderer;

class CGnuPlotAxis {
 public:
  typedef CGnuPlotTypes::DrawLayer     DrawLayer;
  typedef CGnuPlotTypes::AxisDirection AxisDirection;
  typedef CGnuPlotTypes::AxisType      AxisType;

 public:
  CGnuPlotAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data,
               double start=0.0, double end=1.0);

  virtual ~CGnuPlotAxis();

  CGnuPlot *app() const;

  CGnuPlotGroup *group() const { return group_; }

  const CGnuPlotAxisData &data() { return data_; }

  AxisType type() const { return data_.type(); }
  int      ind () const { return data_.ind (); }

  std::string id() const;

  bool isDisplayed() const { return data_.isDisplayed(); }
  void setDisplayed(bool b) { data_.setDisplayed(b); }

  //---

  double getStart() const { return start_; }
  double getEnd  () const { return end_  ; }

  double getDataStart() const { return dataStart_; }
  double getDataEnd  () const { return dataEnd_  ; }

  double getStart1() const { return start1_; }
  double getEnd1  () const { return end1_  ; }

  double getStart2() const;
  double getEnd2  () const;

  //---

  double zposition() const { return zposition_; }
  void setZPosition(double p) { zposition_ = p; }

  //---

  const COptInt &logBase() const { return data_.logBase(); }
  void setLogBase(int b) { data_.setLogBase(b); }
  void resetLogBase() { data_.resetLogBase(); }

  double logTol() const { return 0.1; }

  double logValue(double x, int base) const {
    return log(x + logTol())/log(base);
  }

  double expValue(double x, int base) const {
    return exp(x*log(base)) - logTol();
  }

  double mapLogValue  (double x) const { return data_.mapLogValue  (x); }
  double unmapLogValue(double x) const { return data_.unmapLogValue(x); }

  bool isLogValue() const { return data_.isLogValue(); }

  bool mappedInside(double x) const { return data_.mappedInside(x); }

  //---

  int getNumMajorTicks() const { return numTicks1_; }
  void setNumMajorTicks(int n) { numTicks1_ = n; }

  int getNumMinorTicks() const { return numTicks2_; }
  void setNumMinorTicks(int n) { numTicks2_ = n; }

  double getMajorIncrement() const;
  void setMajorIncrement(double i);

  double getMinorIncrement() const;

  int getTickIncrement() const { return tickIncrement_; }
  void setTickIncrement(int tickIncrement);

  const double *getTickSpaces   () const { return &tickSpaces_[0]; }
  int           getNumTickSpaces() const { return tickSpaces_.size(); }

  //---

  double getTicMajorScale() const { return data_.getTicMajorScale(); }
  void setTicMajorScale(double s) { data_.setTicMajorScale(s); }

  double getTicMinorScale() const { return data_.getTicMinorScale(); }
  void setTicMinorScale(double s) { data_.setTicMinorScale(s); }

  //------

  double ticsRotate() const { return data_.ticsRotate(); }
  void setTicsRotate(double a) { data_.setTicsRotate(a); }

  COptReal labelRotate() const { return data_.labelRotate(); }
  void setLabelRotate(double a) { data_.setLabelRotate(a); }
  void unsetLabelRotate() { data_.unsetLabelRotate(); }

  //---

  bool isTickInside() const { return data_.isInside(); }
  void setTickInside(bool b) { data_.setInside(b); }

  //---

  double getTickSpace(int i) const { return tickSpaces_[i]; }
  void setTickSpaces(double *tickSpaces, int numTickSpaces);

  //---

  bool isLabelInside() const { return labelInside_; }
  void setLabelInside(bool b) { labelInside_ = b; }

  //---

  const std::string &getLabel() const { return data_.text(); }
  void setLabel(const std::string &str) { data_.setText(str); }

  const std::string &getTimeFormat() const { return data_.format(); }
  void setTimeFormat(const std::string &s) { data_.setFormat(s); }

  //---

  const CFontPtr &ticFont() const { return data_.ticFont(); }
  void setTicFont(const CFontPtr &f) { data_.setTicFont(f); }

  const CFontPtr &labelFont() const { return data_.labelFont(); }
  void setLabelFont(const CFontPtr &f) { data_.setLabelFont(f); }

  //---

  const CGnuPlotColorSpec &ticColor() const { return data_.ticColor(); }
  void setTicColor(const CGnuPlotColorSpec &c) { data_.setTicColor(c); }

  const CGnuPlotColorSpec &labelColor() const { return data_.labelColor(); }
  void setLabelColor(const CGnuPlotColorSpec &c) { data_.setLabelColor(c); }

  //---

  bool isParallel() const { return parallel_; }
  void setParallel(bool b) { parallel_ = b; }

  //---

  bool isZeroAxisDisplayed() const { return data_.isZeroAxisDisplayed(); }
  void setZeroAxisDisplayed(bool b) { data_.setZeroAxisDisplayed(b); }

  CRGBA zeroAxisLineColor() const { return data_.getZeroAxisColor(group_); }

  double zeroAxisLineWidth() const { return data_.getZeroAxisWidth(); }
  void setZeroAxisLineWidth(double w) { data_.setZeroAxisLineWidth(w); }

  CLineDash zeroAxisLineDash() const { return data_.getZeroAxisDash(); }
  void setZeroAxisLineDash(const CLineDash &d) { data_.setZeroAxisLineDash(d); }

  //---

  bool isBorderTics() const { return data_.isBorderTics(); }
  void setBorderTics(bool b) { data_.setBorderTics(b); }

  //---

  bool hasGrid() const { return data_.hasGrid(); }

  bool hasGridMajor() const { return data_.hasGridMajor(); }
  void setGridMajor(bool b) { data_.setGridMajor(b); }

  bool hasGridMinor() const { return data_.hasGridMinor(); }
  void setGridMinor(bool b) { data_.setGridMinor(b); }

  bool isGridBackLayer(bool defBack) const {
    if      (getGridLayer() == DrawLayer::BACK)
      return true;
    else if (getGridLayer() == DrawLayer::DEFAULT)
      return defBack;
    else
      return false;
  }

  DrawLayer getGridLayer() const { return data_.getGridLayer(); }
  void setGridLayer(const DrawLayer &layer) { data_.setGridLayer(layer); }

  double gridPolarAngle() const { return data_.gridPolarAngle(); }
  void setGridPolarAngle(double r) { data_.setGridPolarAngle(r); }

  int gridMajorLineStyle() const { return data_.gridMajorLineStyle(); }
  void setGridMajorLineStyle(int i) { data_.setGridMajorLineStyle(i); }

  int gridMinorLineStyle() const { return data_.gridMinorLineStyle(); }
  void setGridMinorLineStyle(int i) { data_.setGridMinorLineStyle(i); }

  int gridMajorLineType() const { return data_.gridMajorLineType(); }
  void setGridMajorLineType(int i) { data_.setGridMajorLineType(i); }

  int gridMinorLineType() const { return data_.gridMinorLineType(); }
  void setGridMinorLineType(int i) { data_.setGridMinorLineType(i); }

  double gridMajorLineWidth() const { return data_.gridMajorLineWidth(); }
  void setGridMajorLineWidth(double r) { data_.setGridMajorLineWidth(r); }

  double gridMinorLineWidth() const { return data_.gridMinorLineWidth(); }
  void setGridMinorLineWidth(double r) { data_.setGridMinorLineWidth(r); }

  //---

  bool isEnhanced() const { return data_.isEnhanced(); }
  void setEnhanced(bool b) { data_.setEnhanced(b); }

  //---

  bool isDrawTickMark() const { return data_.isShowTics(); }
  void setDrawTickMark(bool b) { data_.setShowTics(b); }

  bool isMirror() const { return data_.isMirror(); }
  void setMirror(bool b) { data_.setMirror(b); }

  bool isReverse() const { return data_.isReverse(); }

  //---

  bool isDrawMinorTickMark() const { return drawMinorTickMark_; }
  void setDrawMinorTickMark(bool b) { drawMinorTickMark_ = b; }

  bool clip() const { return clip_; }
  void setClip(bool b) { clip_ = b; }

  //---

  bool isDrawTickLabel() const { return drawTickLabel_; }
  bool isDrawTickLabel1() const { return drawTickLabel1_; }

  void setDrawTickLabel(bool b) { drawTickLabel_ = b; }
  void setDrawTickLabel1(bool b) { drawTickLabel1_ = b; }

  //---

  bool isDrawLabel() const { return drawLabel_; }
  void setDrawLabel(bool b) { drawLabel_ = b; }

  //---

  void setRange    (double start, double end);
  void setDataRange(double start, double end);

  bool hasMajorTicLabels() const;
  bool hasMinorTicLabels() const;

  CGnuPlotAxisData::RTicLabels getMajorTicLabels() const;
  CGnuPlotAxisData::RTicLabels getMinorTicLabels() const;

  std::string getValueStr(int i, double pos) const;

  virtual void drawAxes(CGnuPlotRenderer *renderer);

  void drawGrid(CGnuPlotRenderer *renderer);

  virtual void drawRadialGrid(CGnuPlotRenderer *renderer);

  void drawLowerLine(CGnuPlotRenderer *renderer, const CRGBA &c, double w, const CLineDash &dash);
  void drawUpperLine(CGnuPlotRenderer *renderer, const CRGBA &c, double w, const CLineDash &dash);
  void drawZeroLine (CGnuPlotRenderer *renderer, const CRGBA &c, double w, const CLineDash &dash);

  const CBBox2D &bbox() const { return bbox_; }

  static bool calcTics(double start, double end, double &start1, double &end1,
                       int &numTicks1, int &numTicks2);
  static bool calcTics(double start, double end, double inc, double &start1, double &end1,
                       int &numTicks1, int &numTicks2);

 private:
  void setBoundingBox();

  void drawAxisLine();

  void drawAxis();

  bool calc();

  static bool calcTics(double start, double end, int tickIncrement, double majorIncrement,
                       double &start1, double &end1, int &numTicks1, int &numTicks2);

  static bool testAxisGaps(double start, double end, double testIncrement,
                           int testNumGapTicks, double *start1, double *end1,
                           double *increment, int *numGaps, int *numGapTicks);

  bool checkMinorTickSize(double d) const;

  bool isVisibleValue(double pos) const;

  void drawAxisTicks(double pos, bool large);

  void drawAxisTick(double pos, bool large);
  void drawAxisTick(double pos, bool first, bool large);

  void drawTickLabels(double pos, const std::string &str);
  void drawTickLabel (double pos, const std::string &str, bool first);

  void drawAxisLabel();
  void drawAxisLabelStr(double pos, const std::string &str, int maxSize, bool first);

  void drawClipLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c,
                    double w, const CLineDash &lineDash);

  void drawLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c,
                double w, const CLineDash &lineDash);

  void drawHAlignedText(const CPoint3D &pos, CHAlignType halign, CVAlignType valign,
                        const std::string &str, const CRGBA &c, double angle=0);
  void drawVAlignedText(const CPoint3D &pos, CHAlignType halign, CVAlignType valign,
                        const std::string &str, const CRGBA &c, double angle=0);

  CPoint3D valueToPoint(double v, bool first, bool zero) const;

  CPoint3D ivalueToPoint(const CPoint3D &d) const;
  CPoint2D ivalueToPoint(const CPoint2D &d) const;

  CPoint3D perpPoint(const CPoint3D &p, double d) const;

 protected:
  typedef std::vector<double> TickSpaces;

  CGnuPlotRenderer *renderer_          { 0 };
  CGnuPlotGroup*    group_             { 0 };
  CGnuPlotAxisData  data_;
  AxisDirection     direction_         { AxisDirection::X };
  CPoint3D          v_                 { 1, 0, 0 };
  CPoint3D          iv_                { 0, 1, 0 };
  double            start_             { 0 };
  double            end_               { 1 };
  double            dataStart_         { 0 };
  double            dataEnd_           { 1 };
  double            zposition_         { 0 };
  bool              parallel_          { false };
  bool              labelInside_       { false };
  bool              drawMinorTickMark_ { false };
  bool              drawTickLabel_     { true };
  bool              drawTickLabel1_    { false };
  bool              drawLabel_         { true };
  bool              clip_              { false };
  int               tickIncrement_     { 0 };
  double            majorIncrement_    { 0 };
  TickSpaces        tickSpaces_;
  mutable double    start1_            { 0 };
  mutable double    end1_              { 1 };
  mutable int       numTicks1_         { 1 };
  mutable int       numTicks2_         { 0 };
  mutable bool      reverse_           { false };
  mutable CBBox2D   bbox_              { 0, 0, 1, 1 };
  mutable bool      drawOther_         { false };
  mutable double    maxW_, maxH_;
};

#endif
