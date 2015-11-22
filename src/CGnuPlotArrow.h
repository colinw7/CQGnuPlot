#ifndef CGnuPlotArrow_H
#define CGnuPlotArrow_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotArrowData.h>

class CGnuPlotArrow : public CGnuPlotGroupAnnotation {
 public:
  typedef CGnuPlotArrowData::CoordType CoordType;

 public:
  static const char *getName() { return "arrow"; }

  CGnuPlotArrow(CGnuPlotGroup *group);

  virtual ~CGnuPlotArrow() { }

  CGnuPlotArrow *setData(const CGnuPlotArrow *arrow);

  CGnuPlot *app() const;

  CGnuPlotTypes::ObjectType type() const override { return CGnuPlotTypes::ObjectType::ARROW; }

  CoordType coordType() const { return data_.coordType(); }
  void setCoordType(CoordType type) { data_.setCoordType(type); }

  const CGnuPlotPosition &getFrom() const { return data_.getFrom(); }
  void setFrom(const CGnuPlotPosition &p) { data_.setFrom(p); }

  const CGnuPlotPosition &getTo() const { return data_.getTo(); }
  void setTo(const CGnuPlotPosition &p) { data_.setTo(p); }

  const CGnuPlotCoordValue &getLength() const { return data_.getLength(); }
  void setLength(const CGnuPlotCoordValue &l) { data_.setLength(l); }

  const CAngle &getAngle() const { return data_.getAngle(); }
  void setAngle(const CAngle &a) { data_.setAngle(a); }

  const CGnuPlotArrowStyle &style() const { return data_.style(); }
  void setStyle(const CGnuPlotArrowStyle &s) { data_.setStyle(s); }

  void setFromTo(const CGnuPlotPosition &from, const CGnuPlotPosition &to) {
    setCoordType(CoordType::FROM_TO);
    setFrom     (from);
    setTo       (to);
    setLength   (CGnuPlotCoordValue());
    setAngle    (CAngle());
  }

  void setFromRTo(const CGnuPlotPosition &from, const CGnuPlotPosition &rto) {
    setCoordType(CoordType::FROM_RTO);
    setFrom     (from);
    setTo       (rto);
    setLength   (CGnuPlotCoordValue());
    setAngle    (CAngle());
  }

  void setFromAngle(const CGnuPlotPosition &from, const CGnuPlotCoordValue &length,
                    const CAngle &angle) {
    setCoordType(CoordType::FROM_ANGLE);
    setFrom     (from);
    setLength   (length);
    setAngle    (angle);
  }

  bool getFHead() const { return data_.getFHead(); }
  void setFHead(bool b) { data_.setFHead(b); }

  bool getTHead() const { return data_.getTHead(); }
  void setTHead(bool b) { data_.setTHead(b); }

  const CGnuPlotCoordValue &getHeadLength() const { return data_.getHeadLength(); }
  void setHeadLength(const CGnuPlotCoordValue &l) { data_.setHeadLength(l); }

  double getHeadAngle() const { return data_.getHeadAngle(); }
  void setHeadAngle(double a) { data_.setHeadAngle(a); }

  double getHeadBackAngle() const { return data_.getHeadBackAngle(); }
  void setHeadBackAngle(double a) { data_.setHeadBackAngle(a); }

  bool getHeadFilled() const { return data_.getHeadFilled(); }
  void setHeadFilled(bool b) { data_.setHeadFilled(b); }

  bool getHeadEmpty() const { return data_.getHeadEmpty(); }
  void setHeadEmpty(bool b) { data_.setHeadEmpty(b); }

  void setLineWidth(double w) { data_.setLineWidth(w); }
  double calcLineWidth() const { return data_.calcLineWidth(group_); }

  int getLineStyle() const { return data_.getLineStyle(); }
  void setLineStyle(int t) { data_.setLineStyle(t); }

  const CGnuPlotDash &getDash() const { return data_.getDash(); }
  void setDash(const CGnuPlotDash &dash) { data_.setDash(dash); }

  bool isVariable() const { return data_.isVariable(); }
  void setVariable(bool b) { data_.setVariable(b); }

  double getVarValue() const { return data_.getVarValue(); }
  void setVarValue(double r) { data_.setVarValue(r); }

  void setLineColor(const CRGBA &c) { data_.setLineColor(c); }
  void resetLineColor() { data_.resetLineColor(); }

  void setLineColor(const CGnuPlotColorSpec &c);
  CRGBA calcLineColor() const { return data_.calcLineColor(group_); }

  void initClip() override;

  void draw(CGnuPlotRenderer *renderer) const override;

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return data_.bbox(); }

  CGnuPlotTipData tip() const override;

  void print(std::ostream &os) const override;

 protected:
  CGnuPlotArrowData data_;
};

typedef std::shared_ptr<CGnuPlotArrow> CGnuPlotArrowP;

#endif
