#ifndef CGnuPlotStyleXErrorBars_H
#define CGnuPlotStyleXErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXErrorBars();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
