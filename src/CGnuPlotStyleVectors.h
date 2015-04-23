#ifndef CGnuPlotStyleVectors_H
#define CGnuPlotStyleVectors_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleVectors : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleVectors();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
