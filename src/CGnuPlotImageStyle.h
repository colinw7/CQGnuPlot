#ifndef CGnuPlotImageStyle_H
#define CGnuPlotImageStyle_H

#include <COptVal.h>

struct CGnuPlotImageStyle {
  int                w { 1 };
  int                h { 1 };
  COptValT<CPoint2D> o;
  COptValT<CPoint2D> c;
  COptReal           dx, dy;
  COptReal           a;
  bool               flipy { false };
  std::string        format { "" };
  CGnuPlotUsingCols  usingCols;
};


#endif
