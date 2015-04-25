#ifndef CGnuPlotStyleLabels_H
#define CGnuPlotStyleLabels_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleLabels : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleLabels();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
