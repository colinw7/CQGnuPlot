#ifndef CGnuPlotStyleBase_H
#define CGnuPlotStyleBase_H

#include <CBBox2D.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotMouseEvent.h>
#include <CGnuPlotProbeEvent.h>

class CGnuPlotPlot;
class CGnuPlotRenderer;
class CGnuPlotTipData;

class CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBase(CGnuPlotTypes::PlotStyle style) :
   style_(style) {
  }

  virtual ~CGnuPlotStyleBase() { }

  CGnuPlotTypes::PlotStyle style() const { return style_; }

  // if single type is true, must define drawKey, drawAxis and fit
  virtual bool isSingleType() const { return false; }

  virtual bool has3D() const { return false; }

  virtual int numUsing() const = 0;

  virtual void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) = 0;

  virtual void draw3D(CGnuPlotPlot *, CGnuPlotRenderer *) {
    std::cerr << "draw3D not implemented" << std::endl;
  }

  virtual void drawKey(CGnuPlotPlot *, CGnuPlotRenderer *) {
    std::cerr << "drawKey not implemented" << std::endl;
  }

  virtual bool hasKeyLine() const { return false; }

  virtual void drawKeyLine(CGnuPlotPlot *, CGnuPlotRenderer *,
                           const CPoint2D &, const CPoint2D &) {
    std::cerr << "drawKeyLine not implemented" << std::endl;
  }

  virtual void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) {
    std::cerr << "drawAxes not implemented" << std::endl;
  }

  virtual CBBox2D fit(CGnuPlotPlot *) { return CBBox2D(); }

  virtual bool mouseTip(CGnuPlotPlot *, const CGnuPlotMouseEvent &,
                        CGnuPlotTipData &) { return false; }

  virtual void mousePress  (CGnuPlotPlot *, const CGnuPlotMouseEvent &) { }
  virtual void mouseMove   (CGnuPlotPlot *, const CGnuPlotMouseEvent &, bool) { }
  virtual void mouseRelease(CGnuPlotPlot *, const CGnuPlotMouseEvent &) { }

  virtual bool mouseProbe(CGnuPlotPlot *, CGnuPlotProbeEvent &) { return false; }

  virtual bool isAnimated() const { return false; }

  virtual int animateTimeout() const { return 30; }

  virtual void animate(CGnuPlotPlot *) const { }

 protected:
  CGnuPlotTypes::PlotStyle style_;
};

//------

class CGnuPlotStylePointsBase : public CGnuPlotStyleBase {
 public:
  CGnuPlotStylePointsBase(CGnuPlotTypes::PlotStyle style);

  virtual bool mouseProbe(CGnuPlotPlot *plot, CGnuPlotProbeEvent &probeEvent) override;
};

#endif
