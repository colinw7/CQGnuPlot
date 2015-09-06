#ifndef CGnuPlotStyleBase_H
#define CGnuPlotStyleBase_H

#include <CBBox2D.h>
#include <CGnuPlotTypes.h>

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

  virtual CBBox2D fit(CGnuPlotPlot *plot) = 0;

  virtual bool mouseTip(CGnuPlotPlot *, const CPoint2D &, CGnuPlotTipData &) { return false; }

 protected:
  CGnuPlotTypes::PlotStyle style_;
};

#endif
