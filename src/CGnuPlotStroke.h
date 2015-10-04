#ifndef CGnuPlotStroke_H
#define CGnuPlotStroke_H

#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>

class CGnuPlotPlot;

class CGnuPlotStroke {
 public:
  CGnuPlotStroke(CGnuPlotPlot *plot);

  void init(CGnuPlotPlot *plot);

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  const CRGBA &color() const { return color_; }
  void setColor(const CRGBA &color) { color_ = color; }

  double width() const { return width_; }
  void setWidth(double width) { width_ = width; }

  const CLineDash &lineDash() const { return dash_; }
  void setLineDash(const CLineDash &dash) { dash_ = dash; }

  const CLineCapType &lineCap() const { return cap_; }
  void setLineCap(const CLineCapType &cap) { cap_ = cap; }

  const CLineJoinType &lineJoin() const { return join_; }
  void setLineJoin(const CLineJoinType &join) { join_ = join; }

  double mitreLimit() const { return mlimit_; }
  void setMitreLimit(double mlimit) { mlimit_ = mlimit; }

 private:
  bool          enabled_;
  CRGBA         color_   { 0, 0, 0 };
  double        width_   { 1 };
  CLineDash     dash_;
  CLineCapType  cap_     { LINE_CAP_TYPE_SQUARE };
  CLineJoinType join_    { LINE_JOIN_TYPE_MITRE };
  double        mlimit_  { 3 };
};

#endif
