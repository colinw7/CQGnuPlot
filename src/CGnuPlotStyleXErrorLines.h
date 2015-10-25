#ifndef CGnuPlotStyleXErrorLines_H
#define CGnuPlotStyleXErrorLines_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXErrorLines : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXErrorLines();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
