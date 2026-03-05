#ifndef CGnuPlotSampleVars_H
#define CGnuPlotSampleVars_H

struct CGnuPlotSampleVar {
  std::string           var;
  std::optional<double> min;
  std::optional<double> max;
};

struct CGnuPlotSampleVars {
  CGnuPlotSampleVar x;
  CGnuPlotSampleVar y;
  CGnuPlotSampleVar z;
};

#endif
