#ifndef CGnuPlotStyleXErrorLines_H
#define CGnuPlotStyleXErrorLines_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXErrorLines : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXErrorLines();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
