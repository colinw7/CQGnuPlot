#ifndef CGnuPlotStyleVectors_H
#define CGnuPlotStyleVectors_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleVectors : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleVectors();

  int numUsing() const override { return 4; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;
};

#endif
