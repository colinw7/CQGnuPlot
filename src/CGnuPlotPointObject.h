#ifndef CGnuPlotPointObject_H
#define CGnuPlotPointObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotPointData {
 public:
  typedef CGnuPlotTypes::SymbolType PointType;

 public:
  CGnuPlotPointData() { }

  const CPoint2D &point() const { return point_; }
  void setPoint(const CPoint2D &p) { point_ = p; }

  const PointType &pointType() const { return pointType_; }
  void setPointType(const PointType &t) { pointType_ = t; }

  const COptReal &size() const { return size_; }
  void setSize(double r) { size_ = r; }
  void resetSize() { size_.setInvalid(); }

  const CRGBA &color() const { return color_; }
  void setColor(const CRGBA &c) { color_ = c; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  const std::string &pointString() const { return pointString_; }
  void setPointString(const std::string &s) { pointString_ = s; }

  bool isPixelSize() const { return pixelSize_; }
  void setPixelSize(bool b) { pixelSize_ = b; }

  bool isErasePoint() const { return erasePoint_; }
  void setErasePoint(bool b) { erasePoint_ = b; }

  bool isVisible() const { return visible_; }
  void setVisible(bool b) { visible_ = b; }

 private:
  CPoint2D    point_       { 0, 0 };
  PointType   pointType_   { CGnuPlotTypes::SymbolType::PLUS };
  COptReal    size_;
  CRGBA       color_;
  double      lineWidth_   { 1 };
  std::string pointString_ { "" };
  bool        pixelSize_   { true };
  bool        erasePoint_  { false };
  bool        visible_     { true };
};

//---

class CGnuPlotPointObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::SymbolType PointType;

 public:
  CGnuPlotPointObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPointObject() { }

  void setData(const CGnuPlotPointData &data) { data_ = data; }

  const CPoint2D &point() const { return data_.point(); }
  void setPoint(const CPoint2D &p) { data_.setPoint(p); }

  const PointType &pointType() const { return data_.pointType(); }
  void setPointType(const PointType &t) { data_.setPointType(t); }

  const COptReal &size() const { return data_.size(); }
  void setSize(double r) { data_.setSize(r); }
  void resetSize() { data_.resetSize(); }

  const CRGBA &color() const { return data_.color(); }
  void setColor(const CRGBA &c) { data_.setColor(c); }

  double lineWidth() const { return data_.lineWidth(); }
  void setLineWidth(double r) { data_.setLineWidth(r); }

  const std::string &pointString() const { return data_.pointString(); }
  void setPointString(const std::string &s) { data_.setPointString(s); }

  bool isPixelSize() const { return data_.isPixelSize(); }
  void setPixelSize(bool b) { data_.setPixelSize(b); }

  bool isErasePoint() const { return data_.isErasePoint(); }
  void setErasePoint(bool b) { data_.setErasePoint(b); }

  bool isVisible() const { return data_.isVisible(); }
  void setVisible(bool b) { data_.setVisible(b); }

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  CGnuPlotPointData data_;
  mutable CBBox2D   bbox_;
  mutable CPoint2D  pc_;
  mutable double    pw_, ph_;
};

#endif
