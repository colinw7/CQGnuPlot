#ifndef CGnuPlotRectObject_H
#define CGnuPlotRectObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotCache.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotRectObject : public CGnuPlotPlotObject {
 public:
  CGnuPlotRectObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotRectObject() { }

  const CBBox2D &rect() const { return rect_; }
  void setRect(const CBBox2D &r) { rect_ = r; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CBBox2D         rect_ { 0, 0, 1, 1 };
  std::string     text_ { "" };
  CGnuPlotFillP   fill_;
  CGnuPlotStrokeP stroke_;
};

#endif
