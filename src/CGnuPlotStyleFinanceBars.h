#ifndef CGnuPlotStyleFinanceBars_H
#define CGnuPlotStyleFinanceBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleFinanceBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleFinanceBars();

  int numUsing() const override { return 5; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
