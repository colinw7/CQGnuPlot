#ifndef CGnuPlotStyleXYErrorLines_H
#define CGnuPlotStyleXYErrorLines_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXYErrorLines : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXYErrorLines();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
