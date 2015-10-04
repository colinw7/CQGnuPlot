#ifndef CGnuPlotLabelStyle_H
#define CGnuPlotLabelStyle_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotColorSpec.h>
#include <CFont.h>

class CGnuPlotLabelStyle {
 public:
  typedef CGnuPlotTypes::SymbolType   SymbolType;
  typedef COptValT<CGnuPlotColorSpec> OptColorSpec;

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

  //---

  const OptColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &c) { textColor_ = c; }
  void unsetTextColor() { textColor_.setInvalid(); }

  //---

  bool hasHypertext() const { return hypertext_; }
  void setHypertext(bool b) { hypertext_ = b; }

 protected:
  COptPoint2D  offset_;
  CHAlignType  align_     { CHALIGN_TYPE_CENTER };
  CFontPtr     font_;
  bool         showPoint_ { false };
  int          pointType_ { -1 };
  double       pointSize_ { -1 };
  double       lineWidth_ { 1 };
  OptColorSpec textColor_;
  bool         hypertext_ { false };
};

#endif
