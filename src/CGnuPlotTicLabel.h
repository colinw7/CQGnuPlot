#ifndef CGnuPlotTicLabel_H
#define CGnuPlotTicLabel_H

#include <CGnuPlotTypes.h>

struct CGnuPlotTicLabel {
  typedef CGnuPlotTypes::AxisType AxisType;

  bool        valid;
  AxisType    id;
  int         ind;
  std::string str;

  CGnuPlotTicLabel() {
    valid = false;
    id    = AxisType::X;
    ind   = 1;
    str   = "";
  }
};

#endif
