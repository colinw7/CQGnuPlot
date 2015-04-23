#ifndef CGnuPlotStylePolygons_H
#define CGnuPlotStylePolygons_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStylePolygons : public CGnuPlotStyleBase {
 public:
  CGnuPlotStylePolygons();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
