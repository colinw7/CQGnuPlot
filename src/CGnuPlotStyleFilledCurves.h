#ifndef CGnuPlotStyleFilledCurves_H
#define CGnuPlotStyleFilledCurves_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleFilledCurves : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleFilledCurves();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
