#ifndef CGnuPlotStyleBubblePlot_H
#define CGnuPlotStyleBubblePlot_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBubblePlot : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBubblePlot();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif
