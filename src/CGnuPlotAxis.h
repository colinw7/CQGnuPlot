#ifndef CAXIS_2D_H
#define CAXIS_2D_H

#include <COrientation.h>
#include <CAlignType.h>
#include <CDirectionType.h>
#include <CPoint2D.h>
#include <CFont.h>
#include <string>
#include <vector>

#include <sys/types.h>

class CGnuPlotWindow;

class CGnuPlotAxis {
 public:
  CGnuPlotAxis(CGnuPlotWindow *window, COrientation direction, double start=0.0, double end=1.0);
 ~CGnuPlotAxis();

  double getStart() const { return start1_; }
  double getEnd  () const { return end1_  ; }

  bool logarithmic() const { return logarithmic_ > 1; }
  void setLogarithmic(int base) { assert(base > 1); logarithmic_ = base; }
  void resetLogarithmic() { logarithmic_ = 0; }

  uint getNumMajorTicks() const { return num_ticks1_; }
  uint getNumMinorTicks() const { return num_ticks2_; }

  uint getTickIncrement() const { return tick_increment_; }

  const double *getTickSpaces   () const { return &tick_spaces_[0]; }
  uint          getNumTickSpaces() const { return tick_spaces_.size(); }

  void setTickInside(bool b) { tick_inside_ = b; }

  double getTickSpace(int i) const { return tick_spaces_[i]; }

  const std::string &getLabel() const { return label_; }

  bool getDisplayed() const { return displayed_; }

  void setRange(double start, double end);

  void setTickIncrement(uint tick_increment);

  void setTickSpaces(double *tick_spaces, uint num_tick_spaces);

  void setLabel(const std::string &str);

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
  CGnuPlotWindow*     window_;
  COrientation        direction_;
  double              start_, end_;
  double              start1_, end1_;
  int                 logarithmic_;
  uint                num_ticks1_, num_ticks2_;
  uint                tick_increment_;
  std::vector<double> tick_spaces_;
  bool                tick_inside_;
  std::string         label_;
  bool                displayed_;
  CFontPtr            font_;
};

#endif
