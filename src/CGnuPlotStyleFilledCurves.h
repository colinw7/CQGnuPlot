#ifndef CGnuPlotStyleFilledCurves_H
#define CGnuPlotStyleFilledCurves_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleFilledCurves : public CGnuPlotStyleBase {
 public:
  typedef std::vector<CPoint2D> Points;
  typedef std::vector<Points>   PointsArray;

  CGnuPlotStyleFilledCurves();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  void addPolygons(CGnuPlotPlot *plot, const Points &points, PointsArray &pointsArray);
  void drawPolygon(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, int i, Points &points);
};

#endif
