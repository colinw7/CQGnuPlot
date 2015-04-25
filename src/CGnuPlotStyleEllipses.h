#ifndef CGnuPlotStyleEllipses_H
#define CGnuPlotStyleEllipses_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleEllipses : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleEllipses();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
