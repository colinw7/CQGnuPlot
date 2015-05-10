#ifndef CGnuPlotStyleBoxXYErrorBars_H
#define CGnuPlotStyleBoxXYErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxXYErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxXYErrorBars();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
