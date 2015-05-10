#ifndef CGnuPlotStylePieChart_H
#define CGnuPlotStylePieChart_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStylePieChart : public CGnuPlotStyleBase {
 public:
  CGnuPlotStylePieChart();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif
