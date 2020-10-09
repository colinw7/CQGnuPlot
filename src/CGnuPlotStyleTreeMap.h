#ifndef CGnuPlotStyleTreeMap_H
#define CGnuPlotStyleTreeMap_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleTreeMap : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleTreeMap(bool hier=false);

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent,
                CGnuPlotTipData &tipData) override;

  void mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent) override;

 private:
  bool hier_ { false };
};

#endif
