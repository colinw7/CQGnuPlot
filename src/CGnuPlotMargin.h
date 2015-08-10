#ifndef CGnuPlotMargin_H
#define CGnuPlotMargin_H

#include <CGnuPlotTypes.h>

class CGnuPlotRenderer;

class CGnuPlotMarginValue {
 public:
  CGnuPlotMarginValue() { }

  CGnuPlotMarginValue(double v, bool s=false) { value_ = v; screen_ = s; }

  const COptReal &value() const { return value_; }

  void setValue(double v, bool b) { value_ = v; screen_ = b; }

  void resetValue() { value_.setInvalid(); screen_ = false; }

  bool isScreen() const { return screen_; }

  double xValue(CGnuPlotRenderer *renderer) const;
  double yValue(CGnuPlotRenderer *renderer) const;

  void show(std::ostream &os, const std::string &id) {
    os << id << " is ";

    if (! value_.isValid())
      os << "computed automatically" << std::endl;
    else {
      os << "set to";

      if (screen_)
        os << " screen";

      os << " " << value_.getValue() << std::endl;
    }
  }

 private:
  COptReal value_;
  bool     screen_ { false };
};

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

  const COptReal &leftValue  () const { return lmargin_.value(); }
  const COptReal &bottomValue() const { return bmargin_.value(); }
  const COptReal &rightValue () const { return rmargin_.value(); }
  const COptReal &topValue   () const { return tmargin_.value(); }

  void setLeft  (double l, bool s=false) { lmargin_.setValue(l, s); }
  void setBottom(double b, bool s=false) { bmargin_.setValue(b, s); }
  void setRight (double r, bool s=false) { rmargin_.setValue(r, s); }
  void setTop   (double t, bool s=false) { tmargin_.setValue(t, s); }

  void resetLeft  () { lmargin_.resetValue(); }
  void resetBottom() { bmargin_.resetValue(); }
  void resetRight () { rmargin_.resetValue(); }
  void resetTop   () { tmargin_.resetValue(); }

  void reset() { resetLeft(); resetBottom(); resetRight(); resetTop(); }

  void showLeft  (std::ostream &os) { lmargin_.show(os, "lmargin"); }
  void showBottom(std::ostream &os) { bmargin_.show(os, "bmargin"); }
  void showRight (std::ostream &os) { rmargin_.show(os, "rmargin"); }
  void showTop   (std::ostream &os) { tmargin_.show(os, "tmargin"); }

 private:
  CGnuPlotMarginValue lmargin_;
  CGnuPlotMarginValue bmargin_;
  CGnuPlotMarginValue rmargin_;
  CGnuPlotMarginValue tmargin_;
};

#endif
