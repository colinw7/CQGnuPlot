#ifndef CGnuPlotPosition_H
#define CGnuPlotPosition_H

#include <CGnuPlotTypes.h>
#include <CPoint3D.h>

class CGnuPlotRenderer;

class CGnuPlotPosition {
 public:
  typedef CGnuPlotTypes::CoordSys CoordSys;

 public:
  CGnuPlotPosition(const CPoint3D &p=CPoint3D(0,0,0), CoordSys s=CoordSys::FIRST) :
   p_(p), systemX_(s), systemY_(s), systemZ_(s) {
  }

  CGnuPlotPosition(const CPoint2D &p, CoordSys s=CoordSys::FIRST) :
   p_(CPoint3D(p.x, p.y, 0)), systemX_(s), systemY_(s), systemZ_(s) {
  }

  const CPoint3D &point() const { return p_; }
  void setPoint(const CPoint3D &p) { p_ = p; }

  CGnuPlotTypes::CoordSys systemX() const { return systemX_; }
  void setSystemX(const CGnuPlotTypes::CoordSys &s) { systemX_ = s; }

  CGnuPlotTypes::CoordSys systemY() const { return systemY_; }
  void setSystemY(const CGnuPlotTypes::CoordSys &s) { systemY_ = s; }

  CGnuPlotTypes::CoordSys systemZ() const { return systemZ_; }
  void setSystemZ(const CGnuPlotTypes::CoordSys &s) { systemZ_ = s; }

  bool isScreen() const {
    return (systemX_ == CoordSys::SCREEN ||
            systemY_ == CoordSys::SCREEN ||
            systemZ_ == CoordSys::SCREEN);
  }

  CPoint3D getPoint3D(CGnuPlotRenderer *renderer) const;
  CPoint2D getPoint2D(CGnuPlotRenderer *renderer) const;

  CPoint3D getDistance3D(CGnuPlotRenderer *renderer) const;
  CPoint2D getDistance2D(CGnuPlotRenderer *renderer) const;

  void print(std::ostream &os) const {
    if (systemX_ == systemY_ && systemX_ == systemZ_) {
      if (systemX_ != CoordSys::FIRST)
        printSystem(os, systemX_);

      os << p_;
    }
    else {
      if (systemX_ != CoordSys::FIRST)
        printSystem(os, systemX_);

      os << p_.x << " ";

      if (systemY_ != CoordSys::FIRST)
        printSystem(os, systemY_);

      os << p_.y;

      if (systemZ_ != CoordSys::FIRST)
        printSystem(os, systemZ_);

      os << p_.z;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotPosition &pos) {
    pos.print(os);

    return os;
  }

  static void printSystem(std::ostream &os, const CoordSys &s) {
    switch (s) {
      case CoordSys::FIRST    : os << "first "    ; return;
      case CoordSys::SECOND   : os << "second "   ; return;
      case CoordSys::GRAPH    : os << "graph "    ; return;
      case CoordSys::SCREEN   : os << "screen "   ; return;
      case CoordSys::CHARACTER: os << "character "; return;
    }
  }

 private:
  CPoint3D getPoint(CGnuPlotRenderer *renderer, double x, double y, double z) const;

 private:
  CPoint3D p_       { 0, 0, 0 };
  CoordSys systemX_ { CoordSys::FIRST };
  CoordSys systemY_ { CoordSys::FIRST };
  CoordSys systemZ_ { CoordSys::FIRST };
};

#endif
