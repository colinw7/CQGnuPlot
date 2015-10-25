#ifndef CGnuPlotStyleDelaunay_H
#define CGnuPlotStyleDelaunay_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleDelaunay : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleDelaunay();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
