#ifndef CGnuPlotStyleYErrorBars_H
#define CGnuPlotStyleYErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleYErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleYErrorBars();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
