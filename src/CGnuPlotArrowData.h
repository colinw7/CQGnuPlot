#ifndef CGnuPlotArrowData_H
#define CGnuPlotArrowData_H

#include <CGnuPlotArrowStyle.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotTipData.h>
#include <CAngle.h>
#include <CLine2D.h>

class CGnuPlotGroup;

class CGnuPlotArrowData {
 public:
  enum class CoordType {
    FROM_TO,
    FROM_RTO,
    FROM_ANGLE
  };

 public:
  CGnuPlotArrowData() { }

  CoordType coordType() const { return coordType_; }
  void setCoordType(CoordType type) { coordType_ = type; }

  const CGnuPlotPosition &getFrom() const { return from_; }
  void setFrom(const CGnuPlotPosition &p) { from_ = p; }

  const CGnuPlotPosition &getTo() const { return to_; }
  void setTo(const CGnuPlotPosition &p) { to_ = p; }

  const CGnuPlotCoordValue &getLength() const { return length_; }
  void setLength(const CGnuPlotCoordValue &l) { length_ = l; }

  const CAngle &getAngle() const { return angle_; }
  void setAngle(const CAngle &a) { angle_ = a; }

  const CGnuPlotArrowStyle &style() const { return style_; }
  CGnuPlotArrowStyle &style() { return style_; }
  void setStyle(const CGnuPlotArrowStyle &s) { style_ = s; }

  const COptRGBA &getLineColor() const { return lineColor_; }
  void setLineColor(const CRGBA &c) { lineColor_.setValue(c); }
  void resetLineColor() { lineColor_.setInvalid(); }

  bool getFHead() const { return style().fhead(); }
  void setFHead(bool b) { style().setFHead(b); }

  bool getTHead() const { return style().thead(); }
  void setTHead(bool b) { style().setTHead(b); }

  const CGnuPlotCoordValue &getHeadLength() const { return style().length(); }
  void setHeadLength(const CGnuPlotCoordValue &l) { style().setLength(l); }

  double getHeadAngle() const { return style().angle(); }
  void setHeadAngle(double a) { style().setAngle(a); }

  double getHeadBackAngle() const { return style().backAngle(); }
  void setHeadBackAngle(double a) { style().setBackAngle(a); }

  bool getHeadFilled() const { return style().filled(); }
  void setHeadFilled(bool b) { style().setFilled(b); }

  bool getHeadEmpty() const { return style().empty(); }
  void setHeadEmpty(bool b) { style().setEmpty(b); }

  double getLineWidth() const { return style().lineWidth(); }
  void setLineWidth(double w) { style().setLineWidth(w); }

  int getLineStyle() const { return style().lineStyle().getValue(-1); }
  void setLineStyle(int t) { style().setLineStyle(t); }

  const CGnuPlotDash &getDash() const { return style().dash(); }
  void setDash(const CGnuPlotDash &dash) { style().setDash(dash); }

  bool isVariable() const { return style().isVariable(); }
  void setVariable(bool b) { style().setVariable(b); }

  double getVarValue() const { return style().varValue().getValue(0); }
  void setVarValue(double r) { style().setVarValue(r); }

  CRGBA calcLineColor(CGnuPlotGroup *group) const;
  double calcLineWidth(CGnuPlotGroup *group) const;

  void draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, bool highlighted=false) const;

  CGnuPlotTipData tip() const;

  void getLine(CGnuPlotRenderer *renderer, CPoint2D &from, CPoint2D &to) const;

  bool inside(const CGnuPlotTypes::InsideData &p) const;

  const CLine2D &drawLine     () const { return line_; }
  const CLine2D &drawPixelLine() const { return pline_; }

  double drawTolerance() const { return tol_; }

  const CRGBA &drawColor() const { return lc_; }

 protected:
  CoordType          coordType_ { CoordType::FROM_TO };
  CGnuPlotPosition   from_;
  CGnuPlotPosition   to_;
  CGnuPlotCoordValue length_;
  CAngle             angle_;
  CGnuPlotArrowStyle style_;
  COptRGBA           lineColor_;
  mutable CLine2D    line_;
  mutable CLine2D    pline_;
  mutable double     tol_;
  mutable CRGBA      lc_;
};

#endif
