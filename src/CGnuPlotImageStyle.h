#ifndef CGnuPlotImageStyle_H
#define CGnuPlotImageStyle_H

#include <COptVal.h>
#include <CGnuPlotTypes.h>

struct CGnuPlotImageStyle {
  typedef CGnuPlotTypes::ImageType ImageType;

  COptInt            w;
  COptInt            h;
  COptValT<CPoint2D> o;
  COptValT<CPoint2D> c;
  COptReal           dx, dy;
  COptReal           a;
  bool               flipy { false };
  std::string        format { "" };
  CGnuPlotUsingCols  usingCols;
  ImageType          fileType { CGnuPlotTypes::ImageType::NONE };
};


#endif
