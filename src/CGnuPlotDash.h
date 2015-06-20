#ifndef CGnuPlotDash_H
#define CGnuPlotDash_H

#include <ostream>
#include <CGnuPlotTypes.h>

class CGnuPlot;

class CGnuPlotDash {
 public:
  CGnuPlotDash() { }

  explicit CGnuPlotDash(int ind) {
    setInd(ind);
  }

  explicit CGnuPlotDash(const CLineDash &dash) {
    setDash(dash);
  }

  const COptInt &ind() const { return ind_; }
  void setInd(int ind) { reset(); ind_ = ind; }

  const COptLineDash &dash() const { return dash_; }
  void setDash(const CLineDash &dash) { reset(); dash_ = dash; }

  CLineDash calcDash(CGnuPlot *plot, const CLineDash &dash=CLineDash()) const;

  bool isValid() const { return ind_.isValid() || dash_.isValid(); }

  void reset() { ind_.setInvalid(); dash_.setInvalid(); }

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotDash &dash) {
    dash.print(os);

    return os;
  }

  void print(std::ostream &os=std::cout) const;

 private:
  COptInt      ind_;
  COptLineDash dash_;
};

#endif
