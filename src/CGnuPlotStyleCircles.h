#ifndef CGnuPlotStyleCircles_H
#define CGnuPlotStyleCircles_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleCircles : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleCircles();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
