#ifndef CGnuPlotStyleBase_H
#define CGnuPlotStyleBase_H

#include <CBBox2D.h>
#include <CGnuPlotTypes.h>

class CGnuPlotPlot;
class CGnuPlotRenderer;

class CGnuPlotStyleBase {
 public:
  CGnuPlotStyleBase(CGnuPlotTypes::PlotStyle style) :
   style_(style) {
  }

  virtual ~CGnuPlotStyleBase() { }

  CGnuPlotTypes::PlotStyle style() const { return style_; }

  virtual bool isSingleType() const { return false; }

  virtual void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) = 0;

  virtual void draw3D(CGnuPlotPlot *, CGnuPlotRenderer *) { }

  virtual void drawKey(CGnuPlotPlot *, CGnuPlotRenderer *) { }

  virtual bool hasKeyLine() const { return false; }

  virtual void drawKeyLine(CGnuPlotPlot *, CGnuPlotRenderer *,
                           const CPoint2D &, const CPoint2D &) { }

  virtual void drawAxes(CGnuPlotPlot *, CGnuPlotRenderer *) { }

  virtual CBBox2D fit(CGnuPlotPlot *plot) = 0;

 protected:
  CGnuPlotTypes::PlotStyle style_;
};

#endif
