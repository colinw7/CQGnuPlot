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
  class GetOpts {
   public:
    GetOpts() { }

    bool force() const { return force_; }
    bool checkNaN() const { return checkNaN_; }

    GetOpts &setForce   (bool b) { force_    = b; return *this; }
    GetOpts &setCheckNaN(bool b) { checkNaN_ = b; return *this; }

   private:
    bool force_    { false };
    bool checkNaN_ { true };
  };

  CGnuPlotPoint(const Values &values=Values(), bool discontinuity=false,
                bool bad=false, const Params &params=Params());
 ~CGnuPlotPoint();

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotPoint &p) {
    p.print(os); return os;
  }

  bool getX(double &x, const GetOpts &opts=GetOpts()) const;
  bool getY(double &y, const GetOpts &opts=GetOpts()) const;
  bool getZ(double &z, const GetOpts &opts=GetOpts()) const;

  double getX() const;
  double getY() const;
  double getZ() const;

  bool getXY(double &x, double &y, const GetOpts &opts=GetOpts()) const {
    return getX(x, opts) && getY(y, opts);
  }

  bool getXYZ(double &x, double &y, double &z, const GetOpts &opts=GetOpts()) const {
    return getX(x, opts) && getY(y, opts) && getZ(z, opts);
  }

  bool getReals(std::vector<double> &reals, const GetOpts &opts=GetOpts()) const;

  bool getPoint(CPoint2D &p, const GetOpts &opts=GetOpts()) const;
  bool getPoint(double x, CPoint2D &p, const GetOpts &opts=GetOpts()) const;

  bool getPoint(CPoint3D &p, const GetOpts &opts=GetOpts()) const;

  int getNumValues() const { return values_.size(); }

  const CExprValuePtr &getValue(int i) const { return values_[i]; }

  bool getValue(int n, double &r) const;
  bool getValue(int n, int &i) const;
  bool getValue(int n, std::string &str) const;

  double getReal(int n) const { double r; getValue(n, r); return r; }

  bool getForceReal(int n, double &r) const;

  const std::string &label() const { return label_; }
  void setLabel(const std::string &str) { label_ = str; }

  bool isDiscontinuity() const { return discontinuity_; }
  void setDiscontinuity(bool b) { discontinuity_ = b; }

  bool isBad() const { return bad_; }
  void setBad(bool b) { bad_ = b; }

  bool hasParam(const std::string &name) const;

  CExprValuePtr getParam(const std::string &name) const;
  void setParam(const std::string &name, const CExprValuePtr &value);
  void unsetParam(const std::string &name);

  std::string getParamString(const std::string &name) const;

  CRGBA getParamColor(const std::string &name) const;
  void setParamColor(const std::string &name, const CRGBA &c);

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
