#ifndef CGnuPlotStyleDelaunay_H
#define CGnuPlotStyleDelaunay_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleDelaunay : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleDelaunay();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif
