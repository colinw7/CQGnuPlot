#ifndef CGnuPlotStyleBoxes_H
#define CGnuPlotStyleBoxes_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxes : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxes();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  double getRange(CGnuPlotPlot *plot) const;
};

#endif
