#ifndef CGnuPlotEllipseObject_H
#define CGnuPlotEllipseObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotEllipseObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  CGnuPlotEllipseObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotEllipseObject() { }

  const CPoint2D &center() const { return center_; }
  void setCenter(const CPoint2D &center);

  const CSize2D &size() const { return size_; }
  void setSize(const CSize2D &size);

  bool hasLineColor() const { return lineColor_.isValid(); }
  const CRGBA &lineColor() const { return lineColor_.getValue(CRGBA(0,0,0)); }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }
  void resetLineColor() { lineColor_.setInvalid(); }

  bool hasFillColor() const { return fillColor_.isValid(); }
  const CRGBA &fillColor() const { return fillColor_.getValue(CRGBA(1,1,1)); }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }
  void resetFillColor() { fillColor_.setInvalid(); }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  const std::string &tipText() const { return tipText_; }
  void setTipText(const std::string &v) { tipText_ = v; }

  bool inside(const CPoint2D &p) const override;

  CGnuPlotTipData tip() const override {
    return CGnuPlotTipData(tipText_ == "" ? text_ : tipText_, rect_);
  }

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  void update();

 private:
  CPoint2D         center_;
  CSize2D          size_;
  COptRGBA         lineColor_;
  COptRGBA         fillColor_;
  double           lineWidth_ { 0 };
  std::string      text_ { "" };
  std::string      tipText_ { "" };
  mutable CBBox2D  rect_;
  mutable CPoint2D c_;
};

#endif
