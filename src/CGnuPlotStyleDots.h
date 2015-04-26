#ifndef CGnuPlotStyleDots_H
#define CGnuPlotStyleDots_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleDots : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleDots();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
