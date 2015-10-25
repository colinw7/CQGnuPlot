#ifndef CGnuPlotStyleRadar_H
#define CGnuPlotStyleRadar_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleRadar : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleRadar();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  double getRange(CGnuPlotPlot *plot) const;
};

#endif
