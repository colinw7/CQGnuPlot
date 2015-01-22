#ifndef CGnuPlotPoint_H
#define CGnuPlotPoint_H

#include <CExpr.h>
#include <CPoint2D.h>

class CGnuPlotPoint {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CGnuPlotPoint(const Values &values=Values(), bool discontinuity=false);
 ~CGnuPlotPoint();

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotPoint &p) {
    p.print(os); return os;
  }

  bool getX(double &x) const;
  bool getY(double &y) const;
  bool getZ(double &z) const;

  double getX() const;
  double getY() const;
  double getZ() const;

  bool getXY(double &x, double &y) const {
    return getX(x) && getY(y);
  }

  bool getXYZ(double &x, double &y, double &z) const {
    return getX(x) && getY(y) && getZ(z);
  }

  bool getReals(std::vector<double> &reals) const {
    reals.clear();

    bool   b = true;
    double r = 0.0;

    for (uint i = 0; i < values_.size(); ++i) {
      if (getValue(i + 1, r))
        reals.push_back(r);
      else
        b = false;
    }

    return b;
  }

  bool getPoint(CPoint2D &p) const {
    double x, y;

    if (! getXY(x, y))
      return false;

    p = CPoint2D(x, y);

    return true;
  }

  int getNumValues() const { return values_.size(); }

  bool getValue(int n, double &r) const;
  bool getValue(int n, int &i) const;
  bool getValue(int n, std::string &str) const;

  bool isDiscontinuity() const { return discontinuity_; }

  int cmp(const CGnuPlotPoint &p) const;

  void print(std::ostream &os) const;

  bool operator< (const CGnuPlotPoint &p) const { return (cmp(p) < 0); }
  bool operator> (const CGnuPlotPoint &p) const { return (cmp(p) > 0); }
  bool operator==(const CGnuPlotPoint &p) const { return (cmp(p) == 0); }

  bool operator>=(const CGnuPlotPoint &p) const { return ! (*this < p); }
  bool operator<=(const CGnuPlotPoint &p) const { return ! (*this > p); }

  bool operator!=(const CGnuPlotPoint &p) const { return ! (*this == p); }

 private:
  Values values_;
  bool   discontinuity_;
};

#endif
