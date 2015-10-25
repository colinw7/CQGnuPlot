#ifndef CGnuPlotStyleYErrorLines_H
#define CGnuPlotStyleYErrorLines_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleYErrorLines : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleYErrorLines();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
