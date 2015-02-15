#ifndef CGnuPlotKeyData_H
#define CGnuPlotKeyData_H

#include <CGnuPlotColorSpec.h>
#include <CAlignType.h>
#include <COptVal.h>

class CGnuPlotKeyData {
 public:
  typedef std::vector<std::string> Columns;

 public:
  CGnuPlotKeyData() { }

  bool displayed() const { return displayed_; }
  void setDisplayed (bool b) { displayed_ = b; }

  bool outside() const { return outside_; }
  void setOutside(bool b) { outside_ = b; }

  CHAlignType halign() const { return halign_; }
  void setHAlign(const CHAlignType &a) { halign_ = a; }

  CVAlignType valign() const { return valign_; }
  void setVAlign(const CVAlignType &a) { valign_ = a; }

  bool vertical() const { return vertical_; }
  void setVertical(bool b) { vertical_ = b; }

  bool right() const { return right_; }
  void setRight(bool b) { right_ = b; }

  bool reverse() const { return reverse_; }
  void setReverse(bool b) { reverse_ = b; }

  bool invert() const { return invert_; }
  void setInvert(bool b) { invert_ = b; }

  bool autotitle() const { return autotitle_; }
  void setAutotitle(bool b) { autotitle_ = b; }

  bool opaque() const { return opaque_; }
  void setOpaque(bool b) { opaque_ = b; }

  const COptReal &sampLen() const { return sampLen_; }
  void setSampLen(double r) { sampLen_ = r; }

  const COptReal &spacing() const { return spacing_; }
  void setSpacing(double r) { spacing_ = r; }

  const COptString &title() const { return title_; }
  void setTitle(const std::string &s) { title_ = s; }

  bool box() const { return box_; }
  void setBox(bool b) { box_ = b; }

  bool columnhead() const { return columnhead_; }
  void setColumnhead(bool b) { columnhead_ = b; }

  const COptInt &columnNum() const { return columnNum_; }
  void setColumnNum(int i) { columnNum_ = i; }

  const Columns &columns() const { return columns_; }
  void setColumns(const Columns &c) { columns_ = c; }

  const COptInt &lineType() const { return lineType_; }
  void setLineType(int lt) { lineType_ = lt; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &c) { textColor_ = c; }

 private:
  bool              displayed_  { true };               // key displayed
  bool              outside_    { false };              // inside/outside plot area
  CHAlignType       halign_     { CHALIGN_TYPE_RIGHT }; // key horizontal side
  CVAlignType       valign_     { CVALIGN_TYPE_TOP   }; // key vertical side
  bool              vertical_   { true };               // ??
  bool              right_      { true };               // text justification
  bool              reverse_    { false };              // reverse text and sample
  bool              invert_     { false };              // invert plot order
  bool              autotitle_  { true };
  bool              opaque_     { false };              // draw opaque
  COptReal          sampLen_;
  COptReal          spacing_;
  COptString        title_;
  bool              box_ { false };
  bool              columnhead_ { false };
  COptInt           columnNum_;
  Columns           columns_;
  COptInt           lineType_;
  CGnuPlotColorSpec textColor_;
};

#endif
