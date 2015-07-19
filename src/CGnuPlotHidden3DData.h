#ifndef CGnuPlotHidden3DData_H
#define CGnuPlotHidden3DData_H

#include <CGnuPlotTypes.h>

struct CGnuPlotHidden3DData {
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

  bool      enabled         { false };
  DrawLayer layer           { DrawLayer::FRONT };
  COptReal  offset;
  int       trianglePattern { 3 };
  COptInt   undefined;
  bool      altdiagonal     { false };
  bool      bentover        { false };
};

#endif
