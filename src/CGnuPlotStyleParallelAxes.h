#ifndef CGnuPlotStyleParallelAxes_H
#define CGnuPlotStyleParallelAxes_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleParallelAxes : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleParallelAxes();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
