#ifndef CGnuPlotStyleAdjacency_H
#define CGnuPlotStyleAdjacency_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleAdjacency : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleAdjacency();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif
