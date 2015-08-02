#ifndef CGnuPlotStyleEllipses_H
#define CGnuPlotStyleEllipses_H

#include <CGnuPlotStyleBase.h>
#include <map>

class CGnuPlotStyleEllipses : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleEllipses();

  int numUsing() const override { return 3; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool hasKeyLine() const { return true; }

  void drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                   const CPoint2D &p1, const CPoint2D &p2) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  typedef std::map<int,CRGBA> ColorMap;

  ColorMap keyColor_;
};

#endif
