#ifndef CGnuPlotFilledCurve_H
#define CGnuPlotFilledCurve_H

struct CGnuPlotFilledCurve {
  bool                    closed { true };
  bool                    above  { false  };
  bool                    below  { false  };
  int                     xaxis  { 0 };
  int                     yaxis  { 0 };
  int                     raxis  { 0 };
  std::optional<double>   xval;
  std::optional<double>   yval;
  std::optional<CPoint2D> xyval;
  std::optional<double>   rval;
};

#endif
