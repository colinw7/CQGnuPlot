#ifndef CGnuPlotArrow_H
#define CGnuPlotArrow_H

#include <CGnuPlotObject.h>
#include <CGnuPlotArrowStyle.h>

class CGnuPlotArrow : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "arrow"; }

  CGnuPlotArrow(CGnuPlotGroup *group) :
   CGnuPlotGroupAnnotation(group) {
  }

  virtual ~CGnuPlotArrow() { }

  CGnuPlotArrow *setData(const CGnuPlotArrow *arrow) {
    (void) CGnuPlotGroupAnnotation::setData(arrow);

    from_     = arrow->from_;
    to_       = arrow->to_;
    relative_ = arrow->relative_;
    style_    = arrow->style_;

    return this;
  }

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::ARROW; }

  const CPoint2D &getFrom() const { return from_; }
  void setFrom(const CPoint2D &p) { from_ = p; }

  const CPoint2D &getTo() const { return to_; }
  void setTo(const CPoint2D &p) { to_ = p; }

  bool getRelative() const { return relative_; }
  void setRelative(bool b) { relative_ = b; }

  const CGnuPlotArrowStyle &style() const { return style_; }
  void setStyle(const CGnuPlotArrowStyle &s) { style_ = s; }

  bool getFHead() const { return style_.fhead(); }
  void setFHead(bool b) { style_.setFHead(b); }

  bool getTHead() const { return style_.thead(); }
  void setTHead(bool b) { style_.setTHead(b); }

  const CGnuPlotCoordValue &getLength() const { return style_.length(); }
  void setLength(const CGnuPlotCoordValue &l) { style_.setLength(l); }

  double getLengthValue() const { return style_.lengthValue(); }
  void setLengthValue(double l) { style_.setLengthValue(l); }

  double getAngle() const { return style_.angle(); }
  void setAngle(double a) { style_.setAngle(a); }

  double getBackAngle() const { return style_.backAngle(); }
  void setBackAngle(double a) { style_.setBackAngle(a); }

  bool getFilled() const { return style_.filled(); }
  void setFilled(bool b) { style_.setFilled(b); }

  bool getEmpty() const { return style_.empty(); }
  void setEmpty(bool b) { style_.setEmpty(b); }

  double getLineWidth() const;
  void setLineWidth(double w) { style_.setLineWidth(w); }

  int getLineStyle() const { return style_.lineStyle(); }
  void setLineStyle(int t) { style_.setLineStyle(t); }

  bool getVariable() const { return style_.variable(); }
  void setVariable(bool b) { style_.setVariable(b); }

  const COptReal &getVarValue() const { return style_.varValue(); }
  void setVarValue(double r) { style_.setVarValue(r); }

  CRGBA getLineColor() const;

  void setLineColor(const CRGBA &c, bool set=true) {
    if (set)
      lineColor_.setValue(c);
    else
      lineColor_.setInvalid();
  }

  void draw() const override;

  bool inside(const CPoint2D &p) const;

  void print(std::ostream &os) const;

 protected:
  CPoint2D           from_     { 0, 0 };
  CPoint2D           to_       { 1, 1 };
  bool               relative_ { false };
  CGnuPlotArrowStyle style_;
  COptValT<CRGBA>    lineColor_;
};

typedef std::shared_ptr<CGnuPlotArrow> CGnuPlotArrowP;

#endif
