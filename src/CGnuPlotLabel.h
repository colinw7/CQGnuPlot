#ifndef CGnuPlotLabel_H
#define CGnuPlotLabel_H

#include <CGnuPlotObject.h>
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

  const std::string &getText() const { return text_; }
  void setText(const std::string &t) { text_ = t; }

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

  void setEnhanced(bool b) { enhanced_ = b; }
  bool enhanced() const { return enhanced_; }

  void draw() const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &os) const {
    os << " \"" << text_ << "\"" << " at " << pos_;
  }

 protected:
  std::string text_;
  CHAlignType align_    { CHALIGN_TYPE_LEFT };
  CPoint2D    pos_      { 0, 0 };
  CFontPtr    font_;
  double      angle_    { -1 };
  CPoint2D    offset_   { 0, 0 };
  bool        enhanced_ { false };
};

typedef std::shared_ptr<CGnuPlotLabel> CGnuPlotLabelP;

#endif
