#ifndef CGnuPlotLabelData_H
#define CGnuPlotLabelData_H

#include <CGnuPlotText.h>
#include <CGnuPlotLabelStyle.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotTipData.h>

class CGnuPlotLabelData {
 public:
  CGnuPlotLabelData();

  const std::string &getText() const { return text_; }
  void setText(const std::string &text) { text_ = text; }

  const CGnuPlotPosition &getPos() const { return pos_; }
  void setPos(const CGnuPlotPosition &p) { pos_ = p; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &c) { textColor_ = c; }

  const COptReal &getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }
  void resetAngle() { angle_.setInvalid(); }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  //---

  const CHAlignType &getAlign() const { return labelStyle_.align(); }
  void setAlign(const CHAlignType &a) { labelStyle_.setAlign(a); }

  const CFontPtr &getFont() const { return labelStyle_.font(); }
  void setFont(const CFontPtr &f) { labelStyle_.setFont(f); }

  const COptPoint2D &getOffset() const { return labelStyle_.offset(); }
  void setOffset(const CPoint2D &o) { labelStyle_.setOffset(o); }

  int lineType() const { return lineType_; }
  void setLineType(int i) { lineType_ = i; }

  //---

  bool isBoxFill() const { return boxFill_; }
  void setBoxFill(bool b) { boxFill_ = b; }

  const CRGBA &getBoxFillColor() const { return boxFillColor_; }
  void setBoxFillColor(const CRGBA &c) { boxFillColor_ = c; }

  //---

  bool hasBox() const { return box_; }
  void setBox(bool b) { box_ = b; }

  const CRGBA &getBoxStrokeColor() const { return boxStrokeColor_; }
  void setBoxStrokeColor(const CRGBA &v) { boxStrokeColor_ = v; }

  double getBoxStrokeWidth() const { return boxStrokeWidth_; }
  void setBoxStrokeWidth(double r) { boxStrokeWidth_ = r; }

  //---

  bool showPoint() const { return labelStyle_.showPoint(); }
  void setShowPoint(bool b) { labelStyle_.setShowPoint(b); }

  int pointType() const { return labelStyle_.pointType(); }
  void setPointType(int i) { labelStyle_.setPointType(i); }

  double pointSize() const { return labelStyle_.pointSize(); }
  void setPointSize(double s) { labelStyle_.setPointSize(s); }

  double pointWidth() const { return pointWidth_; }
  void setPointWidth(double r) { pointWidth_ = r; }

  //---

  bool hasHypertext() const { return labelStyle_.hasHypertext(); }
  void setHypertext(bool b) { labelStyle_.setHypertext(b); }

  //---

  const CBBox2D &getBBox() const { return bbox_; }

  const CRGBA &drawColor() const { return drawColor_; }

  //---

  void draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, bool highlighted) const;

  bool inside(const CGnuPlotTypes::InsideData &p) const;

  CGnuPlotTipData tip() const;

 protected:
  static CFontPtr defFont_;

  std::string        text_;
  CGnuPlotPosition   pos_;
  COptReal           angle_;
  bool               enhanced_       { true };
  CGnuPlotLabelStyle labelStyle_;
  CGnuPlotColorSpec  textColor_;
  int                lineType_       { -1 };
  bool               boxFill_        { false };
  CRGBA              boxFillColor_;
  bool               box_            { false };
  CRGBA              boxStrokeColor_;
  double             boxStrokeWidth_ { 1};
  bool               hypertext_      { false };
  double             pointSize_      { 1 };
  double             pointWidth_     { 1 };
  mutable CBBox2D    bbox_;
  mutable CRGBA      drawColor_;
  mutable CPoint2D   drawPos_;
};

#endif
