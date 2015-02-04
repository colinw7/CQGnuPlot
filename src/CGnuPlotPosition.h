#ifndef CGnuPlotPosition_H
#define CGnuPlotPosition_H

#include <CGnuPlotTypes.h>
#include <CPoint2D.h>

class CGnuPlotRenderer;

class CGnuPlotPosition {
 public:
  typedef CGnuPlotTypes::CoordSys CoordSys;

 public:
  CGnuPlotPosition(const CPoint2D &p=CPoint2D(0,0), CoordSys s=CoordSys::FIRST) :
   p_(p), system_(s) {
  }

  const CPoint2D &point() const { return p_; }
  void setPoint(const CPoint2D &p) { p_ = p; }

  CGnuPlotTypes::CoordSys system() const { return system_; }
  void setSystem(const CGnuPlotTypes::CoordSys &s) { system_ = s; }

  CPoint2D getPoint(CGnuPlotRenderer *renderer) const;

 private:
  CPoint2D p_      { 0, 0 };
  CoordSys system_ { CoordSys::FIRST };
};

#endif
