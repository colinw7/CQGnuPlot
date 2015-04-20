#ifndef CGnuPlotLabel_H
#define CGnuPlotLabel_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotText.h>
#include <CGnuPlotColorSpec.h>
#include <CFont.h>

class CGnuPlotLabel : public CGnuPlotGroupAnnotation {
 public:
  typedef COptValT<CPoint2D> OptPoint;

 public:
  static const char *getName() { return "label"; }

  CGnuPlotLabel(CGnuPlotGroup *group);

  virtual ~CGnuPlotLabel() { }

  CGnuPlotLabel *setData(const CGnuPlotLabel *label) {
    (void) CGnuPlotGroupAnnotation::setData(label);

    text_   = label->text_;
    align_  = label->align_;
    pos_    = label->pos_;
    font_   = label->font_;
    angle_  = label->angle_;
    offset_ = label->offset_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::LABEL; }

  const CGnuPlotText &getText() const { return text_; }
  void setText(const CGnuPlotText &text) { text_ = text; }

  const CHAlignType &getAlign() const { return align_; }
  void setAlign(const CHAlignType &a) { align_ = a; }

  const CGnuPlotPosition &getPos() const { return pos_; }
  void setPos(const CGnuPlotPosition &p) { pos_ = p; }

  const CFontPtr &getFont() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  double getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  const OptPoint &getOffset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  bool showPoint() const { return showPoint_; }
  void setShowPoint(bool b) { showPoint_ = b; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &c) { textColor_ = c; }

  int lineType() const { return lineType_; }
  void setLineType(int i) { lineType_ = i; }

  int pointType() const { return pointType_; }
  void setPointType(int i) { pointType_ = i; }

  double pointSize() const { return pointSize_; }
  void setPointSize(double s) { pointSize_ = s; }

  bool hasBox() const { return box_; }
  void setBox(bool b) { box_ = b; }

  bool hasHypertext() const { return hypertext_; }
  void setHypertext(bool b) { hypertext_ = b; }

  const CBBox2D &getBBox() const { return bbox_; }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CPoint2D &p) const override;

  std::string tip() const override{ return ""; }

  CBBox2D tipRect() const override { return CBBox2D(); }

  void print(std::ostream &os) const;

 protected:
  static CFontPtr defFont_;

  CGnuPlotText      text_;
  CHAlignType       align_      { CHALIGN_TYPE_LEFT };
  CGnuPlotPosition  pos_;
  CFontPtr          font_;
  double            angle_      { -1 };
  OptPoint          offset_;
  bool              enhanced_   { true };
  bool              showPoint_  { false };
  CGnuPlotColorSpec textColor_;
  int               lineType_   { -1 };
  int               pointType_  { -1 };
  double            pointSize_  { -1 };
  bool              box_        { false };
  bool              hypertext_  { false };
  mutable CBBox2D   bbox_;
};

typedef std::shared_ptr<CGnuPlotLabel> CGnuPlotLabelP;

#endif
