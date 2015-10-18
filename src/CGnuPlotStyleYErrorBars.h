#ifndef CGnuPlotStyleYErrorBars_H
#define CGnuPlotStyleYErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleYErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleYErrorBars();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
