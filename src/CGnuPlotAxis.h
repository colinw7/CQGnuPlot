#ifndef CGnuPlotAxis_H
#define CGnuPlotAxis_H

#include <COrientation.h>
#include <CAlignType.h>
#include <CDirectionType.h>
#include <CPoint2D.h>
#include <CFont.h>
#include <string>
#include <vector>

#include <sys/types.h>

class CGnuPlotPlot;

class CGnuPlotAxis {
 public:
  CGnuPlotAxis(CGnuPlotPlot *plot, COrientation direction, double start=0.0, double end=1.0);
 ~CGnuPlotAxis();

  double getStart() const { return start1_; }
  double getEnd  () const { return end1_  ; }

  bool logarithmic() const { return logarithmic_ > 1; }
  void setLogarithmic(int base) { assert(base > 1); logarithmic_ = base; }
  void resetLogarithmic() { logarithmic_ = 0; }

  uint getNumMajorTicks() const { return numTicks1_; }
  void setNumMajorTicks(uint n) { numTicks1_ = n; }

  uint getNumMinorTicks() const { return numTicks2_; }
  void setNumMinorTicks(uint n) { numTicks2_ = n; }

  double getMajorIncrement() const;
  void setMajorIncrement(double i);

  double getMinorIncrement() const;

  uint getTickIncrement() const { return tickIncrement_; }
  void setTickIncrement(uint tickIncrement);

  const double *getTickSpaces   () const { return &tickSpaces_[0]; }
  uint          getNumTickSpaces() const { return tickSpaces_.size(); }

  bool isTickInside() const { return tickInside_; }
  void setTickInside(bool b) { tickInside_ = b; }

  double getTickSpace(int i) const { return tickSpaces_[i]; }
  void setTickSpaces(double *tickSpaces, uint numTickSpaces);

  bool isLabelInside() const { return labelInside_; }
  void setLabelInside(bool b) { labelInside_ = b; }

  const std::string &getLabel() const { return label_; }
  void setLabel(const std::string &str);

  const std::string &getTimeFormat() const { return timeFmt_; }
  void setTimeFormat(const std::string &ftm);

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  bool drawLine() const { return drawLine_; }
  void setDrawLine(bool b) { drawLine_ = b; }

  bool drawTickMark() const { return drawTickMark_; }
  void setDrawTickMark(bool b) { drawTickMark_ = b; }

  bool drawMinorTickMark() const { return drawMinorTickMark_; }
  void setDrawMinorTickMark(bool b) { drawMinorTickMark_ = b; }

  bool drawTickLabel() const { return drawTickLabel_; }
  void setDrawTickLabel(bool b) { drawTickLabel_ = b; }

  bool drawLabel() const { return drawLabel_; }
  void setDrawLabel(bool b) { drawLabel_ = b; }

  void setRange(double start, double end);

  std::string getValueStr(int i, double pos) const;

  void setFont(CFontPtr f) { font_ = f; }

  void drawAxis(double pos);

  void drawGrid(double start, double end);

 private:
  bool calc();

  bool testAxisGaps(double start, double end, double testIncrement,
                    uint testNumGapTicks, double *start1, double *end1,
                    double *increment, uint *numGaps, uint *numGapTicks);

  void drawAxisTick(const CPoint2D &p, CDirectionType type, bool large);

  void drawTickLabel(const CPoint2D &p, const std::string &str);

  void drawAxisLabel(const CPoint2D &p, const std::string &str, int maxSize);

  void drawLine(const CPoint2D &p1, const CPoint2D &p2);

  void drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double xOffset,
                        CVAlignType valign, double yOffset, const std::string &str);
  void drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double xOffset,
                        CVAlignType valign, double yOffset, const std::string &str);

 private:
  CGnuPlotPlot*       plot_              { 0 };
  COrientation        direction_;
  double              start_;
  double              end_;
  double              start1_;
  double              end1_;
  int                 logarithmic_       { 0 };
  uint                numTicks1_         { 1 };
  uint                numTicks2_         { 0 };
  uint                tickIncrement_     { 0 };
  double              majorIncrement_    { 0 };
  std::vector<double> tickSpaces_;
  bool                tickInside_        { false };
  bool                labelInside_       { false };
  std::string         label_;
  std::string         timeFmt_;
  bool                displayed_         { true };
  bool                drawLine_          { true };
  bool                drawTickMark_      { true };
  bool                drawMinorTickMark_ { true };
  bool                drawTickLabel_     { true };
  bool                drawLabel_         { true };
  CFontPtr            font_;
};

#endif
