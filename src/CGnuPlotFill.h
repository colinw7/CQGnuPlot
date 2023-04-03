#ifndef CGnuPlotFill_H
#define CGnuPlotFill_H

#include <CGnuPlotTypes.h>
#include <CRefPtr.h>

class CGnuPlotPlot;

class CGnuPlotFill {
 public:
  typedef CGnuPlotTypes::FillType    FillType;
  typedef CGnuPlotTypes::FillPattern FillPattern;

 public:
  CGnuPlotFill(CGnuPlotPlot *plot=nullptr);

  virtual ~CGnuPlotFill() { }

  virtual CGnuPlotFill *dup() const;

  void init(CGnuPlotPlot *plot);

  const CRGBA &color() const { return color_; }
  void setColor(const CRGBA &c) { color_ = c; }

  const CRGBA &background() const { return bg_; }
  void setBackground(const CRGBA &c) { bg_ = c; }

  const FillType &type() const { return type_; }
  void setType(const FillType &t) { type_ = t; }

  const FillPattern &pattern() const { return pattern_; }
  void setPattern(const FillPattern &p) { pattern_ = p; }

 protected:
  CGnuPlotPlot* plot_    { nullptr };
  CRGBA         color_;
  CRGBA         bg_;
  FillType      type_    { FillType::EMPTY };
  FillPattern   pattern_ { FillPattern::NONE };
};

typedef CRefPtr<CGnuPlotFill> CGnuPlotFillP;

#endif
