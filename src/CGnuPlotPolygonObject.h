#ifndef CGnuPlotPolygonObject_H
#define CGnuPlotPolygonObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotPolygonObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D>      Points;
  typedef CGnuPlotTypes::FillPattern FillPattern;
  typedef COptValT<FillPattern>      OptFillPattern;

 public:
  CGnuPlotPolygonObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPolygonObject() { }

  const Points &points() const { return points_; }
  void setPoints(const Points &points);

  bool hasLineColor() const { return lineColor_.isValid(); }
  const CRGBA &lineColor() const { return lineColor_.getValue(); }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }
  void resetLineColor() { lineColor_.setInvalid(); }

  bool hasFillColor() const { return fillColor_.isValid(); }
  const CRGBA &fillColor() const { return fillColor_.getValue(); }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }
  void resetFillColor() { fillColor_.setInvalid(); }

  bool hasFillPattern() const {
    return fillPattern_.isValid() && fillPattern_.getValue() != CGnuPlotTypes::FillPattern::NONE;
  }
  const FillPattern &fillPattern() const { return fillPattern_.getValue(); }
  void setFillPattern(const FillPattern &p) { fillPattern_ = p; }
  void resetFillPattern() { fillPattern_.setInvalid(); }

  bool hasFillBackground() const { return fillBackground_.isValid(); }
  const CRGBA &fillBackground() const { return fillBackground_.getValue(); }
  void setFillBackground(const CRGBA &c) { fillBackground_ = c; }
  void resetFillBackground() { fillBackground_.setInvalid(); }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  Points           points_;
  COptRGBA         lineColor_;
  COptRGBA         fillColor_;
  OptFillPattern   fillPattern_;
  COptRGBA         fillBackground_;
  double           lineWidth_ { 0 };
  bool             clipped_ { false };
  std::string      text_ { "" };
  mutable CBBox2D  bbox_;
  mutable CPoint2D center_;
  mutable CRGBA    lc_;
};

#endif
