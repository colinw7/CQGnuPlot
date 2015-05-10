#ifndef CGnuPlotStyleTestPalette_H
#define CGnuPlotStyleTestPalette_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleTestPalette : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleTestPalette();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
