#ifndef CGnuPlotStyleSteps_H
#define CGnuPlotStyleSteps_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleStepsBase : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleStepsBase(CGnuPlotTypes::PlotStyle style);

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

class CGnuPlotStyleSteps : public CGnuPlotStyleStepsBase {
 public:
  CGnuPlotStyleSteps();
};

class CGnuPlotStyleFSteps : public CGnuPlotStyleStepsBase {
 public:
  CGnuPlotStyleFSteps();
};

class CGnuPlotStyleHiSteps : public CGnuPlotStyleStepsBase {
 public:
  CGnuPlotStyleHiSteps();
};

class CGnuPlotStyleFillSteps : public CGnuPlotStyleStepsBase {
 public:
  CGnuPlotStyleFillSteps();
};

#endif
