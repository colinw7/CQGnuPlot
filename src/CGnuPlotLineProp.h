#ifndef CGnuPlotLineProp_H
#define CGnuPlotLineProp_H

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotDash.h>

class CGnuPlot;

class CGnuPlotLineProp {
 public:
  CGnuPlotLineProp() { }

  double lineWidth(CGnuPlot *plot) const;
  void setLineWidth(double w) { lineWidth_ = w; }

  const std::optional<int> &lineStyle() const { return lineStyle_; }
  void setLineStyle(int t) { lineStyle_ = t; }

  const std::optional<int> &lineType() const { return lineType_; }
  void setLineType(int type) { lineType_ = type; }

  const CGnuPlotColorSpec &lineColor() const { return lineColor_; }
  void setLineColor(const CGnuPlotColorSpec &c) { lineColor_ = c; }

  const CGnuPlotDash &lineDash() const { return lineDash_; }
  void setLineDash(const CGnuPlotDash &d) { lineDash_ = d; }

 private:
  std::optional<double> lineWidth_;
  std::optional<int>    lineStyle_;
  std::optional<int>    lineType_;
  CGnuPlotColorSpec     lineColor_;
  CGnuPlotDash          lineDash_;
};

#endif
