#ifndef CGnuPlotStyleErrorBars_H
#define CGnuPlotStyleErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleErrorBars();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
