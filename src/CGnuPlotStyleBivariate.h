#ifndef CGnuPlotStyleBivariate_H
#define CGnuPlotStyleBivariate_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBivariate : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBivariate();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
