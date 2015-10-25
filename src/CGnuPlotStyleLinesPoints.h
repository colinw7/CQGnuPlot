#ifndef CGnuPlotStyleLinesPoints_H
#define CGnuPlotStyleLinesPoints_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleLinesPoints : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleLinesPoints();

  int numUsing() const override { return 2; }

  bool has3D() const override { return true; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
  void draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;
};

#endif
