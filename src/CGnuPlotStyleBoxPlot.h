#ifndef CGnuPlotStyleBoxPlot_H
#define CGnuPlotStyleBoxPlot_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxPlot : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxPlot();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
