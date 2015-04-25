#ifndef CGnuPlotStyleFinanceBars_H
#define CGnuPlotStyleFinanceBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleFinanceBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleFinanceBars();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
