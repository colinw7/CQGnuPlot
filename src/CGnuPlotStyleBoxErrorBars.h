#ifndef CGnuPlotStyleBoxErrorBars_H
#define CGnuPlotStyleBoxErrorBars_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxErrorBars : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxErrorBars();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

 private:
  double getRange(CGnuPlotPlot *plot) const;
};

#endif
