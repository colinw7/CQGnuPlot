#ifndef CGnuPlotStyleAdjacency_H
#define CGnuPlotStyleAdjacency_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleAdjacency : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleAdjacency();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif
