#ifndef CGnuPlotStyleCandlesticks_H
#define CGnuPlotStyleCandlesticks_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleCandlesticks : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleCandlesticks();

  int numUsing() const override { return 5; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
