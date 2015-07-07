#ifndef CGnuPlotFilledCurve_H
#define CGnuPlotFilledCurve_H

struct CGnuPlotFilledCurve {
  bool        closed { true };
  bool        above  { false  };
  bool        below  { false  };
  int         xaxis  { 0 };
  int         yaxis  { 0 };
  int         raxis  { 0 };
  COptReal    xval;
  COptReal    yval;
  COptPoint2D xyval;
  COptReal    rval;
};

#endif
