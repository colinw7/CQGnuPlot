#ifndef CGnuPlotStyleImpulses_H
#define CGnuPlotStyleImpulses_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleImpulses : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleImpulses();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
