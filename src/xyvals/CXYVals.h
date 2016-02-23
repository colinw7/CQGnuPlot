#ifndef CXY_VALS_H
#define CXY_VALS_H

#include <vector>
#include <iostream>
#include <cstring>
#include <sys/types.h>

class CXYVals {
 public:
  CXYVals(const double *x=0, const double *y=0, uint num_xy=0);

  CXYVals(const double *x1, const double *y1, uint num_xy1,
          const double *x2, const double *y2, uint num_xy2);

 ~CXYVals() { }

  const std::vector<double> &xvals() const { return xvals_; }
  const std::vector<double> &yvals() const { return yvals_; }

  const double *getXVals   () const { return &xvals_[0]; }
  uint          getNumXVals() const { return xvals_.size(); }

  const double *getYVals   () const { return &yvals_[0]; }
  uint          getNumYVals() const { return yvals_.size(); }

  void init(const std::vector<double> &x, std::vector<double> &y);
  void init(const std::vector<double> &x1, std::vector<double> &y1,
            const std::vector<double> &x2, std::vector<double> &y2);

  void init(const double *x, const double *y, uint num_xy);
  void init(const double *x1, const double *y1, uint num_xy1,
            const double *x2, const double *y2, uint num_xy2);

 private:
  void insertValue(double v, double *vals, uint &num_vals);

 protected:
  std::vector<double> xvals_;
  std::vector<double> yvals_;
};

class CXYValsInside : public CXYVals {
 public:
  enum {
    INSIDE1 = (1<<0),
    INSIDE2 = (1<<1)
  };

  typedef std::vector< std::vector<int> > InsideArray;

 public:
  CXYValsInside(const double *x=0, const double *y=0, uint num_xy=0,
                bool init=false);

  CXYValsInside(const double *x1, const double *y1, uint num_xy1,
                const double *x2, const double *y2, uint num_xy2,
                bool init=false);

 ~CXYValsInside() { }

  void initValues(const std::vector<double> &x, std::vector<double> &y);
  void initValues(const std::vector<double> &x1, std::vector<double> &y1,
                  const std::vector<double> &x2, std::vector<double> &y2);

  void initValues(const CXYValsInside &xyvals, const std::vector<double> &x,
                  std::vector<double> &y);
  void initValues(const CXYValsInside &xyvals, const double *x, const double *y, uint num_xy);

  void initValues(const double *x, const double *y, uint num_xy);
  void initValues(const double *x1, const double *y1, uint num_xy1,
                  const double *x2, const double *y2, uint num_xy2);

  const InsideArray &getInside() const { return inside_; }

  InsideArray &getInside() { return inside_; }

  bool isInside1(uint i, uint j) const { return (inside_[i][j] & CXYValsInside::INSIDE1); }
  bool isInside2(uint i, uint j) const { return (inside_[i][j] & CXYValsInside::INSIDE2); }

  bool isInside(uint i, uint j, int val=INSIDE1) const { return (inside_[i][j] == val); }

  void setInside(int val=INSIDE1);

  void combineInside(int val=INSIDE1);

  bool getPoly(std::vector<double> &x, std::vector<double> &y, bool check_consistent=false) const;
  bool getPoly(int inside_val, std::vector<double> &x, std::vector<double> &y,
               bool check_consistent=false) const;

  bool getPoly(double **xo, double **yo, uint *num_xyo, bool check_consistent=false) const;
  bool getPoly(int inside_val, double **xo, double **yo, uint *num_xyo,
               bool check_consistent=false) const;

  void print(std::ostream &os) const;

 private:
  void initMem();

 private:
  InsideArray inside_;
  uint        num_xvals_ { 0 };
  uint        num_yvals_ { 0 };
};

#endif
