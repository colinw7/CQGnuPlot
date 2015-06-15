#ifndef CGnuPlotArrow_H
#define CGnuPlotArrow_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotArrowStyle.h>
#include <CAngle.h>

class CGnuPlotArrow : public CGnuPlotGroupAnnotation {
 public:
  enum class CoordType {
    FROM_TO,
    FROM_RTO,
    FROM_ANGLE
  };

 public:
  static const char *getName() { return "arrow"; }

  CGnuPlotArrow(CGnuPlotGroup *group) :
   CGnuPlotGroupAnnotation(group) {
  }

  virtual ~CGnuPlotArrow() { }

  CGnuPlotArrow *setData(const CGnuPlotArrow *arrow) {
    (void) CGnuPlotGroupAnnotation::setData(arrow);

    coordType_ = arrow->coordType_;
    from_      = arrow->from_;
    to_        = arrow->to_;
    length_    = arrow->length_;
    angle_     = arrow->angle_;
    style_     = arrow->style_;

    return this;
  }

  CGnuPlot *app() const;

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::ARROW; }

  CoordType coordType() const { return coordType_; }
  void setCoordType(CoordType type) { coordType_ = type; }

  void setFromTo(const CGnuPlotPosition &from, const CGnuPlotPosition &to) {
    coordType_ = CoordType::FROM_TO;
    from_      = from;
    to_        = to;
    length_    = CGnuPlotCoordValue();
    angle_     = CAngle();
  }

  void setFromRTo(const CGnuPlotPosition &from, const CGnuPlotPosition &rto) {
    coordType_ = CoordType::FROM_RTO;
    from_      = from;
    to_        = rto;
    length_    = CGnuPlotCoordValue();
    angle_     = CAngle();
  }

  void setFromAngle(const CGnuPlotPosition &from, const CGnuPlotCoordValue &length,
                    const CAngle &angle) {
    coordType_ = CoordType::FROM_ANGLE;
    from_      = from;
    length_    = length;
    angle_     = angle;
  }

  const CGnuPlotPosition &getFrom() const { return from_; }
  void setFrom(const CGnuPlotPosition &p) { from_ = p; }

  const CGnuPlotPosition &getTo() const { return to_; }
  void setTo(const CGnuPlotPosition &p) { to_ = p; }

  const CGnuPlotCoordValue &getLength() const { return length_; }
  void setLength(const CGnuPlotCoordValue &l) { length_ = l; }

  const CAngle &getAngle() const { return angle_; }
  void setAngle(const CAngle &a) { angle_ = a; }

  const CGnuPlotArrowStyle &style() const { return style_; }
  void setStyle(const CGnuPlotArrowStyle &s) { style_ = s; }

  bool getFHead() const { return style_.fhead(); }
  void setFHead(bool b) { style_.setFHead(b); }

  bool getTHead() const { return style_.thead(); }
  void setTHead(bool b) { style_.setTHead(b); }

  const CGnuPlotCoordValue &getHeadLength() const { return style_.length(); }
  void setHeadLength(const CGnuPlotCoordValue &l) { style_.setLength(l); }

  double getHeadAngle() const { return style_.angle(); }
  void setHeadAngle(double a) { style_.setAngle(a); }

  double getHeadBackAngle() const { return style_.backAngle(); }
  void setHeadBackAngle(double a) { style_.setBackAngle(a); }

  bool getHeadFilled() const { return style_.filled(); }
  void setHeadFilled(bool b) { style_.setFilled(b); }

  bool getHeadEmpty() const { return style_.empty(); }
  void setHeadEmpty(bool b) { style_.setEmpty(b); }

  double getLineWidth() const;
  void setLineWidth(double w) { style_.setLineWidth(w); }

  int getLineStyle() const { return style_.lineStyle().getValue(-1); }
  void setLineStyle(int t) { style_.setLineStyle(t); }

  int getDash() const { return style_.dash().getValue(0); }
  void setDash(int dash) { style_.setDash(dash); }

  bool isVariable() const { return style_.isVariable(); }
  void setVariable(bool b) { style_.setVariable(b); }

  double getVarValue() const { return style_.varValue().getValue(0); }
  void setVarValue(double r) { style_.setVarValue(r); }

  CRGBA getLineColor() const;

  void setLineColor(const CRGBA &c, bool set=true) {
    if (set)
      lineColor_.setValue(c);
    else
      lineColor_.setInvalid();
  }

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CPoint2D &p) const override;

  std::string tip() const override { return ""; }

  CBBox2D tipRect() const override { return CBBox2D(); }

  void print(std::ostream &os) const;

 protected:
  CoordType          coordType_ { CoordType::FROM_TO };
  CGnuPlotPosition   from_;
  CGnuPlotPosition   to_;
  CGnuPlotCoordValue length_;
  CAngle             angle_;
  CGnuPlotArrowStyle style_;
  COptRGBA           lineColor_;
};

typedef std::shared_ptr<CGnuPlotArrow> CGnuPlotArrowP;

#endif
