#ifndef CGnuPlotStyleStackedArea_H
#define CGnuPlotStyleStackedArea_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleStackedArea : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleStackedArea();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
