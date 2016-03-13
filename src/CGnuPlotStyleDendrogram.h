#ifndef CGnuPlotStyleDendrogram_H
#define CGnuPlotStyleDendrogram_H

#include <CGnuPlotStyleBase.h>
#include <CDendrogram.h>

class CGnuPlotDendrogramStyleValue;

class CGnuPlotStyleDendrogram : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleDendrogram();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

  bool mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent,
                CGnuPlotTipData &tipData) override;

  void mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent) override;

 private:
  void drawNodes(CGnuPlotRenderer *renderer, CDendrogram::HierNode *hier, int depth);

  void drawNode(CGnuPlotRenderer *renderer, CDendrogram::HierNode *hier, CDendrogram::Node *node);

  void mapPoint(double x1, double y1, double &x2, double &y2) const;
  void unmapPoint(double x1, double y1, double &x2, double &y2) const;

 private:
  CGnuPlotDendrogramStyleValue *value_ { 0 };
};

#endif
