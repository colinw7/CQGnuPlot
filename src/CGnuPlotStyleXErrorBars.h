#ifndef CGnuPlotStyleXErrorBars_H
#define CGnuPlotStyleXErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleXErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleXErrorBars();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
