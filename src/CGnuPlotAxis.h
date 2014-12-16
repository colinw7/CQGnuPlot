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
  uint getNumMinorTicks() const { return numTicks2_; }

  uint getTickIncrement() const { return tickIncrement_; }
  void setTickIncrement(uint tick_increment);

  const double *getTickSpaces   () const { return &tickSpaces_[0]; }
  uint          getNumTickSpaces() const { return tickSpaces_.size(); }

  bool isTickInside() const { return tickInside_; }
  void setTickInside(bool b) { tickInside_ = b; }

  double getTickSpace(int i) const { return tickSpaces_[i]; }
  void setTickSpaces(double *tick_spaces, uint num_tick_spaces);

  const std::string &getLabel() const { return label_; }
  void setLabel(const std::string &str);

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  bool drawLine() const { return drawLine_; }
  void setDrawLine(bool b) { drawLine_ = b; }

  bool drawTicMark() const { return drawTicMark_; }
  void setDrawTicMark(bool b) { drawTicMark_ = b; }

  bool drawTicLabel() const { return drawTicLabel_; }
  void setDrawTicLabel(bool b) { drawTicLabel_ = b; }

  bool drawLabel() const { return drawLabel_; }
  void setDrawLabel(bool b) { drawLabel_ = b; }

  void setRange(double start, double end);

  double getMajorIncrement() const;
  double getMinorIncrement() const;

  std::string getValueStr(double pos) const;

  void setFont(CFontPtr f) { font_ = f; }

  void drawAxis(double pos);

  void drawGrid(double start, double end);

 private:
  bool calc();

  bool testAxisGaps(double start, double end, double test_increment,
                    uint test_num_gap_ticks, double *start1, double *end1,
                    double *increment, uint *num_gaps, uint *num_gap_ticks);

  void drawAxisTick(const CPoint2D &p, CDirectionType type, bool large);

  void drawLine(const CPoint2D &p1, const CPoint2D &p2);

  void drawHAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                        CVAlignType valign, int y_offset, const std::string &str);
  void drawVAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                        CVAlignType valign, int y_offset, const std::string &str);

 private:
  CGnuPlotPlot*       plot_;
  COrientation        direction_;
  double              start_, end_;
  double              start1_, end1_;
  int                 logarithmic_;
  uint                numTicks1_, numTicks2_;
  uint                tickIncrement_;
  std::vector<double> tickSpaces_;
  bool                tickInside_;
  std::string         label_;
  bool                displayed_;
  bool                drawLine_;
  bool                drawTicMark_;
  bool                drawTicLabel_;
  bool                drawLabel_;
  CFontPtr            font_;
};

#endif
