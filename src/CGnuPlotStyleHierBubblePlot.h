#ifndef CGnuPlotStyleHierBubblePlot_H
#define CGnuPlotStyleHierBubblePlot_H

#include <CGnuPlotStyleBase.h>
#include <CHierBubblePack.h>

class CGnuPlotStyleHierBubblePlot : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleHierBubblePlot();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  //CBBox2D fit(CGnuPlotPlot *plot) override;

  bool mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent,
                CGnuPlotTipData &tipData);

 private:
  void drawNodes(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer,
                 CHierBubblePack::HierNode *hier, int depth);

  void mapPoint(double x1, double y1, double &x2, double &y2) const;
  void unmapPoint(double x1, double y1, double &x2, double &y2) const;

 private:
  double marginLeft_   { 0.0 };
  double marginRight_  { 0.0 };
  double marginBottom_ { 0.0 };
  double marginTop_    { 0.0 };
};

#endif
