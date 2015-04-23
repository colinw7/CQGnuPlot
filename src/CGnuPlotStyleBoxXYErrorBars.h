#ifndef CGnuPlotStyleBoxXYErrorBars_H
#define CGnuPlotStyleBoxXYErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxXYErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxXYErrorBars();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
