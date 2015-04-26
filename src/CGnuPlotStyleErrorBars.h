#ifndef CGnuPlotStyleErrorBars_H
#define CGnuPlotStyleErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleErrorBars();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
