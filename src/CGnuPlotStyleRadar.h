#ifndef CGnuPlotStyleRadar_H
#define CGnuPlotStyleRadar_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleRadar : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleRadar();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool isSingleType() const override { return true; }

 private:
  double getRange(CGnuPlotPlot *plot) const;
};

#endif
