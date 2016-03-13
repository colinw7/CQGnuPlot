#ifndef CGnuPlotForceDirectedStyleValue_H
#define CGnuPlotForceDirectedStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CForceDirected.h>

class CGnuPlotForceDirectedStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotForceDirectedStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
    forceDirected_ = new CForceDirected;
  }

 ~CGnuPlotForceDirectedStyleValue() {
    delete forceDirected_;
  }

  CForceDirected *forceDirected() const { return forceDirected_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    delete forceDirected_;

    forceDirected_ = new CForceDirected;
  }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

  double circleSize() const { return circleSize_; }
  void setCircleSize(double r) { circleSize_ = r; }

  bool isAnimating() const { return animating_; }
  void setAnimating(bool b) { animating_ = b; }

 private:
  CForceDirected *forceDirected_ { 0 };
  bool            inited_        { false };
  std::string     palette_       { "adjacency" };
  double          circleSize_    { 12 };
  bool            animating_     { true };
};

#endif
