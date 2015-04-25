#ifndef CGnuPlotStyleCircles_H
#define CGnuPlotStyleCircles_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleCircles : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleCircles();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
