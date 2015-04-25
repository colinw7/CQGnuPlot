#ifndef CGnuPlotStyleCandlesticks_H
#define CGnuPlotStyleCandlesticks_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleCandlesticks : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleCandlesticks();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
