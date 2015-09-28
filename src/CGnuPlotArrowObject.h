#ifndef CGnuPlotArrowObject_H
#define CGnuPlotArrowObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotArrowData.h>

class CGnuPlotPlot;

class CGnuPlotArrowObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotArrowData::CoordType CoordType;

 public:
  CGnuPlotArrowObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotArrowObject() { }

  const CGnuPlotArrowData &data() const { return data_; }
  void setData(const CGnuPlotArrowData &data) { data_ = data; }

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
  double calcLineWidth() const;

  int getLineStyle() const { return data_.getLineStyle(); }
  void setLineStyle(int t) { data_.setLineStyle(t); }

  const CGnuPlotDash &getDash() const { return data_.getDash(); }
  void setDash(const CGnuPlotDash &dash) { data_.setDash(dash); }

  void setLineColor(const CRGBA &c) { data_.setLineColor(c); }
  void resetLineColor() { data_.resetLineColor(); }
  CRGBA calcLineColor() const;

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  CGnuPlotArrowData data_;
};

#endif
