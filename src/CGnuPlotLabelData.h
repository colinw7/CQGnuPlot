#ifndef CGnuPlotLabelData_H
#define CGnuPlotLabelData_H

#include <CGnuPlotText.h>
#include <CGnuPlotLabelStyle.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotTipData.h>
#include <CGnuPlotMouseEvent.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotLabelData {
 public:
  typedef CGnuPlotTypes::SymbolType     SymbolType;
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotLabelData(CGnuPlotGroup *group=0);
  CGnuPlotLabelData(CGnuPlotPlot *plot);
 ~CGnuPlotLabelData();

  CGnuPlotLabelData(const CGnuPlotLabelData &) = delete;
  CGnuPlotLabelData &operator=(const CGnuPlotLabelData &l) = delete;

  CGnuPlotLabelData *dup() const;

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

  CGnuPlotFill *boxFill() const { return boxFill_; }
  void setBoxFill(const CGnuPlotFill &f) { *boxFill_ = f; }

  CGnuPlotStroke *boxStroke() const { return boxStroke_; }
  void setBoxStroke(const CGnuPlotStroke &s) { *boxStroke_ = s; }

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

  SymbolType symbolType() const { return (SymbolType) pointType(); }

  //---

  const CBBox2D &bbox() const { return bbox_; }

  const CRGBA &drawColor() const { return drawColor_; }

  //---

  void draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, bool highlighted) const;

  bool inside(const CGnuPlotMouseEvent &p) const;

  CGnuPlotTipData tip() const;

 protected:
  static CFontPtr defFont_;

  CGnuPlotGroup*     group_ { 0 };
  CGnuPlotPlot*      plot_  { 0 };
  std::string        text_;
  CGnuPlotPosition   pos_;
  COptReal           angle_;
  bool               enhanced_   { true };
  CGnuPlotLabelStyle labelStyle_;
  CGnuPlotColorSpec  textColor_;
  int                lineType_   { -1 };
  CGnuPlotFill*      boxFill_    { 0 };
  CGnuPlotStroke*    boxStroke_  { 0 };
  bool               hypertext_  { false };
  double             pointSize_  { 1 };
  double             pointWidth_ { 1 };
  mutable CBBox2D    bbox_;
  mutable CRGBA      drawColor_;
  mutable CPoint2D   drawPos_;
};

#endif
