#ifndef CGnuPlotFinanceBarObject_H
#define CGnuPlotFinanceBarObject_H

#include <CGnuPlotObject.h>
#include <CGnuPlotEndBar.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotMark.h>

class CGnuPlotPlot;

class CGnuPlotFinanceBarObject : public CGnuPlotPlotObject {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotFinanceBarObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotFinanceBarObject() { }

  CGnuPlotFinanceBarObject(const CGnuPlotFinanceBarObject &) = delete;
  const CGnuPlotFinanceBarObject &operator=(const CGnuPlotFinanceBarObject &) = delete;

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  double low() const { return low_; }
  void setLow(double r) { low_ = r; }

  double high() const { return high_; }
  void setHigh(double r) { high_ = r; }

  double open() const { return open_; }
  void setOpen(double r) { open_ = r; }

  double close() const { return close_; }
  void setClose(double r) { close_ = r; }

  double barWidth() const { return barWidth_; }
  void setBarWidth(double r) { barWidth_ = r; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  double          value_     { 0.0 };
  double          low_      { 0.0 };
  double          high_     { 0.0 };
  double          open_     { 0.0 };
  double          close_    { 0.0 };
  double          barWidth_ { 0.0 };
  CGnuPlotStrokeP stroke_;
  mutable CBBox2D bbox_;
};

#endif
