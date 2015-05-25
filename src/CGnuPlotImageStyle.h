#ifndef CGnuPlotImageStyle_H
#define CGnuPlotImageStyle_H

#include <COptVal.h>
#include <CGnuPlotTypes.h>

struct CGnuPlotImageStyle {
  typedef CGnuPlotTypes::ImageType ImageType;

  COptInt           w;
  COptInt           h;
  COptPoint2D       o;
  COptPoint2D       c;
  COptReal          dx, dy;
  COptReal          a;
  bool              flipy { false };
  CGnuPlotUsingCols usingCols;
  ImageType         fileType { CGnuPlotTypes::ImageType::NONE };

  void reset() {
    w .setInvalid();
    h .setInvalid();
    o .setInvalid();
    c .setInvalid();
    dx.setInvalid();
    dy.setInvalid();
    a .setInvalid();

    flipy     = false;
    usingCols = CGnuPlotUsingCols();
    fileType  = CGnuPlotTypes::ImageType::NONE;
  }
};


#endif
