#ifndef CGnuPlotStyleTestPalette_H
#define CGnuPlotStyleTestPalette_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleTestPalette : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleTestPalette();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
