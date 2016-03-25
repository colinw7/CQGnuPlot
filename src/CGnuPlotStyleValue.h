#ifndef CGnuPlotStyleValue_H
#define CGnuPlotStyleValue_H

class CGnuPlotPlot;

class CGnuPlotStyleValue {
 public:
  CGnuPlotStyleValue(CGnuPlotPlot *plot) :
   plot_(plot) {
 }

  virtual ~CGnuPlotStyleValue() { }

  CGnuPlotPlot *plot() const { return plot_; }

 protected:
  CGnuPlotPlot *plot_ { 0 };
};

#endif
