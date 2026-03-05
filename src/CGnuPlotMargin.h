#ifndef CGnuPlotMargin_H
#define CGnuPlotMargin_H

#include <CGnuPlotTypes.h>

class CGnuPlotRenderer;
class CGnuPlotMargin;

class CGnuPlotMarginValue {
 public:
  CGnuPlotMarginValue(bool low) :
   low_(low) {
  }

  CGnuPlotMarginValue(bool low, double v, bool s=false) :
   low_(low), value_(v), screen_(s) {
  }

  bool isLow() const { return low_; }
  void setLow(bool b) { low_ = b; }

  const std::optional<double> &value() const { return value_; }
  void setValue(double v, bool b) { value_ = v; screen_ = b; }
  void resetValue() { value_.reset(); screen_ = false; }

  double realValue() const { return value_.value_or(defValue_); }

  bool isScreen() const { return screen_; }
  void setScreen(bool b) { screen_ = b; }

  double defValue() const { return defValue_; }
  void setDefValue(double r) { defValue_ = r; }

  double xValue(CGnuPlotRenderer *renderer, const CGnuPlotMargin &margin) const;
  double yValue(CGnuPlotRenderer *renderer, const CGnuPlotMargin &margin) const;

  void show(std::ostream &os, const std::string &id) {
    os << id << " is ";

    if (! value_)
      os << "computed automatically\n";
    else {
      os << "set to";

      if (screen_)
        os << " screen";

      os << " " << value_.value() << "\n";
    }
  }

 private:
  bool                  low_      { true };
  std::optional<double> value_;
  double                defValue_ { 0 };
  bool                  screen_   { false };
};

//---

class CGnuPlotMarginLValue : public CGnuPlotMarginValue {
 public:
  CGnuPlotMarginLValue() : CGnuPlotMarginValue(true) { }

  CGnuPlotMarginLValue(double v, bool s=false) :
   CGnuPlotMarginValue(true, v, s) {
  }
};

//---

class CGnuPlotMarginHValue : public CGnuPlotMarginValue {
 public:
  CGnuPlotMarginHValue() : CGnuPlotMarginValue(false) { }

  CGnuPlotMarginHValue(double v, bool s=false) :
   CGnuPlotMarginValue(false, v, s) {
  }
};

//---

class CGnuPlotMargin {
 public:
  CGnuPlotMargin() { }

  CGnuPlotMargin(double l, double b, double r, double t) :
   lmargin_(l), bmargin_(b), rmargin_(r), tmargin_(t) {
  }

  const CGnuPlotMarginValue &left  () const { return lmargin_; }
  const CGnuPlotMarginValue &bottom() const { return bmargin_; }
  const CGnuPlotMarginValue &right () const { return rmargin_; }
  const CGnuPlotMarginValue &top   () const { return tmargin_; }

  const std::optional<double> &leftValue  () const { return left  ().value(); }
  const std::optional<double> &bottomValue() const { return bottom().value(); }
  const std::optional<double> &rightValue () const { return right ().value(); }
  const std::optional<double> &topValue   () const { return top   ().value(); }

  void setLeft  (double l, bool s=false) { lmargin_.setValue(l, s); }
  void setBottom(double b, bool s=false) { bmargin_.setValue(b, s); }
  void setRight (double r, bool s=false) { rmargin_.setValue(r, s); }
  void setTop   (double t, bool s=false) { tmargin_.setValue(t, s); }

  void resetLeft  () { lmargin_.resetValue(); }
  void resetBottom() { bmargin_.resetValue(); }
  void resetRight () { rmargin_.resetValue(); }
  void resetTop   () { tmargin_.resetValue(); }

  void reset() { resetLeft(); resetBottom(); resetRight(); resetTop(); }

  void updateDefaultValues(CGnuPlotRenderer *renderer, double lm, double bm, double rm, double tm);

  bool hasFontSize() const { return !!fw_; }

  void updateFontSize(CGnuPlotRenderer *renderer) const;

  void showLeft  (std::ostream &os) { lmargin_.show(os, "lmargin"); }
  void showBottom(std::ostream &os) { bmargin_.show(os, "bmargin"); }
  void showRight (std::ostream &os) { rmargin_.show(os, "rmargin"); }
  void showTop   (std::ostream &os) { tmargin_.show(os, "tmargin"); }

  double fontWidth () const { return fw_.value_or(1); }
  double fontHeight() const { return fw_.value_or(1); }

 private:
  CGnuPlotMarginLValue  lmargin_;
  CGnuPlotMarginLValue  bmargin_;
  CGnuPlotMarginHValue  rmargin_;
  CGnuPlotMarginHValue  tmargin_;
  std::optional<double> fw_;
  std::optional<double> fh_;
};

#endif
