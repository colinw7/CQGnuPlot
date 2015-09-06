#ifndef CGnuPlotStyleSunburst_H
#define CGnuPlotStyleSunburst_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleSunburst : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleSunburst();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }
};

#endif
