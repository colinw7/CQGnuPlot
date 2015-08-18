#ifndef CGnuPlotLineProp_H
#define CGnuPlotLineProp_H

#include <COptVal.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotDash.h>

class CGnuPlot;

class CGnuPlotLineProp {
 public:
  CGnuPlotLineProp() { }

  double lineWidth(CGnuPlot *plot) const;
  void setLineWidth(double w) { lineWidth_ = w; }

  const COptInt &lineStyle() const { return lineStyle_; }
  void setLineStyle(int t) { lineStyle_ = t; }

  const COptInt &lineType() const { return lineType_; }
  void setLineType(int type) { lineType_ = type; }

  const CGnuPlotColorSpec &lineColor() const { return lineColor_; }
  void setLineColor(const CGnuPlotColorSpec &c) { lineColor_ = c; }

  const CGnuPlotDash &lineDash() const { return lineDash_; }
  void setLineDash(const CGnuPlotDash &d) { lineDash_ = d; }

 private:
  COptReal          lineWidth_;
  COptInt           lineStyle_;
  COptInt           lineType_;
  CGnuPlotColorSpec lineColor_;
  CGnuPlotDash      lineDash_;
};

#endif
