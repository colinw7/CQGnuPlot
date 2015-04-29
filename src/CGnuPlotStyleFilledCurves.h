#ifndef CGnuPlotStyleFilledCurves_H
#define CGnuPlotStyleFilledCurves_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleFilledCurves : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleFilledCurves();

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  void drawPolygon(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   int i, std::vector<CPoint2D> &points);
};

#endif
