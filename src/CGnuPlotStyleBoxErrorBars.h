#ifndef CGnuPlotStyleBoxErrorBars_H
#define CGnuPlotStyleBoxErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxErrorBars();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  double getRange(CGnuPlotPlot *plot) const;
};

#endif
