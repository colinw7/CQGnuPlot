#ifndef CGnuPlotDelaunayStyleValue_H
#define CGnuPlotDelaunayStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CDelaunay.h>
#include <CRGBA.h>

class CGnuPlotDelaunayStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotDelaunayStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
    delaunay_ = new CDelaunay;
  }

 ~CGnuPlotDelaunayStyleValue() {
    delete delaunay_;
  }

  CDelaunay *delaunay() const { return delaunay_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  const CRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const CRGBA &v) { lineColor_ = v; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  void init() {
    delete delaunay_;

    delaunay_ = new CDelaunay;
  }

 private:
  CDelaunay *delaunay_  { nullptr };
  bool       inited_    { false };
  bool       clipped_   { false };
  CRGBA      lineColor_ { 0, 0, 0.8 };
  double     lineWidth_ { 1 };
};

#endif
