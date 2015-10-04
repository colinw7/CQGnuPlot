#ifndef CGnuPlotStyleBoxes_H
#define CGnuPlotStyleBoxes_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleBoxes : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBoxes();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const override { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  double getRange(CGnuPlotPlot *plot) const;
};

#endif
