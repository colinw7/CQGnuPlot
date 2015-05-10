#ifndef CGnuPlotStyleTreeMap_H
#define CGnuPlotStyleTreeMap_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleTreeMap : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleTreeMap();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }
};

#endif
