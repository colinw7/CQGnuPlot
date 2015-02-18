#ifndef CGnuPlotSize_H
#define CGnuPlotSize_H

#include <CGnuPlotTypes.h>
#include <CSize2D.h>

class CGnuPlotRenderer;

class CGnuPlotSize {
 public:
  typedef CGnuPlotTypes::CoordSys CoordSys;

 public:
  CGnuPlotSize(const CSize2D &s=CSize2D(0,0), CoordSys sys=CoordSys::FIRST) :
   s_(s), systemX_(sys), systemY_(sys) {
  }

  const CSize2D &size() const { return s_; }
  void setSize(const CSize2D &s) { s_ = s; }

  CGnuPlotTypes::CoordSys systemX() const { return systemX_; }
  void setSystemX(const CGnuPlotTypes::CoordSys &s) { systemX_ = s; }

  CGnuPlotTypes::CoordSys systemY() const { return systemY_; }
  void setSystemY(const CGnuPlotTypes::CoordSys &s) { systemY_ = s; }

  CSize2D getSize(CGnuPlotRenderer *renderer) const;

 private:
  CSize2D  s_       { 0, 0 };
  CoordSys systemX_ { CoordSys::FIRST };
  CoordSys systemY_ { CoordSys::FIRST };
};

#endif
