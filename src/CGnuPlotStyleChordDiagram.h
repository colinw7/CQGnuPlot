#ifndef CGnuPlotStyleChordDiagram_H
#define CGnuPlotStyleChordDiagram_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleChordDiagram : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleChordDiagram();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent,
                CGnuPlotTipData &tipData) override;

  void mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent) override;
};

#endif
