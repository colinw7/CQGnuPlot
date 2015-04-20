#ifndef CGnuPlotStyleParallelAxes_H
#define CGnuPlotStyleParallelAxes_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleParallelAxes : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleParallelAxes();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
