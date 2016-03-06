#ifndef CGnuPlotStyleForceDirected_H
#define CGnuPlotStyleForceDirected_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleForceDirectedRenderer;

class CGnuPlotStyleForceDirected : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleForceDirected();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  bool isSingleType() const override { return true; }

  void drawKey(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) override { }

  CBBox2D fit(CGnuPlotPlot *plot) override;

  void mousePress  (CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent) override;
  void mouseMove   (CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent,
                    bool pressed) override;
  void mouseRelease(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent) override;

  bool isAnimated() const override { return true; }

  int animateTimeout() const override { return 30; }

  void animate(CGnuPlotPlot *plot) const override;

 private:
  bool pressed_ { false };
};

#endif
