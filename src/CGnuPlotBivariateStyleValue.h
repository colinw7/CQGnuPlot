#ifndef CGnuPlotBivariateStyleValue_H
#define CGnuPlotBivariateStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CGnuPlotPlot.h>

class CGnuPlotBivariateStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotBivariateStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotBivariateStyleValue() { }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() { }

 private:
  bool inited_ { false };
};

#endif
