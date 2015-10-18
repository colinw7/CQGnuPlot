#ifndef CGnuPlotStyleAdjacency_H
#define CGnuPlotStyleAdjacency_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleAdjacencyRenderer;

class CGnuPlotStyleAdjacency : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleAdjacency();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }

  bool mouseTip(CGnuPlotPlot *plot, const CGnuPlotTypes::InsideData &insideData,
                CGnuPlotTipData &tipData) override;

  void mousePress(CGnuPlotPlot *plot, const CGnuPlotTypes::InsideData &insideData) override;
};

#endif
