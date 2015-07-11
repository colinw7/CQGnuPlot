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

 public:
  CGnuPlotAxis(CGnuPlotGroup *group=0, const std::string &id="",
               AxisDirection dir=AxisDirection::X, double start=0.0, double end=1.0);

  virtual ~CGnuPlotAxis();

  CGnuPlot *app() const;

  CGnuPlotGroup *group() const { return group_; }

  const std::string &id() const { return id_; }
  void setId(const std::string &id) { id_ = id; }

  bool isInitialized() const { return initialized_; }
  void setInitialized(bool b) { initialized_ = b; }

  double getStart() const { return start_; }
  double getEnd  () const { return end_  ; }

  double getStart1() const { return start1_; }
  double getEnd1  () const { return end1_  ; }

  const CPoint3D &position() const { return position_; }
  void setPosition(const CPoint3D &p) { position_ = p; }

  const CPoint3D &position1() const { return position1_; }
  void setPosition1(const CPoint3D &p) { position1_ = p; }

  const CPoint3D &linePosition() const { return linePosition_; }
  void setLinePosition(const CPoint3D &p) { linePosition_ = p; }

  const CPoint3D &linePosition1() const { return linePosition1_; }
  void setLinePosition1(const CPoint3D &p) { linePosition1_ = p; }

  //---

  bool isLogarithmicX() const { return logarithmicX_; }
  void setLogarithmicX(bool b) { logarithmicX_ = b; }

  int logarithmicBaseX() const { return logarithmicBaseX_; }
  void setLogarithmicBaseX(int b) { logarithmicBaseX_ = b; }

  bool isLogarithmicY() const { return logarithmicY_; }
  void setLogarithmicY(bool b) { logarithmicY_ = b; }

  int logarithmicBaseY() const { return logarithmicBaseY_; }
  void setLogarithmicBaseY(int b) { logarithmicBaseY_ = b; }

  bool isLogarithmicZ() const { return logarithmicZ_; }
  void setLogarithmicZ(bool b) { logarithmicZ_ = b; }

  int logarithmicBaseZ() const { return logarithmicBaseZ_; }
  void setLogarithmicBaseZ(int b) { logarithmicBaseZ_ = b; }

  CPoint3D mapLogPoint(const CPoint3D &p) const {
    return CPoint3D(mapLogXValue(p.x), mapLogYValue(p.y), mapLogZValue(p.z));
  }

  CPoint3D unmapLogPoint(const CPoint3D &p) const {
    return CPoint3D(unmapLogXValue(p.x), unmapLogYValue(p.y), unmapLogZValue(p.z));
  }

  double logValue(double x, int base) const {
    return log(std::max(x, 0.5))/log(base); // TODO: min value ?
  }

  double expValue(double x, int base) const {
    return exp(x*log(base)); // TODO: min value ?
  }

  double mapLogXValue(double x) const {
    if (logarithmicX_ && logarithmicBaseX_ > 1)
      return logValue(x, logarithmicBaseX_);
    else
      return x;
  }

  double mapLogYValue(double y) const {
    if (logarithmicY_ && logarithmicBaseY_ > 1)
      return logValue(y, logarithmicBaseY_);
    else
      return y;
  }

  double mapLogZValue(double z) const {
    if (logarithmicZ_ && logarithmicBaseZ_ > 1)
      return logValue(z, logarithmicBaseZ_);
    else
      return z;
  }

  double unmapLogXValue(double x) const {
    if (logarithmicX_ && logarithmicBaseX_ > 1)
      return expValue(x, logarithmicBaseX_);
    else
      return x;
  }

  double unmapLogYValue(double y) const {
    if (logarithmicY_ && logarithmicBaseY_ > 1)
      return expValue(y, logarithmicBaseY_);
    else
      return y;
  }

  double unmapLogZValue(double z) const {
    if (logarithmicZ_ && logarithmicBaseZ_ > 1)
      return expValue(z, logarithmicBaseZ_);
    else
      return z;
  }

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

  bool isTickInside (bool first) const { return (first ? tickInside_  : tickInside1_ ); }
  bool isTickOutside(bool first) const { return (first ? tickOutside_ : tickOutside1_); }

  bool isTickInside () const { return tickInside_; }
  bool isTickOutside() const { return tickOutside_; }

  void setTickInside (bool b) { tickInside_  = b; }
  void setTickOutside(bool b) { tickOutside_ = b; }

  bool isTickInside1 () const { return tickInside1_; }
  bool isTickOutside1() const { return tickOutside1_; }

  void setTickInside1 (bool b) { tickInside1_  = b; }
  void setTickOutside1(bool b) { tickOutside1_ = b; }

  //---

  double getTickSpace(int i) const { return tickSpaces_[i]; }
  void setTickSpaces(double *tickSpaces, int numTickSpaces);

  //---

  bool isLabelInside(bool first) const { return (first ? labelInside_ : labelInside1_); }

  bool isLabelInside () const { return labelInside_ ; }
  bool isLabelInside1() const { return labelInside1_; }

  void setLabelInside (bool b) { labelInside_  = b; }
  void setLabelInside1(bool b) { labelInside1_ = b; }

  //---

  const std::string &getLabel() const { return label_; }
  void setLabel(const std::string &str);

  const std::string &getTimeFormat() const { return timeFmt_; }
  void setTimeFormat(const std::string &ftm);

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  bool isDrawLine() const { return drawLine_; }
  void setDrawLine(bool b) { drawLine_ = b; }

  bool isDrawLine1() const { return drawLine1_; }
  void setDrawLine1(bool b) { drawLine1_ = b; }

  const CRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const CRGBA &v) { lineColor_ = v; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  const CLineDash &lineDash() const { return lineDash_; }
  void setLineDash(const CLineDash &d) { lineDash_ = d; }

  bool hasGrid() const { return grid_; }
  void setGrid(bool b) { grid_ = b; }

  bool hasGridMajor() const { return gridMajor_; }
  void setGridMajor(bool b) { gridMajor_ = b; }

  bool hasGridMinor() const { return gridMinor_; }
  void setGridMinor(bool b) { gridMinor_ = b; }

  bool isGridBackLayer(bool defBack) const {
    if      (gridLayer_ == DrawLayer::BACK)
      return true;
    else if (gridLayer_ == DrawLayer::DEFAULT)
      return defBack;
    else
      return false;
  }

  DrawLayer getGridLayer() const { return gridLayer_; }
  void setGridLayer(const DrawLayer &layer) { gridLayer_ = layer; }

  //---

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  //---

  bool isDrawTickMark(bool first) const { return (first ? drawTickMark_ : drawTickMark1_); }

  bool isDrawTickMark() const { return drawTickMark_; }
  void setDrawTickMark(bool b) { drawTickMark_ = b; }

  bool isDrawTickMark1() const { return drawTickMark1_; }
  void setDrawTickMark1(bool b) { drawTickMark1_ = b; }

  //---

  bool isDrawMinorTickMark() const { return drawMinorTickMark_; }
  void setDrawMinorTickMark(bool b) { drawMinorTickMark_ = b; }

  bool clip() const { return clip_; }
  void setClip(bool b) { clip_ = b; }

  //---

  bool isDrawTickLabel(bool first) const { return (first ? drawTickLabel_ : drawTickLabel1_); }

  bool isDrawTickLabel() const { return drawTickLabel_; }
  void setDrawTickLabel(bool b) { drawTickLabel_ = b; }

  bool isDrawTickLabel1() const { return drawTickLabel1_; }
  void setDrawTickLabel1(bool b) { drawTickLabel1_ = b; }

  //---

  bool isDrawLabel(bool first) const { return (first ? drawLabel_ : drawLabel1_); }

  bool isDrawLabel() const { return drawLabel_; }
  void setDrawLabel(bool b) { drawLabel_ = b; }

  bool isDrawLabel1() const { return drawLabel1_; }
  void setDrawLabel1(bool b) { drawLabel1_ = b; }

  //---

  void setRange(double start, double end);

  bool hasMajorTicLabels(bool first) const;
  bool hasMinorTicLabels(bool first) const;

  CGnuPlotAxisData::RTicLabels getMajorTicLabels(bool first) const;
  CGnuPlotAxisData::RTicLabels getMinorTicLabels(bool first) const;

  std::string getValueStr(int i, double pos) const;

  virtual void drawAxis(CGnuPlotRenderer *renderer, bool first=true);
  virtual void drawGrid(CGnuPlotRenderer *renderer);

  virtual void drawRadialGrid(CGnuPlotRenderer *renderer);

  const CBBox2D &getBBox() const { return bbox_; }

  static bool calcTics(double start, double end, double &start1, double &end1,
                       int &numTicks1, int &numTicks2);
  static bool calcTics(double start, double end, double inc, double &start1, double &end1,
                       int &numTicks1, int &numTicks2);

 private:
  bool calc();

  static bool calcTics(double start, double end, int tickIncrement, double majorIncrement,
                       double &start1, double &end1, int &numTicks1, int &numTicks2);

  static bool testAxisGaps(double start, double end, double testIncrement,
                           int testNumGapTicks, double *start1, double *end1,
                           double *increment, int *numGaps, int *numGapTicks);

  bool checkMinorTickSize(double d) const;

  void drawAxisTick(double pos, bool first, bool large);

  void drawTickLabel(double pos, const std::string &str, bool first);
  void drawAxisLabel(double pos, const std::string &str, int maxSize, bool first);

  void drawClipLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c,
                    double w, const CLineDash &lineDash);
  void drawLine    (const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c,
                    double w, const CLineDash &lineDash);

  void drawHAlignedText(const CPoint3D &pos, CHAlignType halign, double xOffset,
                        CVAlignType valign, double yOffset, const std::string &str,
                        bool map=false);
  void drawVAlignedText(const CPoint3D &pos, CHAlignType halign, double xOffset,
                        CVAlignType valign, double yOffset, const std::string &str, bool map=false);

  CPoint3D valueToPoint(double v, bool first) const;
  CPoint3D ivalueToPoint(const CPoint3D &d) const;
  CPoint2D ivalueToPoint(const CPoint2D &d) const;

  CPoint3D lineValueToPoint(double v, bool first) const;

  CPoint3D perpPoint(const CPoint3D &p, double d) const;

 protected:
  typedef std::vector<double> TickSpaces;

  CGnuPlotRenderer *renderer_          { 0 };
  CGnuPlotGroup*    group_             { 0 };
  std::string       id_                { "" };
  bool              initialized_       { false };
  AxisDirection     direction_         { AxisDirection::X };
  CPoint3D          v_                 { 1, 0, 0 };
  CPoint3D          iv_                { 0, 1, 0 };
  double            start_             { 0 };
  double            end_               { 1 };
  double            start1_            { 0 };
  double            end1_              { 1 };
  CPoint3D          position_          { 0, 0, 0 };
  CPoint3D          position1_         { 0, 0, 0 };
  CPoint3D          linePosition_      { 0, 0, 0 };
  CPoint3D          linePosition1_     { 0, 0, 0 };
  bool              reverse_           { false };
  bool              logarithmicX_      { false };
  int               logarithmicBaseX_  { 10 };
  bool              logarithmicY_      { false };
  int               logarithmicBaseY_  { 10 };
  bool              logarithmicZ_      { false };
  int               logarithmicBaseZ_  { 10 };
  int               numTicks1_         { 1 };
  int               numTicks2_         { 0 };
  int               tickIncrement_     { 0 };
  double            majorIncrement_    { 0 };
  TickSpaces        tickSpaces_;
  bool              tickInside_        { false };
  bool              tickInside1_       { true  };
  bool              tickOutside_       { false };
  bool              tickOutside1_      { false };
  bool              labelInside_       { false };
  bool              labelInside1_      { false };
  std::string       label_;
  std::string       timeFmt_;
  bool              displayed_         { true };
  bool              drawLine_          { true };
  bool              drawLine1_         { true };
  CRGBA             lineColor_;
  double            lineWidth_         { 0 };
  CLineDash         lineDash_;
  bool              drawTickMark_      { true };
  bool              drawTickMark1_     { true };
  bool              drawMinorTickMark_ { true };
  bool              drawTickLabel_     { true };
  bool              drawTickLabel1_    { false };
  bool              drawLabel_         { true };
  bool              drawLabel1_        { false };
  bool              clip_              { false };
  bool              grid_              { false };
  bool              gridMajor_         { true };
  bool              gridMinor_         { false };
  DrawLayer         gridLayer_         { DrawLayer::BACK };
  bool              enhanced_          { true };
  mutable CBBox2D   bbox_              { 0, 0, 1, 1 };
};

#endif
