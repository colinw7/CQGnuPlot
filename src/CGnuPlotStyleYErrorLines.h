#ifndef CGnuPlotStyleYErrorLines_H
#define CGnuPlotStyleYErrorLines_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleYErrorLines : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleYErrorLines();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
