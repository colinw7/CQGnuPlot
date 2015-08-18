#ifndef CGnuPlotKeyData_H
#define CGnuPlotKeyData_H

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotPosition.h>
#include <CAlignType.h>
#include <COptVal.h>
#include <CFont.h>

class CGnuPlotKeyData {
 public:
  typedef std::vector<std::string>   Columns;
  typedef COptValT<CGnuPlotPosition> OptPos;

 public:
  CGnuPlotKeyData();

  bool displayed() const { return displayed_; }
  void setDisplayed (bool b) { displayed_ = b; }

  bool outside() const { return outside_; }
  void setOutside(bool b) { outside_ = b; }

  bool lmargin() const { return lmargin_; }
  void setLMargin(bool b) { lmargin_ = b; }
  bool rmargin() const { return rmargin_; }
  void setRMargin(bool b) { rmargin_ = b; }
  bool tmargin() const { return tmargin_; }
  void setTMargin(bool b) { tmargin_ = b; }
  bool bmargin() const { return bmargin_; }
  void setBMargin(bool b) { bmargin_ = b; }

  CHAlignType halign() const { return halign_; }
  void setHAlign(const CHAlignType &a) { halign_ = a; }

  CVAlignType valign() const { return valign_; }
  void setVAlign(const CVAlignType &a) { valign_ = a; }

  bool vertical() const { return vertical_; }
  void setVertical(bool b) { vertical_ = b; }

  CHAlignType justify() const { return justify_; }
  void setJustify(CHAlignType j) { justify_ = j; }

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

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  int widthIncrement() const { return widthIncrement_; }
  void setWidthIncrement(int i) { widthIncrement_ = i; }

  int heightIncrement() const { return heightIncrement_; }
  void setHeightIncrement(int i) { heightIncrement_ = i; }

  const COptReal &sampLen() const { return sampLen_; }
  void setSampLen(double r) { sampLen_ = r; }

  const COptReal &spacing() const { return spacing_; }
  void setSpacing(double r) { spacing_ = r; }

  const COptString &title() const { return title_; }
  void setTitle(const std::string &s) { title_ = s; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  const OptPos &position() const { return pos_; }
  void setPosition(const CGnuPlotPosition &p) { pos_ = p; }

  //---

  bool hasBox() const { return box_; }
  void setBox(bool b) { box_ = b; }

  const COptInt &boxLineType() const { return boxLineType_; }
  void setBoxLineType(int lt) { boxLineType_ = lt; }
  void resetBoxLineType() { boxLineType_.setInvalid(); }

  const COptInt &boxLineStyle() const { return boxLineStyle_; }
  void setBoxLineStyle(int lt) { boxLineStyle_ = lt; }
  void resetBoxLineStyle() { boxLineStyle_.setInvalid(); }

  double boxLineWidth() const { return boxLineWidth_; }
  void setBoxLineWidth(double w) { boxLineWidth_ = w; }

  //---

  bool columnHead() const { return columnhead_; }
  void setColumnHead(bool b) { columnhead_ = b; }

  const COptInt &columnNum() const { return columnNum_; }
  void setColumnNum(int i) { columnNum_ = i; }
  void resetColumnNum() { columnNum_.setInvalid(); }

  const Columns &columns() const { return columns_; }
  void setColumns(const Columns &c) { columns_ = c; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &c) { textColor_ = c; }

  const COptInt &maxCols() const { return maxCols_; }
  void setMaxCols(int i) { maxCols_ = i; }
  void resetMaxCols() { maxCols_.setInvalid(); }

  const COptInt &maxRows() const { return maxRows_; }
  void setMaxRows(int i) { maxRows_ = i; }
  void resetMaxRows() { maxRows_.setInvalid(); }

  //---

  bool isColumnNumValid() const {
    const COptInt &inum = columnNum();
    if (! inum.isValid()) return false;

    int num = inum.getValue();
    return (num >= 1 && num <= int(columns().size()));
  }

  std::string columnNumValue() const {
    return columns_[columnNum_.getValue(0) - 1];
  }

  void reset() {
    displayed_       = true;
    outside_         = false;
    lmargin_         = false;
    rmargin_         = false;
    tmargin_         = false;
    bmargin_         = false;
    halign_          = CHALIGN_TYPE_RIGHT;
    valign_          = CVALIGN_TYPE_TOP;
    vertical_        = true;
    justify_         = CHALIGN_TYPE_RIGHT;
    right_           = true;
    reverse_         = false;
    invert_          = false;
    autotitle_       = true;
    opaque_          = false;
    enhanced_        = false;
    pos_             = OptPos();
    widthIncrement_  = 0;
    heightIncrement_ = 0;
    sampLen_         = COptReal();
    spacing_         = COptReal();
    title_           = COptString();
    font_            = CFontPtr();
    box_             = false;
    boxLineType_     = COptInt();
    boxLineStyle_    = COptInt();
    boxLineWidth_    = 1;
    columnhead_      = false;
    columnNum_       = COptInt();
    columns_         = Columns();
    textColor_       = CGnuPlotColorSpec();
    maxCols_         = COptInt();
    maxRows_         = COptInt();
  }

  void show(std::ostream &os) const;

 private:
  bool              displayed_  { true };               // key displayed
  bool              outside_    { false };              // inside/outside plot area
  bool              lmargin_    { false };
  bool              rmargin_    { false };
  bool              tmargin_    { false };
  bool              bmargin_    { false };
  CHAlignType       halign_     { CHALIGN_TYPE_RIGHT }; // key horizontal side
  CVAlignType       valign_     { CVALIGN_TYPE_TOP   }; // key vertical side
  bool              vertical_   { true };               // ??
  CHAlignType       justify_    { CHALIGN_TYPE_RIGHT };
  bool              right_      { true };               // text justification
  bool              reverse_    { false };              // reverse text and sample
  bool              invert_     { false };              // invert plot order
  bool              autotitle_  { true };
  bool              opaque_     { false };              // draw opaque
  bool              enhanced_   { false };              // enhanced text
  OptPos            pos_;
  int               widthIncrement_  { 0 };
  int               heightIncrement_ { 0 };
  COptReal          sampLen_;
  COptReal          spacing_;
  COptString        title_;
  CFontPtr          font_;
  bool              box_           { false };
  COptInt           boxLineType_;
  COptInt           boxLineStyle_;
  double            boxLineWidth_ { 1 };
  bool              columnhead_ { false };
  COptInt           columnNum_;
  Columns           columns_;
  CGnuPlotColorSpec textColor_;
  COptInt           maxCols_;
  COptInt           maxRows_;
};

#endif
