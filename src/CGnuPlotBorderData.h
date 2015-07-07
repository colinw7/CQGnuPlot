#ifndef CGnuPlotBorderData_H
#define CGnuPlotBorderData_H

#include <CGnuPlotTypes.h>

struct CGnuPlotBorderData {
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

  int       sides     { 0xFF };
  DrawLayer layer     { DrawLayer::FRONT };
  double    lineWidth { 1.0 };
  COptInt   lineStyle;
  COptInt   lineType;

  void unset() {
    sides = 0;
  }

  void show(std::ostream &os) const;
  void save(std::ostream &os) const;
};

#endif
