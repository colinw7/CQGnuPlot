#ifndef CGnuPlotStyleImpulses_H
#define CGnuPlotStyleImpulses_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleImpulses : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleImpulses();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;
};

#endif
