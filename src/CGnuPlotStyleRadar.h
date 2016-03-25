#ifndef CGnuPlotStyleRadar_H
#define CGnuPlotStyleRadar_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotRadarStyleValue;

class CGnuPlotStyleRadar : public CGnuPlotStyleBase {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotStyleRadar();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

 private:
  double getRange(CGnuPlotPlot *plot) const;

  void getPointsColor(CGnuPlotRadarStyleValue *value, int pi, CRGBA &lc, CRGBA &fc);

  CPoint2D radarPoint(const CPoint2D &o, double r, double a) const;

  std::vector<CPoint2D> radarPoints(double a0, const CPoint2D &o, double r, int np) const;
};

#endif
