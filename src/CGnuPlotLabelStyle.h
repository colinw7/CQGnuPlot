#ifndef CGnuPlotLabelStyle_H
#define CGnuPlotLabelStyle_H

#include <CGnuPlotTypes.h>
#include <CFont.h>

class CGnuPlotLabelStyle {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotLabelStyle() { }

  const COptPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  const CHAlignType &align() const { return align_; }
  void setAlign(const CHAlignType &a) { align_ = a; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  //---

  bool showPoint() const { return showPoint_; }
  void setShowPoint(bool b) { showPoint_ = b; }

  int pointType() const { return pointType_; }
  void setPointType(int i) { pointType_ = i; }

  double pointSize() const { return pointSize_; }
  void setPointSize(double r) { pointSize_ = r; }

  //---

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  //---

  SymbolType symbolType() const {
    if (pointType_ < 0 || pointType_ > int(SymbolType::LAST))
      return SymbolType::PLUS;

    return SymbolType(pointType_);
  }

 protected:
  COptPoint2D offset_;
  CHAlignType align_     { CHALIGN_TYPE_LEFT };
  CFontPtr    font_;
  bool        showPoint_ { false };
  int         pointType_ { -1 };
  double      pointSize_ { -1 };
  double      lineWidth_ { 1 };
};

#endif
