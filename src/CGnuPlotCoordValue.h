#ifndef CGnuPlotCoordValue_H
#define CGnuPlotCoordValue_H

#include <CGnuPlotTypes.h>
#include <ostream>

class CGnuPlotRenderer;

struct CGnuPlotCoordValue {
  double                  value  { 0.0 };
  CGnuPlotTypes::CoordSys system { CGnuPlotTypes::CoordSys::FIRST };

  CGnuPlotCoordValue(double v=0.0, CGnuPlotTypes::CoordSys s=CGnuPlotTypes::CoordSys::FIRST) :
   value(v), system(s) {
  }

  double pixelXValue(CGnuPlotRenderer *renderer) const;

  void print(std::ostream &os, const std::string &axis="x") const {
    if      (system == CGnuPlotTypes::CoordSys::SECOND)
      os << "(second " << axis << " axis) ";
    else if (system == CGnuPlotTypes::CoordSys::GRAPH)
      os << "(graph units) ";
    else if (system == CGnuPlotTypes::CoordSys::SCREEN)
      os << "(screen units) ";
    else if (system == CGnuPlotTypes::CoordSys::CHARACTER)
      os << "(character units) ";

    os << value;
  }
};

#endif
