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
   p_(p), systemX_(s), systemY_(s) {
  }

  const CPoint2D &point() const { return p_; }
  void setPoint(const CPoint2D &p) { p_ = p; }

  CGnuPlotTypes::CoordSys systemX() const { return systemX_; }
  void setSystemX(const CGnuPlotTypes::CoordSys &s) { systemX_ = s; }

  CGnuPlotTypes::CoordSys systemY() const { return systemY_; }
  void setSystemY(const CGnuPlotTypes::CoordSys &s) { systemY_ = s; }

  CPoint2D getPoint(CGnuPlotRenderer *renderer) const;

 private:
  CPoint2D p_       { 0, 0 };
  CoordSys systemX_ { CoordSys::FIRST };
  CoordSys systemY_ { CoordSys::FIRST };
};

#endif
