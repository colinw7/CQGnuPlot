#ifndef CGnuPlotLabel_H
#define CGnuPlotLabel_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotLabelStyle.h>
#include <CGnuPlotText.h>
#include <CGnuPlotColorSpec.h>
#include <CFont.h>

class CGnuPlotLabel : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "label"; }

  CGnuPlotLabel(CGnuPlotGroup *group);

  virtual ~CGnuPlotLabel() { }

  CGnuPlotLabel *setData(const CGnuPlotLabel *label) {
    (void) CGnuPlotGroupAnnotation::setData(label);

    text_  = label->text_;
    pos_   = label->pos_;
    angle_ = label->angle_;

    enhanced_   = label->enhanced_;
    labelStyle_ = label->labelStyle_;
    textColor_  = label->textColor_;
    lineType_   = label->lineType_;
    box_        = label->box_;
    hypertext_  = label->hypertext_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::LABEL; }

  const CGnuPlotText &getText() const { return text_; }
  void setText(const CGnuPlotText &text) { text_ = text; }

  const CHAlignType &getAlign() const { return labelStyle_.align(); }
  void setAlign(const CHAlignType &a) { labelStyle_.setAlign(a); }

  const CGnuPlotPosition &getPos() const { return pos_; }
  void setPos(const CGnuPlotPosition &p) { pos_ = p; }

  const CFontPtr &getFont() const { return labelStyle_.font(); }
  void setFont(const CFontPtr &f) { labelStyle_.setFont(f); }

  const COptReal getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }
  void resetAngle() { angle_.setInvalid(); }

  const COptPoint2D &getOffset() const { return labelStyle_.offset(); }
  void setOffset(const CPoint2D &o) { labelStyle_.setOffset(o); }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &c) { textColor_ = c; }

  int lineType() const { return lineType_; }
  void setLineType(int i) { lineType_ = i; }

  //---

  bool showPoint() const { return labelStyle_.showPoint(); }
  void setShowPoint(bool b) { labelStyle_.setShowPoint(b); }

  int pointType() const { return labelStyle_.pointType(); }
  void setPointType(int i) { labelStyle_.setPointType(i); }

  double pointSize() const { return labelStyle_.pointSize(); }
  void setPointSize(double s) { labelStyle_.setPointSize(s); }

  //---

  bool hasHypertext() const { return labelStyle_.hasHypertext(); }
  void setHypertext(bool b) { labelStyle_.setHypertext(b); }

  //---

  bool hasBox() const { return box_; }
  void setBox(bool b) { box_ = b; }

  const CBBox2D &getBBox() const { return bbox_; }

  const CRGBA &drawColor() const { return drawColor_; }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void print(std::ostream &os) const;

 protected:
  static CFontPtr defFont_;

  CGnuPlotText       text_;
  CGnuPlotPosition   pos_;
  COptReal           angle_;
  bool               enhanced_   { true };
  CGnuPlotLabelStyle labelStyle_;
  CGnuPlotColorSpec  textColor_;
  int                lineType_   { -1 };
  bool               box_        { false };
  bool               hypertext_  { false };
  mutable CBBox2D    bbox_;
  mutable CRGBA      drawColor_;
  mutable CPoint2D   drawPos_;
};

typedef std::shared_ptr<CGnuPlotLabel> CGnuPlotLabelP;

#endif
