#ifndef CGnuPlotStyleXErrorBars_H
#define CGnuPlotStyleXErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXErrorBars();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
