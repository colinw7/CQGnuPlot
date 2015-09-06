#ifndef CGnuPlotPointObject_
#define CGnuPlotPointObject_

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotPointObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::SymbolType PointType;

 public:
  CGnuPlotPointObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPointObject() { }

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

  bool isErasePoint() const { return erasePoint_; }
  void setErasePoint(bool b) { erasePoint_ = b; }

  bool inside(const CPoint2D &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  CPoint2D       point_       { 0, 0 };
  PointType      pointType_   { CGnuPlotTypes::SymbolType::PLUS };
  COptReal       size_;
  CRGBA          color_;
  double         lineWidth_   { 1 };
  std::string    pointString_ { "" };
  bool           erasePoint_  { false };
  mutable double pw_          { 0 };
  mutable double ph_          { 0 };
};

#endif
