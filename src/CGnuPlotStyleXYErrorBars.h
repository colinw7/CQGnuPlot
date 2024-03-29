#ifndef CGnuPlotStyleXYErrorBars_H
#define CGnuPlotStyleXYErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXYErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXYErrorBars();

  int numUsing() const override { return 4; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;
};

#endif
