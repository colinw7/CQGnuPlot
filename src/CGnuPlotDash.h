#ifndef CGnuPlotDash_H
#define CGnuPlotDash_H

#include <CGnuPlotTypes.h>

#include <ostream>
#include <optional>

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

  const std::optional<int> &ind() const { return ind_; }
  void setInd(int ind) { reset(); ind_ = ind; }

  const std::optional<CLineDash> &dash() const { return dash_; }
  void setDash(const CLineDash &dash) { reset(); dash_ = dash; }

  CLineDash calcDash(CGnuPlot *plot, const CLineDash &dash=CLineDash()) const;

  bool isValid() const { return ind_ || dash_; }

  void reset() { ind_.reset(); dash_.reset(); }

  friend std::ostream &operator<<(std::ostream &os, const CGnuPlotDash &dash) {
    dash.print(os);

    return os;
  }

  void print(std::ostream &os=std::cout) const;

 private:
  std::optional<int>       ind_;
  std::optional<CLineDash> dash_;
};

#endif
