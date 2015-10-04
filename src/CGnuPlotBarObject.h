#ifndef CGnuPlotBarObject_H
#define CGnuPlotBarObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>

class CGnuPlotPlot;

class CGnuPlotBarObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::FillType    FillType;
  typedef CGnuPlotTypes::FillPattern FillPattern;

 public:
  class EndBar {
   public:
    EndBar(const CPoint2D &start, const CPoint2D &end) :
     start_(start), end_(end) {
    }

    const CPoint2D &start() const { return start_; }
    void setStart(const CPoint2D &v) { start_ = v; }

    const CPoint2D &end() const { return end_; }
    void setEnd(const CPoint2D &v) { end_ = v; }

    bool isStartLine() const { return startLine_; }
    void setStartLine(bool b) { startLine_ = b; }

    bool isEndLine() const { return endLine_; }
    void setEndLine(bool b) { endLine_ = b; }

    double lineWidth() const { return lineWidth_; }
    void setLineWidth(double r) { lineWidth_ = r; }

    const CRGBA &lineColor() const { return lineColor_; }
    void setLineColor(const CRGBA &v) { lineColor_ = v; }

    double endWidth() const { return endWidth_; }
    void setEndWidth(double r) { endWidth_ = r; }

   private:
    CPoint2D start_;
    CPoint2D end_;
    bool     startLine_ { false };
    bool     endLine_   { false };
    double   lineWidth_ { 0 };
    CRGBA    lineColor_;
    double   endWidth_  { 0 };
  };

 public:
  CGnuPlotBarObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBarObject() { }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; }

  double xvalue() const { return x_; }
  double yvalue() const { return y_; }
  void setValues(double x, double y) { x_ = x; y_ = y; }

  void setXValueStr(const std::string &s) { x_str_ = s; }
  void setYValueStr(const std::string &s) { y_str_ = s; }

  bool isInitialized() const { return initialized_; }
  void setInitialized(bool b) { initialized_ = b; }

  FillType fillType() const { return fillType_; }
  void setFillType(FillType t) { fillType_ = t; }

  FillPattern fillPattern() const { return fillPattern_; }
  void setFillPattern(FillPattern t) { fillPattern_ = t; }

  const COptRGBA &fillColor() const { return fillColor_; }
  void setFillColor(const COptRGBA &c) { fillColor_ = c; }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

  bool hasBorder() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  const COptRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const COptRGBA &c) { lineColor_ = c; }
  void setLineColor(const CRGBA &c) { lineColor_ = c; }

  double width() const { return width_; }
  void setWidth(double r) { width_ = r; }

  void clearEndBars();
  void addEndBar(const EndBar &endBar);

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  typedef std::vector<EndBar> EndBars;

  CBBox2D         bbox_        { 0, 0, 1, 1 };
  double          x_           { 0.0 }; // associated x value
  double          y_           { 0.0 }; // associated y value
  std::string     x_str_;
  std::string     y_str_;
  bool            initialized_ { false };
  FillType        fillType_    { FillType::EMPTY };
  FillPattern     fillPattern_ { FillPattern::NONE };
  COptRGBA        fillColor_;
  bool            border_      { true }; // border displayed
  COptRGBA        lineColor_;
  double          width_       { 1.0 }; // border value
  EndBars         endBars_;
};

#endif
