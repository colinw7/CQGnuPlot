#ifndef CGnuPlotSampleVars_H
#define CGnuPlotSampleVars_H

struct CGnuPlotSampleVar {
  std::string var;
  COptReal    min;
  COptReal    max;
};

struct CGnuPlotSampleVars {
  CGnuPlotSampleVar x;
  CGnuPlotSampleVar y;
  CGnuPlotSampleVar z;
};

#endif
