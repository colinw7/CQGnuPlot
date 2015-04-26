#ifndef CGnuPlotStyleTestTerminal_H
#define CGnuPlotStyleTestTerminal_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleTestTerminal : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleTestTerminal();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
