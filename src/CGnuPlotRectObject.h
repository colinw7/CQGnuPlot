#ifndef CGnuPlotRectObject_H
#define CGnuPlotRectObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotRectObject : public CGnuPlotPlotObject {
 public:
  CGnuPlotRectObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotRectObject() { }

  const CBBox2D &rect() const { return rect_; }
  void setRect(const CBBox2D &r) { rect_ = r; }

  bool hasLineColor() const { return lineColor_.isValid(); }
  const CRGBA &lineColor() const { return lineColor_.getValue(); }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  bool hasFillColor() const { return fillColor_.isValid(); }
  const CRGBA &fillColor() const { return fillColor_.getValue(); }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  const std::string &tipText() const { return tipText_; }
  void setTipText(const std::string &v) { tipText_ = v; }

  bool inside(const CPoint2D &p) const override;

  std::string tip() const override { return (tipText_ == "" ? text_ : tipText_); }

  CBBox2D tipRect() const { return rect_; }

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  CBBox2D     rect_ { 0, 0, 1, 1 };
  COptRGBA    lineColor_;
  COptRGBA    fillColor_;
  double      lineWidth_ { 0 };
  std::string text_ { "" };
  std::string tipText_ { "" };
};

#endif
