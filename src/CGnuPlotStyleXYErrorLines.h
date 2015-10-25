#ifndef CGnuPlotStyleXYErrorLines_H
#define CGnuPlotStyleXYErrorLines_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXYErrorLines : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXYErrorLines();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
