#ifndef CGnuPlotLabel_H
#define CGnuPlotLabel_H

#include <CGnuPlotObject.h>
#include <CGnuPlotText.h>
#include <CFont.h>

class CGnuPlotLabel : public CGnuPlotGroupAnnotation {
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

  const CPoint2D &getPos() const { return pos_; }
  void setPos(const CPoint2D &p) { pos_ = p; }

  const CFontPtr &getFont() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  double getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  const CPoint2D &getOffset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  int pointStyle() const { return pointStyle_; }
  void setPointStyle(int i) { pointStyle_ = i; }

  const CBBox2D &getBBox() const { return bbox_; }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &os) const {
    os << " \"";

    text_.print(os);

    os << "\"" << " at " << pos_;
  }

 protected:
  CGnuPlotText    text_;
  CHAlignType     align_      { CHALIGN_TYPE_LEFT };
  CPoint2D        pos_        { 0, 0 };
  CFontPtr        font_;
  double          angle_      { -1 };
  CPoint2D        offset_     { 0, 0 };
  bool            enhanced_   { true };
  int             pointStyle_ { -1 };
  mutable CBBox2D bbox_;
};

typedef std::shared_ptr<CGnuPlotLabel> CGnuPlotLabelP;

#endif
