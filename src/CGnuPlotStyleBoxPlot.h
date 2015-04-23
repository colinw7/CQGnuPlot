#ifndef CGnuPlotStyleBoxPlot_H
#define CGnuPlotStyleBoxPlot_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxPlot : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxPlot();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
