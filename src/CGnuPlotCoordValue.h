#ifndef CGnuPlotCoordValue_H
#define CGnuPlotCoordValue_H

#include <CGnuPlotTypes.h>

#include <ostream>

class CGnuPlotRenderer;

class CGnuPlotCoordValue {
 public:
  typedef CGnuPlotTypes::CoordSys CoordSys;

 public:
  CGnuPlotCoordValue(double v=0.0, CoordSys s=CGnuPlotTypes::CoordSys::FIRST) :
   value_(v), system_(s) {
  }

  double value() const { return value_; }
  void setValue(double v) { value_ = v; }

  CoordSys system() const { return system_; }
  void setSystem(CoordSys s) { system_ = s; }

  double pixelXValue(CGnuPlotRenderer *renderer) const;

  void print(std::ostream &os, const std::string &axis="x") const {
    if      (system_ == CGnuPlotTypes::CoordSys::SECOND)
      os << "(second " << axis << " axis) ";
    else if (system_ == CGnuPlotTypes::CoordSys::GRAPH)
      os << "(graph units) ";
    else if (system_ == CGnuPlotTypes::CoordSys::SCREEN)
      os << "(screen units) ";
    else if (system_ == CGnuPlotTypes::CoordSys::CHARACTER)
      os << "(character units) ";

    os << value_;
  }

 private:
  double   value_  { 0.0 };
  CoordSys system_ { CoordSys::FIRST };
};

#endif
