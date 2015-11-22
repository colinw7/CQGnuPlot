#ifndef CGnuPlotPoint_H
#define CGnuPlotPoint_H

#include <CExpr.h>
#include <CPoint2D.h>
#include <CPoint3D.h>
#include <CMathGen.h>
#include <CRGBA.h>

class CGnuPlotPoint {
 public:
  typedef std::vector<CExprValuePtr>          Values;
  typedef std::map<std::string,CExprValuePtr> Params;

 public:
  CGnuPlotPoint(const Values &values=Values(), bool discontinuity=false,
                bool bad=false, const Params &params=Params());
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

  bool getXY(double &x, double &y) const { return getX(x) && getY(y); }

  bool getXYZ(double &x, double &y, double &z) const { return getX(x) && getY(y) && getZ(z); }

  bool getReals(std::vector<double> &reals) const;

  bool getPoint(CPoint2D &p, bool checkNaN=true) const;
  bool getPoint(double x, CPoint2D &p, bool checkNaN=true) const;

  bool getPoint(CPoint3D &p) const;

  int getNumValues() const { return values_.size(); }

  bool getValue(int n, double &r) const;
  bool getValue(int n, int &i) const;
  bool getValue(int n, std::string &str) const;

  double getReal(int n) const { double r; getValue(n, r); return r; }

  const std::string &label() const { return label_; }
  void setLabel(const std::string &str) { label_ = str; }

  bool isDiscontinuity() const { return discontinuity_; }
  void setDiscontinuity(bool b) { discontinuity_ = b; }

  bool isBad() const { return bad_; }
  void setBad(bool b) { bad_ = b; }

  bool hasParam(const std::string &name) const;

  CExprValuePtr getParam(const std::string &name) const;

  std::string getParamString(const std::string &name) const;

  CRGBA getParamColor(const std::string &name) const;

  int cmp(const CGnuPlotPoint &p) const;

  void print(std::ostream &os) const;

  bool operator< (const CGnuPlotPoint &p) const { return (cmp(p) < 0); }
  bool operator> (const CGnuPlotPoint &p) const { return (cmp(p) > 0); }
  bool operator==(const CGnuPlotPoint &p) const { return (cmp(p) == 0); }

  bool operator>=(const CGnuPlotPoint &p) const { return ! (*this < p); }
  bool operator<=(const CGnuPlotPoint &p) const { return ! (*this > p); }

  bool operator!=(const CGnuPlotPoint &p) const { return ! (*this == p); }

 private:
  Values      values_;
  std::string label_;
  bool        discontinuity_;
  bool        bad_;
  Params      params_;
};

#endif
